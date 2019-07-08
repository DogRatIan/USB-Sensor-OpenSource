# Console app

_lib : basic library
get_pa : Command line program for USB-PA
get_tnh : Command line program for USB-TnH

# Compile (native linux)
1) Install gcc compile.
```sh
$ sudo apt install build-essential
```
2) Build basic library
```sh
$ cd _lib
$ make clean
$ make
```
3) Build command line program
```sh
$ cd get_pa
$ make clean
$ make
```

# Compile (Windows, cross-build under Ubuntu 18.04)
1) Install MinGW
```sh
$ sudo apt install mingw-w64
```
2) Set enviroment for MinGW build
```sh
$ export MINGW32=1
```
3) Build basic library
```sh
$ cd _lib
$ make clean
$ make
```
4) Build command line program
```sh
$ cd get_pa
$ make clean
$ make
```

# Cross-compile for other linux
Modify the CROSS_PREFIX and CROSS_COMPILE define at makefile to match the cross-compiler you needed.
```sh
SYSTEM_MSW=0
SYSTEM_LINUX=1
OUT_FILE=$(PRJ_NAME)_linux
LIB=$(LIBDIR)/lib_linux.a
CROSS_PREFIX=arm-linux-gnueabihf-
CROSS_COMPILE=1
```
