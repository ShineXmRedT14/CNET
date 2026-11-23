#ifndef CNET_H
#define CNET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cnet_platform.h>

#if defined(CNET_PLATFORM_LINUX)
    #ifndef _GNU_SOURCE
    #define _GNU_SOURCE
    #endif

    #include <sys/socket.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
    #include <netinet/tcp.h>
    #include <linux/if_packet.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <endian.h>

#elif defined(CNET_PLATFORM_WINDOWS)
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #include <pcap/pcap.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")

    #ifndef IFNAMSIZ
    #define IFNAMSIZ 256
    #endif

	#include <cnet_endian.h>

#elif defined(CNET_PLATFORM_ESP32)
    #include <cnet_endian.h>
#else
#error "cnet.h: platform error."
#endif

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

#include <cnet_ip.h>
#include <cnet_tcp.h>
#include <cnet_udp.h>
#include <cnet_igmp.h>
#include <cnet_icmp.h>
#include <cnet_ipv6.h>
#include <cnet_icmpv6.h>
#include <cnet_vlan.h>
#include <cnet_ether.h>
#include <cnet_sctp.h>
#include <cnet_buffer.h>
#include <cnet_route.h>
#include <cnet_helper.h>

static inline int IP_IHL_CODE(int data);
static inline int IP_IHL_DECODE(int data);
static inline void CNET_IPV6_VTC(void *ipv6, struct cnet_dipv6 *dipv6);
static inline void CNET_ICMPV6_RSO(void *icmpv6, struct cnet_dicmpv6 *dicmpv6);
static inline void CNET_VLAN_VDP(void *vlan, struct cnet_dvlan *dvlan);
static inline void CNET_DIRECT_SWAP16(uint8_t *uptr, size_t id);
static inline void CNET_DIRECT_SWAP32(uint8_t *uptr, size_t id);
static inline void CNET_RAND_IP(void *nip);
static inline void CNET_RAND_MAC(void *nmac);
static inline void CNET_SET_SMAC(void *eth, void *sbytes);
static inline void CNET_SET_DMAC(void *eth, void *dbytes);
static inline void CNET_MEMSET(void *cstruct, size_t nlen);
static inline uint16_t CNET_BIG16(uint16_t bdata);
static inline uint16_t CNET_LITTLE16(uint16_t bdata);
static inline uint32_t CNET_BIG32(uint32_t bdata);
static inline uint32_t CNET_LITTLE32(uint32_t bdata);
static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate);
static inline void *CNET_CREATE_LAYER(uint8_t *pkt, int *offset, int size);

uint32_t CNET_GET_IP_TIMESTAMP();
uint16_t CNET_L3_CSUM(void *ndata, size_t nlen);
uint16_t CNET_L4_CSUM(uint8_t type, void *l3, void *l4, size_t len);
void CNET_IP_BYTES(void *cipb, char sip[]);
void CNET_MAC_BYTES(void *ceth, char mac[]);
void CNET_FRAME_FCS(struct cnet_frame_fcs *ffcs, void *data, size_t len);
void CNET_SCTP_CSUM(void *sctp, size_t len);
void CNET_PACKET_IP_FAST_SET_TIMESTAMP(struct cnet_ip_timestamp *timestamp, uint8_t custom, const char *ip);
void CNET_PACKET_TCP_FAST_SET_INFO(struct cnet_tcp_info *info, uint8_t flag);
void CNET_EXPAND(struct cnet_expand_info *info);
int CNET_RAND(int min, int max);

struct cnet_ntp_dtime CNET_NTP_TIMESTAMP();
cnet_errno_t CNET_PACKET_TCP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_tcp *tcp, void *ip, uint8_t type);
cnet_errno_t CNET_PACKET_UDP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_udp *udp, void *ip, uint8_t type);
cnet_errno_t CNET_PACKET_IP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_ip *ip, uint8_t options);
cnet_errno_t CNET_PACKET_ETHER_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_ether *ether);

#if defined(CNET_PLATFORM_LINUX)
    static inline struct sockaddr_in CNET_SOCK_ADDR_IN(struct cnet_ip *cip);
	void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2);
    struct sockaddr_ll CNET_SOCK_ADDR_LL(struct cnet_route *route, int sock);
	cnet_errno_t CNET_CONFIGURE(struct cnet_route *route);
	static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, int offset);
	void CNET_BUFFER_INIT_ALL(struct cnet_buffer *cbuf, int ind, void *addr, int layer);
    void CNET_BUFFER_INIT_SET(struct cnet_buffer *cbuf, int ind, void *addr, int layer);
	int CNET_SOCK(struct cnet_route *route, int layer, uint8_t flag);
	static inline void CNET_BURST(struct cnet_buffer *cbuf, int sockfd, int hlen);
	cnet_errno_t CNET_GET_TCP_INFO(int socket, struct cnet_tcp_info *info);
	struct cnet_ring CNET_RING_INIT(int layer, struct cnet_route *route);
	static inline void CNET_RING_BURST(struct cnet_ring *ring, struct cnet_buffer *buff, int len);
	static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index);

	static inline void CNET_RING_BURST(struct cnet_ring *ring, struct cnet_buffer *buff, int len)
    {
	    for (int i = 0; i < len; i++)
	    {
		    volatile struct tpacket_hdr *hdr = (struct tpacket_hdr *)(ring->ring_ptr + (ring->position * ring->frame_size));

		    while (hdr->tp_status != TP_STATUS_AVAILABLE) {
    #ifdef __x86_64__
                __builtin_ia32_pause();
    #endif
            }

		    uint8_t *data = (uint8_t *)hdr + TPACKET_ALIGN(sizeof(struct tpacket_hdr));
		    memcpy(data, buff->buff[i], buff->size[i]);
		    hdr->tp_len = buff->size[i];
		    hdr->tp_status = TP_STATUS_SEND_REQUEST;
		    ring->position = (ring->position + 1) % ring->frame_count;
	    }
    }

	static inline struct sockaddr_in CNET_SOCK_ADDR_IN(struct cnet_ip *cip)
    {
	    struct sockaddr_in addr;
	    CNET_MEMSET(&addr, sizeof(addr));
	    addr.sin_family = AF_INET;
	    addr.sin_addr.s_addr = cip->CNET_IP_MACROS_DST32;
	    return addr;
    }

	static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index)
    {
	    uint8_t* uptr = (uint8_t *)cbuf->buff[index];
	    CNET_MEMSET(uptr, APX);
	    return uptr;
    }

	static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, size_t offset)
    {
	    cbuf->size[index] = offset;
    }

static inline void CNET_BURST(struct cnet_buffer *cbuf, int sockfd, int hlen)
{
	int cerr = sendmmsg(sockfd, cbuf->msg, hlen, 0);
	if (cerr < 0) {fprintf(stderr, "CNET/BURST-ERR: %s (errno=%d)\n", strerror(errno), errno);}
}

#elif defined(CNET_PLATFORM_WINDOWS)
    void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2);
	static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, int offset);
	void CNET_BUFFER_INIT(struct cnet_buffer *cbuf);
    void CNET_BUFFER_PACKETS(struct cnet_buffer *cbuf, int ind);
	cnet_errno_t CNET_CONFIGURE(struct cnet_route *route, pcap_t *socket);
	pcap_t* CNET_SOCK(struct cnet_route *route);
	static inline void CNET_BURST(struct cnet_buffer *cbuf, pcap_t* socket);
	static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index);

	static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index)
    {
	    uint8_t* uptr = (uint8_t *)cbuf->buff[index];
	    CNET_MEMSET(uptr, APX);
	    return uptr;
    }

    static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, int offset)
    {
	    cbuf->size[index] = offset;
    }

    static inline void CNET_BURST(struct cnet_buffer *cbuf, pcap_t *socket)
    {
	    uint32_t bytes = pcap_sendqueue_transmit(socket, cbuf->buffer, 0);
	    if (bytes < 1) {fprintf(stderr, "CNET/BURST-ERR");}
    }

#elif defined(CNET_PLATFORM_ESP32)
    static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, size_t len);
	static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf);

	static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf)
    {
	    uint8_t* uptr = (uint8_t *)cbuf->buff;
	    CNET_MEMSET(uptr, APX);
	    return uptr;
    }

	static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, size_t len)
    {
	    cbuf->size = len;
    }
#else
#error "cnet.h: platform error."
#endif

static inline void *CNET_CREATE_LAYER(uint8_t *pkt, int *offset, int size)
{
	void *data = (void *)(pkt + *offset);
	*offset += size;

	return data;
}

#define CNET_EXTRA_LAYER(structure, pkt, offset) ({ \
    volatile int *_tmp_offset = (offset); \
    volatile uint8_t *_tmp_pkt = (pkt); \
	size_t pkt_size = sizeof(structure); \
    \
    ((_tmp_pkt == NULL || _tmp_offset == NULL) ? \
        (printf("CNET COMPILING/RUNTIME Error CNET_EXTRA_LAYER() macros\n"), (structure *)NULL) : \
        ((structure *)CNET_CREATE_LAYER(pkt, offset, pkt_size))); \
})

/*
struct cnet_buffer buff;
CNET_MEMSET(&buff, sizeof(buff));

int offset = 0;
uint8_t *pkt = CNET_PACKET_UPTR(&buff);

struct cnet_ip *ip = CNET_EXTRA_LAYER(struct cnet_ip, pkt, &offset);
struct cnet_tcp *tcp = CNET_EXTRA_LAYER(struct cnet_tcp, pkt, &offset);
*/

static inline int IP_IHL_CODE(int data) {
	return data*4;
}

static inline int IP_IHL_DECODE(int data) {
	return data/4;
}

static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate)
{
	if (state==0) {state=1;}
	*xorstate = state;
}

static inline void CNET_IPV6_VTC(void *ipv6, struct cnet_dipv6 *dipv6)
{
	struct cnet_ipv6 *cipv6 = (struct cnet_ipv6 *)ipv6;
	uint32_t nvtc = (uint32_t)(dipv6->version & 0x0F) << 28 | (uint32_t)(dipv6->tc & 0xFF) << 20 | (uint32_t)(dipv6->flow & 0xFFFFF);
	cipv6->vtc = nvtc;
}

static inline void CNET_ICMPV6_RSO(void *icmpv6, struct cnet_dicmpv6 *dicmpv6)
{
	struct cnet_icmpv6 *cicmpv6 = (struct cnet_icmpv6 *)icmpv6;
	uint32_t nrso = ((uint32_t)(dicmpv6->r & 0x1) << 2 | (uint32_t)(dicmpv6->s & 0x1) << 1 | (uint32_t)(dicmpv6->o & 0x1)) << 29;
	cicmpv6->CNET_ICMPV6_MACROS_NA.rso = nrso;
}

static inline void CNET_VLAN_VDP(void *vlan, struct cnet_dvlan *dvlan)
{
	struct cnet_vlan *cvlan = (struct cnet_vlan *)vlan;
	uint16_t nvdp = (uint16_t)(dvlan->p & 0x7) << 13 | (uint16_t)(dvlan->d & 0x1) << 12 | (uint16_t)(dvlan->v & 0x0FFF);
	cvlan->vdp = nvdp;
}

static inline void CNET_DIRECT_SWAP16(uint8_t *uptr, size_t id)
{
	uint8_t tmp = uptr[id];
	uptr[id] = uptr[id+1];
	uptr[id+1] = tmp;
}

static inline void CNET_DIRECT_SWAP32(uint8_t *uptr, size_t id)
{
	uint8_t tmp_i0 = uptr[id];
	uint8_t tmp_i1 = uptr[id+1];

	uptr[id] = uptr[id+2];
	uptr[id+2] = tmp_i0;
	uptr[id+1] = uptr[id+3];
	uptr[id+3] = tmp_i1;
}

static inline void CNET_MEMSET(void *bdata, size_t nlen)
{
	uint8_t *bytes = (uint8_t *)bdata;
	for (size_t i = 0; i < nlen; i++) {bytes[i]=0;}
}

static inline void CNET_RAND_IP(void *nip)
{
	struct cnet_ip_bytes *cip = (struct cnet_ip_bytes *)nip;

	for (int i = 0; i < 4; i++) {cip->bytes.b8its[i]=(uint8_t)(CNET_RAND(0, 255));}
}

static inline void CNET_RAND_MAC(void *nmac)
{
	struct cnet_ether_bytes *ceth = (struct cnet_ether_bytes *)nmac;

	for (int i = 0; i < 6; i++) {ceth->bytes[i]=(uint8_t)(CNET_RAND(0, 255));}
 ceth->bytes[0] = (ceth->bytes[0] & 0xFE) | 0x02;
}

static inline void CNET_SET_DMAC(void *eth, void *dbytes)
{
	struct cnet_ether *ether = (struct cnet_ether *)eth;
	struct cnet_ether_bytes *ether_b = (struct cnet_ether_bytes *)dbytes;

	for (int i = 0; i < 6; i++)
	{
		ether->dst[i] = ether_b->bytes[i];
	}
}

static inline void CNET_SET_SMAC(void *eth, void *sbytes)
{
	struct cnet_ether *ether = (struct cnet_ether *)eth;
	struct cnet_ether_bytes *ether_b = (struct cnet_ether_bytes *)sbytes;

	for (int i = 0; i < 6; i++)
	{
		ether->src[i] = ether_b->bytes[i];
	}
}

static inline uint16_t CNET_BIG16(uint16_t bdata)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	return (uint16_t)(bdata & 0x00FF) << 8 | (bdata & 0xFF00) >> 8;
#else
    return bdata;
#endif
}

static inline uint16_t CNET_LITTLE16(uint16_t bdata)
{
#if __BYTE_ORDER == __BIG_ENDIAN
    return (uint16_t)(bdata & 0x00FF) << 8 | (bdata & 0xFF00) >> 8;
#else
    return bdata;
#endif
}

static inline uint32_t CNET_BIG32(uint32_t bdata)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	return (uint32_t)(bdata & 0x000000FF) << 24 | (bdata & 0x0000FF00) << 8 | (bdata & 0x00FF0000) >> 8 | (bdata & 0xFF000000) >> 24;
#else
    return bdata;
#endif
}

static inline uint32_t CNET_LITTLE32(uint32_t bdata)
{
#if __BYTE_ORDER == __BIG_ENDIAN
    return (uint32_t)(bdata & 0x000000FF) << 24 | (bdata & 0x0000FF00) << 8 | (bdata & 0x00FF0000) >> 8 | (bdata & 0xFF000000) >> 24;
#else
    return bdata;
#endif
}

#ifdef __cplusplus
}
#endif

#endif