# Taken from the "Linux Device Drivers": Alessandro Rubini

ifneq ($(KERNELRELEASE),)
   obj-m := driver.o
else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default:
 $(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
endif
