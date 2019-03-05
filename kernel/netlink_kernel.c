#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#include <net/sock.h>

#define NETLINK_CUSTOM 31
#define UNICAST

struct sock *nl_sck = NULL;

static void hello_nl_recv_msg(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	struct sk_buff *skb_out;
	char *msg = "Hello from kernel";

	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

	nlh = (struct nlmsghdr *)skb->data;
	printk(KERN_INFO "Netlink received message payload: %s\n",
		   (char *)nlmsg_data(nlh));

	skb_out = nlmsg_new(strlen(msg), 0);
	if (!skb_out)
	{
		printk(KERN_INFO "Failed to allocate new sk_buffer\n");
		return;
	}

	// Store the allocated buffer back to skb
	nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, strlen(msg), 0);
	NETLINK_CB(skb_out).dst_group = 0;
	strncpy(nlmsg_data(nlh), msg, strlen(msg));

#ifdef UNICAST
	if (nlmsg_unicast(nl_sck, skb_out, nlh->nlmsg_pid) < 0)
		printk(KERN_INFO "Error while sending kernel's reply\n");
#endif
}

static int __init hello_init(void)
{
	// Specify Netlink callback function
	struct netlink_kernel_cfg cfg = {
		.groups = 1,
		.input = hello_nl_recv_msg,
	};

	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

	nl_sck = netlink_kernel_create(&init_net, NETLINK_CUSTOM, &cfg);
	if (!nl_sck)
	{
		printk(KERN_ALERT "Error creating socket.\n");
		return -1;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Exiting hello-world module\n");
	netlink_kernel_release(nl_sck);
}

module_init(hello_init);
module_exit(hello_exit);

// MODULE_LICENSE("GPL");