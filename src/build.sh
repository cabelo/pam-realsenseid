dpcpp -shared pam_realsenseid.cpp -o pam_realsenseid.so -Wl,-rpath,/dados/Fontes/intel/RealSenseID/build/lib  /dados/Fontes/intel/RealSenseID/build/lib/librsid.so  -I/dados/Fontes/intel/RealSenseID/include/  -lpam -fPIC

