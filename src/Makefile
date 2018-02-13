KERNELDIR ?= /lib/modules/$(shell uname -r)/build/
SOURCEDIR ?= /home/heappl/median_dev/src
BUILD_DIR ?= /home/heappl/median_dev/build
BUILD_DIR_MAKEFILE ?= $(BUILD_DIR)/Makefile
FLAGS=-I/lib/modules/$(shell uname -r)/build/include
 
modname=median
obj-m := ${modname}.o
${modname}-objs := init.o median_dev.o linux_wrapper.o median_container.o
${modname}-objs += array.o allocator.o heap.o bigint.o

