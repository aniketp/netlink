CC = gcc
SRC = userspace.c
OBJ = userspace
obj-m = netlink_kernel.o
KVERSION = $(shell uname -r)

all:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
		$(CC) -o $(OBJ) $(SRC)
clean:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
		rm $(OBJ)
