SOURCEDIR = src
BUILD_DIR ?= $(PWD)/build
BUILD_DIR_MAKEFILE ?= $(BUILD_DIR)/Makefile
FLAGS=-I/lib/modules/$(shell uname -r)/build/include

modname=median
obj-m += ${modname}.o
${modname}-objs := src/init.c src/median_dev.c src/linux_wrapper.c

all: $(BUILD_DIR_MAKEFILE) $(OBJECTS)
	make -C /lib/modules/$(shell uname -r)/build/ M=$(BUILD_DIR) src=$(PWD)/src modules

$(BUILD_DIR):
	@mkdir -p "$@"

$(BUILD_DIR_MAKEFILE): $(BUILD_DIR)
	@touch "$@"

clean: unload
	rm $(BUILD_DIR_MAKEFILE)
	make -C /lib/modules/$(shell uname -r)/build/ M=$(BUILD_DIR) src=$(PWD)/src clean

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

