#include <sys/socket.h>
#include <linux/netlink.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NLINK_MSG_LEN 1024

int main() {
    int netfd;

    // Create a Netlink socket
    netfd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_GENERIC);
    if (netfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a unique address
    struct sockaddr_nl src_addr = {
        .nl_family  = AF_NETLINK,   // AF_NETLINK socket protocol
        .nl_pid     = getpid(),     // Application unique ID
        .nl_groups  = 0             // Not a multicast communication
    };

    // Attach socket to the unique address
    if (bind(netfd, (struct sockaddr *) &src_addr, sizeof(src_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_nl dest_addr = {
        .nl_family  = AF_NETLINK,
        .nl_pid     = 26808,            // Destination process ID
        .nl_groups  = 0
    };

    // Netlink Message = Message Header + Message Payload
    struct nlmsghdr *nlh = (struct nlmsghdr *) malloc(NLMSG_SPACE(NLINK_MSG_LEN));
    nlh->nlmsg_len   = NLMSG_SPACE(NLINK_MSG_LEN);
    nlh->nlmsg_pid   = getpid();
    nlh->nlmsg_flags = 0;
    // NLMSG_DATA: Access the address of the payload
    strncpy(NLMSG_DATA(nlh), "Hello Process", sizeof(NLMSG_DATA(nlh)));

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

    if (sendmsg(netfd, &msg, 0) < 0) {
        perror("sendmsg failed");
        exit(EXIT_FAILURE);
    }
}