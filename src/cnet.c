#include <cnet.h>

int CNET_RAND(int min, int max)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	uint32_t xorstate = 1;
	cnet_xorrand((uint32_t)(ts.tv_sec ^ ts.tv_nsec), &xorstate);
	
	uint32_t x = xorstate;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	xorstate = x;
	
	return min + (x % (max - min + 1));
}

struct sockaddr_ll CNET_SOCK_ADDR_LL(int sock)
{
	struct cnet_route crt;
	struct ifreq ifr;
	struct sockaddr_ll addr;
	CNET_MEMSET(&addr, sizeof(addr));
	CNET_MEMSET(&ifr, sizeof(ifr));
	CNET_MEMSET(&crt, sizeof(crt));
	CNET_GET_IF(&crt);
	strncpy(ifr.ifr_name, crt.iface, IFNAMSIZ-1);
	ioctl(sock, SIOCGIFINDEX, &ifr);
	
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = ifr.ifr_ifindex;
	
	return addr;
}

void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2)
{
	for (int i = 0; i < APX; i++)
	{
		cbuff->buff[cl1][i] = cbuff->buff[cl2][i];
	}
	
	cbuff->size[cl1] = cbuff->size[cl2];
}

void CNET_SCTP_CSUM(void *sctp, size_t len)
{
	uint32_t sum32[256];
	
	for (uint32_t i = 0; i < 256; i++)
	{
		uint32_t crc = i;
		for (uint32_t j = 0; j < 8; j++)
		{
			crc = (crc & 1)?(crc >> 1)^0x82F63B78:(crc >> 1);
		}
		sum32[i] = crc;
	}
	
	uint8_t *data = (uint8_t *)sctp;
	uint32_t crc32 = 0xFFFFFFFF;
	
	for (size_t i = 0; i < len; i++)
	{
		crc32 = sum32[(crc32 ^ data[i]) & 0xFF] ^ (crc32 >> 8);
	}
	
	uint32_t csum = crc32 ^ 0xFFFFFFFF;
	struct cnet_sctp *csctp = (struct cnet_sctp *)sctp;
	
	csctp->sum = csum;
}

cnet_errno_t CNET_GET_IF(void *crt)
{
	struct cnet_route *croute = (struct cnet_route *)crt;
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	
	if (getifaddrs(&ifaddr) == -1)
	{
		fprintf(stderr, "GET-IF-GETIF-ERR: %s (errno=%d)\n", strerror(errno), errno);
		return CNET_ERR_IFADDRS;
	}
	cnet_errno_t fnd = CNET_ERR_NIFACE;
	
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL) {continue;}
		if (ifa->ifa_flags & IFF_LOOPBACK) {continue;}
		if (!(ifa->ifa_flags & IFF_UP)) {continue;}
		if (ifa->ifa_addr->sa_family != AF_INET) {continue;}
		
		strncpy(croute->iface, ifa->ifa_name, IFNAMSIZ-1);
		croute->iface[IFNAMSIZ-1] = '\0';
		fnd = CNET_SCFL;
		break;
	}
	
	freeifaddrs(ifaddr);
	return fnd;
}

cnet_errno_t CNET_GET_IP(void *route)
{
	struct cnet_route *crt = (struct cnet_route *)route;
	CNET_GET_IF(crt);
	
	int usock = socket(AF_INET, SOCK_RAW, 0);
	
	if (usock < 0)
	{
		fprintf(stderr, "GET-IP-SOCK-ERR: %s (errno=%d)\n", strerror(errno), errno);
		return CNET_ERR_SOCKET;
	}
	
	struct ifreq ifr;
	struct ifreq *ifrq = &ifr;
	CNET_MEMSET(ifrq, sizeof(*ifrq));
	strncpy(ifrq->ifr_name, crt->iface, IFNAMSIZ-1);
	
	if (ioctl(usock, SIOCGIFADDR, ifrq) == -1) 
	{
		fprintf(stderr, "GET-IP-IOCTL-ERR: %s (errno=%d)\n", strerror(errno), errno);
		close(usock);
		return CNET_ERR_IOCTL;
	} else {
		struct sockaddr_in *addr = (struct sockaddr_in *)&ifrq->ifr_addr;
		uint8_t *ibytes = (uint8_t *)&addr->sin_addr;
		for (int i = 0; i < 4; i++) {crt->ip.i8p[i]=ibytes[i];}
		close(usock);
		return CNET_SCFL;
	}
}

cnet_errno_t CNET_GET_MAC(void *route)
{
	struct cnet_route *crt = (struct cnet_route *)route;
	CNET_GET_IF(crt);
	
	int usock = socket(AF_INET, SOCK_RAW, 0);
	
	if (usock < 0)
	{
		fprintf(stderr, "GET-MAC-SOCK-ERR: %s (errno=%d)\n", strerror(errno), errno);
		return CNET_ERR_SOCKET;
	}
	
	struct ifreq ifr;
	struct ifreq *ifrq = &ifr;
	CNET_MEMSET(ifrq, sizeof(*ifrq));
	strncpy(ifrq->ifr_name, crt->iface, IFNAMSIZ-1);
	
	if (ioctl(usock, SIOCGIFHWADDR, ifrq) == -1)
	{
		fprintf(stderr, "GET-MAC-IOCTL-ERR: %s (errno=%d)\n", strerror(errno), errno);
		close(usock);
		return CNET_ERR_IOCTL;
	} else {
		char *hwaddr = ifrq->ifr_ifru.ifru_hwaddr.sa_data;
		for (int i = 0; i < 6; i++) {crt->mac[i]=(uint8_t)hwaddr[i];}
		close(usock);
		return CNET_SCFL;
	}
}

void CNET_BUFFER_INIT_SET(struct cnet_buffer *cbuf, int ind, void *addr, int layer)
{
	cbuf->iov[ind].iov_base = cbuf->buff[ind];
	cbuf->iov[ind].iov_len = cbuf->size[ind];
	cbuf->msg[ind].msg_hdr.msg_iov = &cbuf->iov[ind];
	cbuf->msg[ind].msg_hdr.msg_iovlen = 1;
	
	if (layer==2)
	{
	    struct sockaddr_ll *scaddr = (struct sockaddr_ll *)addr;
    	cbuf->msg[ind].msg_hdr.msg_name = scaddr;
    	cbuf->msg[ind].msg_hdr.msg_namelen = sizeof(*scaddr);
	} else {
		struct sockaddr_in *scaddr = (struct sockaddr_in *)addr;
		cbuf->msg[ind].msg_hdr.msg_name = scaddr;
		cbuf->msg[ind].msg_hdr.msg_namelen = sizeof(*scaddr);
	}
};

void CNET_BUFFER_INIT_ALL(struct cnet_buffer *cbuf, int ind, void *addr, int layer)
{
	for (int i = 0; i < ind; i++) {
		cbuf->iov[i].iov_base = cbuf->buff[i];
		cbuf->iov[i].iov_len = cbuf->size[i];
		cbuf->msg[i].msg_hdr.msg_iov = &cbuf->iov[i];
		cbuf->msg[i].msg_hdr.msg_iovlen = 1;
		
		if (layer==2)
		{
		    struct sockaddr_ll *scaddr = (struct sockaddr_ll *)addr;
	    	cbuf->msg[i].msg_hdr.msg_name = scaddr;
	    	cbuf->msg[i].msg_hdr.msg_namelen = sizeof(*scaddr);
		} else {
			struct sockaddr_in *scaddr = (struct sockaddr_in *)addr;
			cbuf->msg[i].msg_hdr.msg_name = scaddr;
			cbuf->msg[i].msg_hdr.msg_namelen = sizeof(*scaddr);
		}
	}
}

int CNET_SOCK(int layer, uint8_t flag)
{
	if (layer == 2) {
    	int sockfd = socket(AF_PACKET, SOCK_RAW, 0);
    	
    	if (sockfd < 0) {
    		fprintf(stderr, "SOCK-ERR-CREATE: %s (errno=%d)\n", strerror(errno), errno);
    		return 0;
    	}
    	
    	struct cnet_route crt;
    	CNET_MEMSET(&crt, sizeof(crt));
    	struct ifreq ifr;
    	CNET_GET_IF(&crt);
    	memset(&ifr, 0, sizeof(ifr));
    	strncpy(ifr.ifr_name, crt.iface, IFNAMSIZ - 1);
	
    	if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
    		fprintf(stderr, "SOCK-ERR-IOCTL: %s (errno=%d)\n", strerror(errno), errno);
    		close(sockfd);
    		return 0;
    	}
	
    	struct sockaddr_ll sll;
    	memset(&sll, 0, sizeof(sll));
    	sll.sll_family = AF_PACKET;
    	sll.sll_ifindex = ifr.ifr_ifindex;
    	sll.sll_protocol = 0;
	
    	if (bind(sockfd, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
    		fprintf(stderr, "SOCK-ERR-BIND: %s (errno=%d)\n", strerror(errno), errno);
    		close(sockfd);
    		return 0;
    	}
	
    	int sbuf = SIZEBUFF();
    	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sbuf, sizeof(sbuf)) < 0) {
    		fprintf(stderr, "SOCK-ERR-SETOPT: %s (errno=%d)\n", strerror(errno), errno);
	    	close(sockfd);
	    	return 0;
    	}
	
    	int bflg = 1;
    	if (setsockopt(sockfd, SOL_PACKET, PACKET_QDISC_BYPASS, &bflg, sizeof(bflg)) < 0) {
    		fprintf(stderr, "SOCK-ERR-SETOPT: %s (errno=%d)\n", strerror(errno), errno);
	    	close(sockfd);
	    	return 0;
    	}
	
    	return sockfd;
	} else if (layer == 3) {
		int opt = 1;
		int sockfd = socket(AF_INET, SOCK_RAW, flag);
		
		if (sockfd < 0) {
			fprintf(stderr, "SOCK-ERR-CREATE: %s (errno=%d)\n", strerror(errno), errno);
			return 0;
		}
		
		if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt)) < 0) {
			fprintf(stderr, "SOCK-ERR-SETOPT: %s (errno=%d)\n", strerror(errno), errno);
			return 0;
		}
		
		return sockfd;
	} else {printf("CNET-SOCK-INVALID-LAYER\n");}
	
	return 0;
}

void CNET_MAC_BYTES(void *ceth, char mac[])
{
	struct cnet_ether_bytes *cether = (struct cnet_ether_bytes *)ceth;
	CNET_MEMSET(cether, sizeof(*cether));
	sscanf(mac, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &cether->bytes[0], &cether->bytes[1], &cether->bytes[2], &cether->bytes[3], &cether->bytes[4], &cether->bytes[5]);
}

void CNET_IP_BYTES(void *cipb, char sip[])
{
	struct cnet_ip_bytes *cipbyte = (struct cnet_ip_bytes *)cipb;
	CNET_MEMSET(cipbyte, sizeof(*cipbyte));
	sscanf(sip, "%hhu.%hhu.%hhu.%hhu", &cipbyte->bytes.b8its[0], &cipbyte->bytes.b8its[1], &cipbyte->bytes.b8its[2], &cipbyte->bytes.b8its[3]);
}

uint16_t CNET_L3_CSUM(void *ndata, size_t nlen)
{
	uint32_t sum = 0;
	uint16_t *rdata = (uint16_t *)ndata;
	
	while (nlen > 1)
	{
		sum += *rdata++;
		nlen -= 2;
	}
	
	if (nlen)
	{
		uint8_t temp[2] = {*(uint8_t *)rdata, 0x00};
		sum += *(uint16_t *)temp;
	}
	
	while (sum >> 16)
	{
		sum = (sum & 0xFFFF)+(sum >> 16);
	}
	
	return (uint16_t)(~sum);
}

uint16_t CNET_L4_CSUM(void *cip, void *ndata, size_t nlen)
{
	size_t tcplen = nlen;
	struct cnet_ip *cnip = (struct cnet_ip *)cip;
	uint32_t sum = 0;
	uint16_t *rdata = (uint16_t *)ndata;
	
	while (nlen > 1)
	{
		sum += *rdata++;
		nlen -= 2;
	}

	sum += CNET_BIG16((uint16_t)(tcplen));
	sum += CNET_BIG16((uint16_t)(0x00));
	sum += CNET_BIG16((uint16_t)(cnip->proto));
	sum += cnip->src.s32rc;
	sum += cnip->dst.d32st;
	
	if (nlen)
	{
		uint8_t temp[2] = {*(uint8_t *)rdata, 0x00};
		sum += *(uint16_t *)temp;
	}
	
	while (sum >> 16)
	{
		sum = (sum & 0xFFFF)+(sum >> 16);
	}
	
	return (uint16_t)(~sum);
}