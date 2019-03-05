#include <sys/socket.h>
#include <linux/netlink.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NLINK_MSG_LEN 1024

int main() {
    int netfd;
	struct sockaddr_nl dest_addr;

    // Create a Netlink socket
    netfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (netfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a unique address
    struct sockaddr_nl src_addr = {
        .nl_family  = AF_NETLINK,   // AF_NETLINK socket protocol
        .nl_pid     = getpid(),     // Application unique ID
        // The processes want to listen to addresses 3 and 5 (represented in bitmap)
        .nl_groups  = (1 << 3) | (1 << 5)
    };

    // Attach socket to the unique address
    if (bind(netfd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Netlink Message = Message Header + Message Payload
    struct nlmsghdr *nlh = (struct nlmsghdr *) malloc(NLMSG_SPACE(NLINK_MSG_LEN));
    memset(nlh, 0, NLMSG_SPACE(NLINK_MSG_LEN));

    struct iovec iov = {
        .iov_base   = (void *)nlh,      // Message header base address
        .iov_len    = nlh->nlmsg_len    // Netlink message length
    };

    // Construct message header
    struct msghdr msg = {
        .msg_name   = (void *)&dest_addr,
        .msg_namelen = sizeof(dest_addr),
        .msg_iov    = &iov,
        .msg_iovlen = 1
    };

    if (recvmsg(netfd, &msg, 0) < 0) {
        perror("recvmsg failed");
        exit(EXIT_FAILURE);
    }
}