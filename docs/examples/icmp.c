#include "cnet.h"
#include "cnet_icmp.h"

#define SRC "192.168.0.101"
#define DST "192.168.0.1"

int main()
{
	int sock = CNET_SOCK(CNET_LAYER_IP, CNET_IP_ICMP);
	
	struct cnet_buffer buff;
	CNET_MEMSET(&buff, sizeof(buff));
	struct cnet_buffer *cbuff = &buff;
	
	uint8_t *pkt = CNET_PACKET_UPTR(cbuff, 0);
	
	struct cnet_ip *cip = (struct cnet_ip *)pkt;
	cip->ihl = 5;
	cip->version = CNET_IP_VERSION;
	cip->type = CNET_IP_TYPE;
	cip->len = CNET_BIG16(sizeof(struct cnet_ip)+sizeof(struct cnet_icmp));
	cip->id = CNET_BIG16(67);
	cip->frag = CNET_BIG16(0);
	cip->ttl = CNET_IP_TTL;
	cip->proto = CNET_IP_ICMP;
	
	struct cnet_ip_bytes src, dst;
	CNET_MEMSET(&src, sizeof(src));
	CNET_MEMSET(&dst, sizeof(dst));
	CNET_IP_BYTES(&src, SRC);
	CNET_IP_BYTES(&dst, DST);
	
	cip->CNET_IP_MACROS_SRC32 = src.bytes.b32its;
	cip->CNET_IP_MACROS_DST32 = dst.bytes.b32its;
	
	struct cnet_icmp *icmp = (struct cnet_icmp *)(pkt + IHL_LEN(cip->ihl));
	icmp->type = CNET_ICMP_ECHO;
	icmp->code = CNET_ICMP_CODE_HOST_ISOLATED;
	icmp->CNET_ICMP_MACROS_ECHO.id = CNET_BIG16(67);
	icmp->CNET_ICMP_MACROS_ECHO.seq = CNET_BIG16(67);
	
	cip->sum = CNET_L3_CSUM(cip, sizeof(*cip));
	icmp->sum = CNET_L3_CSUM(icmp, sizeof(*icmp));
	
	CNET_SIZE_SET(cbuff, 0, sizeof(*icmp), CNET_LAYER_IP);
	
	for (int i = 0; i < APY; i++)
	{
		CNET_BUFFER_CLONE(cbuff, i, 0);
	}
	
	struct sockaddr_in addr = CNET_SOCK_ADDR_IN(cip);
	
	CNET_BUFFER_INIT_ALL(cbuff, APY, &addr, CNET_LAYER_IP);
	CNET_BURST(cbuff, sock, APY);
	
	return 0;
}