#include "cnet.h"
#include "cnet_tcp.h"

#define SMAC "11:22:33:44:55:66"
#define DMAC "00:11:22:33:44:55"
#define SRC "192.168.0.101"
#define DST "192.168.0.1"
#define SPORT 35423
#define DPORT 80

int main()
{
	int sock = CNET_SOCK(CNET_LAYER_ETHER, 0);
	
	struct cnet_buffer buff;
	CNET_MEMSET(&buff, sizeof(buff));
	struct cnet_buffer *cbuff = &buff;
	
	uint8_t *pkt = CNET_PACKET_UPTR(cbuff, 0);
	
	struct cnet_ether *eth = (struct cnet_ether *)pkt;
	struct cnet_ether_bytes msrc, mdst;
	CNET_MEMSET(&msrc, sizeof(msrc));
	CNET_MEMSET(&mdst, sizeof(mdst));
	CNET_MAC_BYTES(&msrc, SMAC);
	CNET_MAC_BYTES(&mdst, DMAC);
	CNET_SET_SMAC(eth, &msrc);
	CNET_SET_DMAC(eth, &mdst);
	eth->next = CNET_BIG16(CNET_ETH_IP);
	
	struct cnet_ip *cip = (struct cnet_ip *)(pkt + sizeof(*eth));
	cip->ihl = 5;
	cip->version = CNET_IP_VERSION;
	cip->type = CNET_IP_TYPE;
	cip->len = CNET_BIG16(sizeof(struct cnet_ip)+sizeof(struct cnet_tcp));
	cip->id = CNET_BIG16(67);
	cip->frag = CNET_BIG16(0);
	cip->ttl = CNET_IP_TTL;
	cip->proto = CNET_IP_TCP;
	
	struct cnet_ip_bytes src, dst;
	CNET_MEMSET(&src, sizeof(src));
	CNET_MEMSET(&dst, sizeof(dst));
	CNET_IP_BYTES(&src, SRC);
	CNET_IP_BYTES(&dst, DST);
	
	cip->CNET_IP_MACROS_SRC32 = src.bytes.b32its;
	cip->CNET_IP_MACROS_DST32 = dst.bytes.b32its;
	
	struct cnet_tcp *tcp = (struct cnet_tcp *)(pkt + IHL_LEN(cip->ihl) + sizeof(*eth));
	tcp->src = CNET_BIG16(SPORT);
	tcp->dst = CNET_BIG16(DPORT);
	tcp->seq = CNET_BIG32(8);
	tcp->aseq = CNET_BIG32(0);
	tcp->res1 = CNET_TCP_RES1;
	tcp->res2 = CNET_TCP_RES2;
	tcp->doff = CNET_TCP_DOFF;
	tcp->flags = CNET_TCP_SYN;
	tcp->win = CNET_BIG16(CNET_TCP_WIN);
	tcp->uptr = CNET_BIG16(0);
	
	cip->sum = CNET_L3_CSUM(cip, sizeof(*cip));
	tcp->sum = CNET_L4_CSUM(cip, tcp, sizeof(*tcp));
	
	CNET_SIZE_SET(cbuff, 0, sizeof(*tcp), CNET_LAYER_ETHER);
	
	for (int i = 0; i < APY; i++)
	{
		CNET_BUFFER_CLONE(cbuff, i, 0);
	}
	
	struct sockaddr_ll addr = CNET_SOCK_ADDR_LL(sock);
	
	CNET_BUFFER_INIT_ALL(cbuff, APY, &addr, CNET_LAYER_ETHER);
	CNET_BURST(cbuff, sock, APY);
	
	return 0;
}