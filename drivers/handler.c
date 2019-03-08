// A handler to communicate (or manipulate) with our character device
// via the registered device driver (driver.c)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int filedesc;
	char write_buff[100] = "Sample message", read_buff[100];

	if ((filedesc = open("/dev/char_device", O_RDWR)) < 0) {
		perror("Open /dev/char_device");
		exit(EXIT_FAILURE);
	}

	// Example write to the device
	if (write(filedesc, write_buff, sizeof(write_buff)) < 0)
		perror("Write error");
	close(filedesc);

	if ((filedesc = open("/dev/char_device", O_RDWR)) < 0) {
		perror("Open /dev/char_device");
		exit(EXIT_FAILURE);
	}

	// Example read from the device
	if (read(filedesc, read_buff, sizeof(read_buff)) < 0)
		perror("Read error");

	printf("%s\n", read_buff);
	close(filedesc);
}