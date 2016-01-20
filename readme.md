# **NGEngine** #

[![build status](https://gitlab.com/ci/projects/6667/status.png?ref=master)](https://gitlab.com/ci/projects/6667?ref=master)

Author: Tymoteusz Kubicz <tymoteusz.kubicz@gmail.com>

Project page: http://redmine.lynxware.eu/projects/nge/

Issue Tracker: http://redmine.lynxware.eu/projects/nge/issues

Documentation: http://redmine.lynxware.eu/projects/nge/wiki

## About ##
NGEngine (No Gravity Engine) is a multiplatform OpenGL graphics engine. Currently it works on Linux, Windows and Android.



## Installation
This section describes how to download and compile NGE on various operation systems.

### Download
First download the repository.
```sh
$ git clone git@gitlab.com:lynxware/ngengine.git
```
After that, initialise the submodules.
```sh
$ cd ngengine/
$ git submodules init
$ git submodules update
```

### Dependencies
NGE depends on various third party libraries that you need for compilation. On debain based systems you will need:
* g++
* cmake
* libglew-dev
* libglfw3-dev
* libboost-dev
* libboost-test-dev
* zlib1g-dev
* lua5.2
* liblua5.2-dev

Example usage on ubuntu:
```sh
$ sudo apt-get -y install g++ git cmake libglew-dev libglfw3-dev libboost-dev libboost-test-dev zlib1g-dev \
lua5.2 liblua5.2-dev libxrandr-dev libxcursor-dev
```

### Build
To build library use cmake:
```sh
$ cd ngengine/
$ mkdir build && cd build/
$ cmake ..
$ make
```
By default tests are build too. To run tests use:
```sh
$ ctest
```