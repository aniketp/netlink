# netlink
Demonstration of userspace and kernel communication via Netlink-protocol (both Unicast and Multicast). Additionally, a character device-driver `/dev/char_device` to be expanded to a full-fledged module to use netlink for inter-process and inter-kernel communication instead of ioctls.

References:
* Linux Device Drivers, by Jonathan Corbet, Alessandro Rubini (Heavily)
* [writing-example-driver-from-scratch](http://tuxthink.blogspot.com/2010/10/writing-example-driver-from-scratch.html) [driver.c](drivers/driver.c)