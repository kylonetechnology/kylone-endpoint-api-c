# Kylone Endpoint API for C implementation
Kylone provides an Endpoint API for developers to help creating a custom software to use with Kylone MicroCMS server including accessing and utilising several types of content related data located on server and ready to use methods while working with the server.

Beside accessing and using such data easily while doing development of custom endpoint applications, the main purpose of the API is to maintain functionalities on both server and endpoint side and to be sure about the consistency of whole framework.

Please check documentation for more information.

## Requirements
Kylone Endpoint API for C will support following platforms;

* Starting from MicroCMS v2.0.81
* Linux, arm-linux-gnueabi, Linaro GCC 4.9-2014.08
* Linux, aarch64-linux-gnu, Linaro GCC 4.9-2014.09
* Linux, csky-linux-gcc, v2.5.01, ABI v1, Build on 20131226, 4.5.1
* Linux , x86_64-linux-gnu, Linaro GCC 4.9-2014.08
* MacOS, x86_64-apple-darwin15.6.0, Apple LLVM version 8.0.0

## Demo Application
There is a demo application provided with the API. It is possible to compile using provided script and test the API with demo application like below;

	> ./build.sh make <armv7|aarch64|linux-x86_64|macos|csky>

It needs to edited src/Makefile for correct compiler options and library paths for cross compilation.
Demo application will be placed into bin folder once compiled.

### Trademarks
Products and Brands mentioned in this project are trademarks or registered trademarks of their respective holders.

