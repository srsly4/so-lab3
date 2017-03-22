pwd
ls -la



#ENVSETTEST tojesttest
printenv
./cmake-build-debug/envget ENVSETTEST
find /home/sirius/python -type f -exec md5sum {} +
