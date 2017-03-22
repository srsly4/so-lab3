#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_ARGUMENT_COUNT 32

void execute_command(char* cmd){
    __pid_t pid = fork();
    if (pid == -1){
        fprintf(stderr, "Could not have forked process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pid == 0){
        printf("Executing command: %s\n", cmd);
        //parsing arguments
        char** args = calloc(MAX_ARGUMENT_COUNT, sizeof(char*));
        int curr_args = 1;

        args[0] = strtok(cmd, " ");
        char *cmd_curr;
        while ((cmd_curr = strtok(NULL, " "))){
            args[curr_args] = cmd_curr;
            curr_args++;
            if (curr_args >= MAX_ARGUMENT_COUNT)
            {
                fprintf(stderr, "Max expression argument count size exceeded.\n");
                exit(EXIT_FAILURE);
            }
        }

        if (execvp(args[0], args) == -1){
            fprintf(stderr, "Could not have executed %s: %s\n", args[0], strerror(errno));
        }

        _exit(EXIT_SUCCESS);
    }
    else {
        printf("Running at PID: %d\n", pid);
        int status = 0;
        waitpid(pid, &status, 0);

        while (WIFSTOPPED(status) || WIFCONTINUED(status))
            waitpid(pid, &status, 0);

        if (WIFSIGNALED(status)){
            fprintf(stderr, "Command `%s` has been terminated by signal %d\n", cmd, WTERMSIG(status));
            exit(EXIT_FAILURE);
        }
        if (WEXITSTATUS(status) != 0){
            fprintf(stderr, "Command `%s` exited with status code %d\n", cmd, WEXITSTATUS(status));
            exit(WEXITSTATUS(status));
        }
    }

}

void execute_env_command(char* cmd){

    char *env_name = strtok(cmd, " ");
    char *env_param = strtok(NULL, " ");
    if (env_param != NULL){
        //sets the environment variable
        printf("Setting environment variable `%s` to `%s`\n", env_name, env_param);
        setenv(env_name, env_param, true);
    }
    else {
        //delete the env var
        printf("Deleting environment variable `%s`\n", env_name);
        unsetenv(env_name);
    }
}

void interprete_file(int file){
    struct stat fst;
    fstat(file, &fst);
    if (fst.st_size < 1){
        fprintf(stderr, "File cannot be empty.\n");
        exit(EXIT_FAILURE);
    }
    //initialize buffer
    char* fbuffer = malloc(fst.st_size*sizeof(char)+1);
    lseek(file, 0, SEEK_SET);
    read(file, fbuffer, (size_t)fst.st_size);
    fbuffer[fst.st_size] = '\0';

    char* cmd_start = fbuffer;
    size_t curr = 0;
    bool env_flag = false;
    size_t max_buff_size = (size_t)fst.st_size;
    while (fbuffer[curr] != '\0'){
        //if it is a first sign and its hash - mark a env flag
        if (fbuffer+curr == cmd_start && fbuffer[curr] == '#')
            env_flag = true;

        //if it is a endline, finish the comand
        if (fbuffer[curr] == '\n'
            || (curr+1 == max_buff_size && fbuffer[curr+1] == '\0')){
            if (fbuffer[curr] == '\n')
                fbuffer[curr] = '\0';
            //execute [cmd_start, cmd_end]
            if (!env_flag)
                execute_command(cmd_start);
            else
                execute_env_command(cmd_start+1);

            //start new command
            env_flag = false;
            cmd_start = fbuffer + curr + 1;
        }
        curr++;
    }

    free(fbuffer);
}

int main(int argc, char* argv[]){
    if (argc != 2)
    {
        fprintf(stderr, "Incorrect number of arguments\nUsage: <batch_name>\n");
        exit(EXIT_FAILURE);
    }

    char* batchname = argv[1];
    int file;
    if ((file = open(batchname, O_RDONLY)) == -1)
    {
        fprintf(stderr, "Can't open batch file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    interprete_file(file);

    close(file);
    return EXIT_SUCCESS;
}
