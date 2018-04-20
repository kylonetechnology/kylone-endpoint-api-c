
current_dir = $(shell pwd)
CC = gcc
CFDEFS = -Wall -O2 -I../inc
CFLAGS = $(CFDEFS)
LDPATH = -L../lib
LDLIBS = 
LDFILE = libcLshcapi.so

ifeq (${TARGET}, linux-x86_64)
  SYSROOT = /opt/devel/local/api/x86_64-linux-gnu-4.9-2014.08
  CC = $(SYSROOT)/bin/x86_64-linux-gnu-gcc
  CFLAGS = --sysroot=$(SYSROOT)/x86_64-linux-gnu/sysroot $(CFDEFS) -fPIC -isystem $(SYSROOT)/include
  LDLIBS = -lstdc++ -lz
  LDPATH = -L../lib/${TARGET} -L/opt/devel/projects/cLsuite/deps/Linux-x86_64/lib
endif
ifeq (${TARGET}, aarch64)
  SYSROOT = /opt/devel/local/api/aarch64-linux-gnu-4.9-2014.09
  CC = $(SYSROOT)/bin/aarch64-linux-gnu-gcc
  CFLAGS = --sysroot=$(SYSROOT)/aarch64-linux-gnu/libc $(CFDEFS) -fPIC -isystem $(SYSROOT)/include
  LDLIBS = -lstdc++ -lz -lrt -lnsl -lm
  LDPATH = -L../lib/${TARGET} -L/opt/devel/projects/cLsuite/deps/${TARGET}/lib
endif
ifeq (${TARGET}, armv7)
  SYSROOT = /opt/devel/local/api/arm-linux-gnueabi-4.9-2014.08
  CC = $(SYSROOT)/bin/arm-linux-gnueabi-gcc
  CFLAGS = --sysroot=$(SYSROOT)/arm-linux-gnueabi/libc $(CFDEFS) -ffunction-sections -fdata-sections -mfloat-abi=soft -march=armv7 -fPIC -isystem $(SYSROOT)/include
  LDLIBS = -lstdc++ -lz -lrt -lnsl -lm
  LDPATH = -L../lib/${TARGET} -L/opt/devel/projects/cLsuite/deps/${TARGET}/lib
endif
ifeq (${TARGET}, csky)
  SYSROOT = /opt/devel/local/api/csky-linux-i386
  CC = $(SYSROOT)/bin/csky-linux-gcc
  CFLAGS = --sysroot=$(SYSROOT)/csky-linux/sysroot -Wall -Os -mlittle-endian  -Wpointer-arith -fPIC -I../inc -isystem $(SYSROOT)/include
  LDLIBS = -lstdc++ -lz
  LDPATH = -L../lib/${TARGET} -L/opt/devel/projects/cLsuite/deps/csky-linux/lib
endif
ifeq (${TARGET}, macos)
  CC = clang
  LDPATH = -L../lib/${TARGET}
  LDLIBS = -framework Foundation -framework CoreFoundation
  LDFILE = libcLshcapi.dylib
endif

LDFLAGS = $(LDPATH) -lcLshcapi -ldl -lpthread $(LDLIBS)

shcapitest : shcapitest.c ../lib/${TARGET}/$(LDFILE)
	mkdir -p ../bin
	$(CC) $(CFLAGS) shcapitest.c -o ../bin/shcapitest $(LDFLAGS)
ifeq (${TARGET}, macos)
	install_name_tool -change $(LDFILE) $(current_dir)/../lib/${TARGET}/$(LDFILE) ../bin/shcapitest
endif

clean:
	rm -f shcapitest.o ../bin/shcapitest

