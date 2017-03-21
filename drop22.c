#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dorofeev");
MODULE_DESCRIPTION("Drop SSH";)

static struct nf_hook_ops netfilter_ops;
struct sk_buff *sock_buff;
struct udphdr *udp_header;
struct tcphdr *tcp_header;
struct iphdr *ip_header;
unsigned short port = htons(22);

unsigned int main_hook(void *priv,
		struct sk_buff *skb,
		const struct nf_hook_state *state)
{
	ip_header = (struct iphdr *)skb_network_header(skb);

	if (ip_header->protocol == IPPROTO_UDP) {
		udp_header = (struct udphdr *)skb_transport_header(skb);
		if ((udp_header->dest) == port)
			return NF_DROP;
	}

	if (ip_header->protocol == IPPROTO_TCP) {
		tcp_header = (struct tcphdr *)skb_transport_header(skb);
		if ((tcp_header->dest) == port)
			return NF_DROP;
	}

	return NF_ACCEPT;
}

int init_module(void)
{
	netfilter_ops.hook = main_hook;
	netfilter_ops.pf = PF_INET;
	netfilter_ops.hooknum = 0;
	netfilter_ops.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&netfilter_ops);

	return 0;
}

void cleanup_module(void)
{
	nf_unregister_hook(&netfilter_ops);
}

