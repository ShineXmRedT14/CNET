#ifndef CNET_H
#define CNET_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <endian.h>
#include <time.h>
#include <errno.h>

#include <cnet_ip.h>
#include <cnet_ipv6.h>
#include <cnet_icmpv6.h>
#include <cnet_vlan.h>
#include <cnet_ether.h>
#include <cnet_sctp.h>
#include <cnet_buffer.h>
#include <cnet_route.h>

#define CNET_LAYER_ETHER 2
#define CNET_LAYER_IP 3
#define CNET_BROADCAST {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define CNET_IP_ICMP 0x01
#define CNET_IP_TCP 0x06
#define CNET_IP_UDP 0x11
#define CNET_PORT_FTP_DAT 20
#define CNET_PORT_FTP_SET 21
#define CNET_PORT_SSH 22
#define CNET_PORT_TELNET 23
#define CNET_PORT_SMTP 25
#define CNET_PORT_DNS 53
#define CNET_PORT_DHCP_IP 67
#define CNET_PORT_DHCP_CLIENT 68
#define CNET_PORT_TFTP 69
#define CNET_PORT_HTTP 80
#define CNET_PORT_POP3 110
#define CNET_PORT_NTP 123
#define CNET_PORT_IMAP 143
#define CNET_PORT_SNMP_N1 161
#define CNET_PORT_SNMP_N2 162
#define CNET_PORT_LDAP 389
#define CNET_PORT_HTTPS 443
#define CNET_PORT_SMB 445
#define CNET_PORT_SYSLOG 514
#define CNET_PORT_LDAPS 636
#define CNET_PORT_IMAPS 993
#define CNET_PORT_POP3S 995
#define CNET_PORT_MSSQL 1433
#define CNET_PORT_ORACLE 1521
#define CNET_PORT_MQTT 1883
#define CNET_PORT_MYSQL 3306
#define CNET_PORT_RDP 3389
#define CNET_PORT_SIP_N1 5060
#define CNET_PORT_SIP_N2 5061
#define CNET_PORT_PSQL 5432
#define CNET_PORT_RMQ 5672
#define CNET_PORT_VNC 5900
#define CNET_PORT_REDIS 6379
#define CNET_PORT_HTTP8 8080
#define CNET_PORT_HTTPS8 8443
#define CNET_PORT_FPM 9000
#define CNET_PORT_KAFKA 9092
#define CNET_PORT_MDB 27017

#define MEGASIZE 4
#define SIZEBUFF() (MEGASIZE*1024*1024)
#define IHL_LEN(s) (s*4)
#define IHL(s) ((20+s)/4)

struct cnet_dipv6
{
	int version;
	int tc;
	uint32_t flow;
};

struct cnet_dicmpv6
{
	int r;
	int s;
	int o;
};

struct cnet_dvlan
{
	int v;
	int d;
	int p;
};

struct cnet_ntp_dtime
{
	uint32_t sec;
	uint32_t frc;	
};

struct cnet_frame_fcs
{
    union
    {
        uint32_t fcs32;
        uint16_t fcs16[2];
        uint8_t fcs8[4];
    } fcs;
}__attribute__((packed));

#define CNET_FRAME_MACROS_FCS32 fcs.fcs32
#define CNET_FRAME_MACROS_FCS16 fcs.fcs16
#define CNET_FRAME_MACROS_FCS8 fcs.fcs8

typedef enum
{
	CNET_SCFL = 0,
	CNET_ERR_IFADDRS = -1,
	CNET_ERR_SOCKET = -2,
	CNET_ERR_IOCTL = -3,
	CNET_ERR_NIFACE = -4
} cnet_errno_t;

void CNET_FRAME_FCS(void *data, size_t len);
struct cnet_ntp_dtime CNET_NTP_TIMESTAMP();
int CNET_RAND(int min, int max);
static inline struct sockaddr_in CNET_SOCK_ADDR_IN(struct cnet_ip *cip);
struct sockaddr_ll CNET_SOCK_ADDR_LL(int sock);
static inline void CNET_IPV6_VTC(void *ipv6, struct cnet_dipv6 *dipv6);
static inline void CNET_ICMPV6_RSO(void *icmpv6, struct cnet_dicmpv6 *dicmpv6);
static inline void CNET_VLAN_VDP(void *vlan, struct cnet_dvlan *dvlan);
void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2);
static inline void CNET_DIRECT_SWAP16(uint8_t *uptr, size_t id);
static inline void CNET_DIRECT_SWAP32(uint8_t *uptr, size_t id);
void CNET_SCTP_CSUM(void *sctp, size_t len);
static inline void CNET_RAND_IP(void *nip);
static inline void CNET_RAND_MAC(void *nmac);
cnet_errno_t CNET_GET_IF(void *crt);
cnet_errno_t CNET_GET_IP(void *route);
cnet_errno_t CNET_GET_MAC(void *route);
static inline void CNET_SET_SMAC(void *eth, void *sbytes);
static inline void CNET_SET_DMAC(void *eth, void *dbytes);
static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index);
static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, size_t nlen, int flag);
void CNET_BUFFER_INIT_ALL(struct cnet_buffer *cbuf, int ind, void *addr, int layer);
void CNET_BUFFER_INIT_SET(struct cnet_buffer *cbuf, int ind, void *addr, int layer);
int CNET_SOCK(int layer, uint8_t flag);
static inline void CNET_BURST(struct cnet_buffer *cbuf, int sockfd, int hlen);
static inline void CNET_MEMSET(void *cstruct, size_t nlen);
static inline uint16_t CNET_BIG16(uint16_t bdata);
static inline uint16_t CNET_LITTLE16(uint16_t bdata);
static inline uint32_t CNET_BIG32(uint32_t bdata);
static inline uint32_t CNET_LITTLE32(uint32_t bdata);
uint16_t CNET_L3_CSUM(void *ndata, size_t nlen);
uint16_t CNET_L4_CSUM(void *cip, void *ndata, size_t nlen);
void CNET_IP_BYTES(void *cipb, char sip[]);
void CNET_MAC_BYTES(void *ceth, char mac[]);
static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate);

static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate)
{
	if (state==0) {state=1;}
	*xorstate = state;
}

static inline struct sockaddr_in  CNET_SOCK_ADDR_IN(struct cnet_ip *cip)
{
	struct sockaddr_in addr;
	CNET_MEMSET(&addr, sizeof(addr));
	addr.sin_family = AF_INET;	
	addr.sin_addr.s_addr = cip->CNET_IP_MACROS_DST32;
	return addr;
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
	uint32_t nrso = ((uint32_t)(dicmpv6->r & 0x1) << 1 | (uint32_t)(dicmpv6->s & 0x1) << 1 | (uint32_t)(dicmpv6->o & 0x1) << 1) << 29;
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
	uptr[id] = uptr[id+2];
	uptr[id+1] = uptr[id+3];
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

static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index)
{
	uint8_t* uptr = (uint8_t *)cbuf->buff[index];
	CNET_MEMSET(uptr, APX);
	return uptr;
}

static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, size_t nlen, int flag)
{
	size_t allen = 0;
	
	if (flag==2)
	{
        allen = sizeof(struct cnet_ether) + sizeof(struct cnet_ip) + nlen;
	} else {
		allen = sizeof(struct cnet_ip) + nlen;
	}
	cbuf->size[index] = allen;
}

static inline void CNET_BURST(struct cnet_buffer *cbuf, int sockfd, int hlen)
{
	int cerr = sendmmsg(sockfd, cbuf->msg, hlen, 0);
	if (cerr < 0) {fprintf(stderr, "CNET/BURST-ERR: %s (errno=%d)\n", strerror(errno), errno);}
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

#endif
