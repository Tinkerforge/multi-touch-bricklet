@rem windows command shell batch script
cmake -E chdir build/ cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM:PATH=make.exe -DCMAKE_TOOLCHAIN_FILE=toolchains/arm-none-eabi.cmake ../
