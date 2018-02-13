# simple_linux_dev

It is just a simple try out how to create a device kernel module.

* To run tests: **make test** (will compile using CMake as a normal C)
* To compile within kernel: **make [all]**
* To compile and load kernel: **make load**  (will require sudo password to load the module)
* To unload kernel: **make unload** (will require sudo as well)

