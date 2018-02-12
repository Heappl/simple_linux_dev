KERNELDIR ?= /lib/modules/$(shell uname -r)/build/
SOURCEDIR ?= /home/heappl/median_dev/src
BUILD_DIR ?= /home/heappl/median_dev/build
BUILD_DIR_MAKEFILE ?= $(BUILD_DIR)/Makefile
FLAGS=-I/lib/modules/$(shell uname -r)/build/include
 
modname=median
obj-m := ${modname}.o
${modname}-objs := init.o median_dev.o linux_wrapper.o


all: $(BUILD_DIR_MAKEFILE) $(OBJECTS)
	$(MAKE) -C $(KERNELDIR)  M=$(BUILD_DIR) src=$(SOURCEDIR) modules

$(BUILD_DIR):
	@mkdir -p "$@"

$(BUILD_DIR_MAKEFILE): $(BUILD_DIR)
	@touch "$@"

clean: unload
	rm $(BUILD_DIR_MAKEFILE)
	$(MAKE) -C $(KERNELDIR) M=$(BUILD_DIR) src=$(SOURCEDIR) clean

load: all
	@sudo rmmod median.ko || echo ""
	@sudo insmod $(BUILD_DIR)/median.ko
	@sudo chmod +777 /dev/median

unload:
	@sudo rmmod median.ko || echo ""
    
prepare_test: 
	mkdir -p test_build
	cd test_build && cmake ..

test:  prepare_test
	$(MAKE) -C test_build
	ctest -C test_build

