# Taken from the "Linux Device Drivers": Alessandro Rubini
CC  = gcc
SRC = handler.c
OBJ = handler

ifneq ($(KERNELRELEASE),)
   obj-m := driver.o
else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
		$(CC) -o $(OBJ) $(SRC)
clean:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
		rm $(OBJ)
endif
