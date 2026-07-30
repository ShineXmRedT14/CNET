#include "cnet.h"
#include "cnet_udp.h"

#define SRC "192.168.0.101"
#define DST "192.168.0.1"
#define SPORT 35423
#define DPORT 80

int main()
{
	int sock = CNET_SOCK(CNET_LAYER_IP, CNET_IP_UDP);
	
	struct cnet_buffer buff;
	CNET_MEMSET(&buff, sizeof(buff));
	struct cnet_buffer *cbuff = &buff;
	
	uint8_t *pkt = CNET_PACKET_UPTR(cbuff, 0);
	
	struct cnet_ip *cip = (struct cnet_ip *)pkt;
	cip->ihl = 5;
	cip->version = CNET_IP_VERSION;
	cip->type = CNET_IP_TYPE;
	cip->len = CNET_BIG16(sizeof(struct cnet_ip)+sizeof(struct cnet_udp));
	cip->id = CNET_BIG16(67);
	cip->frag = CNET_BIG16(0);
	cip->ttl = CNET_IP_TTL;
	cip->proto = CNET_IP_UDP;
	
	struct cnet_ip_bytes src, dst;
	CNET_MEMSET(&src, sizeof(src));
	CNET_MEMSET(&dst, sizeof(dst));
	CNET_IP_BYTES(&src, SRC);
	CNET_IP_BYTES(&dst, DST);
	
	cip->CNET_IP_MACROS_SRC32 = src.bytes.b32its;
	cip->CNET_IP_MACROS_DST32 = dst.bytes.b32its;
	
	struct cnet_udp *udp = (struct cnet_udp *)(pkt + IHL_LEN(cip->ihl));
	udp->src = CNET_BIG16(SPORT);
	udp->dst = CNET_BIG16(DPORT);
	udp->len = CNET_BIG16(sizeof(*udp));
	
	cip->sum = CNET_L3_CSUM(cip, sizeof(*cip));
	udp->sum = CNET_L4_CSUM(cip, udp, sizeof(*udp));
	
	CNET_SIZE_SET(cbuff, 0, sizeof(*udp), CNET_LAYER_IP);
	
	for (int i = 0; i < APY; i++)
	{
		CNET_BUFFER_CLONE(cbuff, i, 0);
	}
	
	struct sockaddr_in addr = CNET_SOCK_ADDR_IN(cip);
	
	CNET_BUFFER_INIT_ALL(cbuff, APY, &addr, CNET_LAYER_IP);
	CNET_BURST(cbuff, sock, APY);
	
	return 0;
}