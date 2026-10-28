#ifndef CNET_H
#define CNET_H

#include <pcap/pcap.h>

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include <cnet_endian.h>
#include <cnet_ip.h>
#include <cnet_ether.h>
#include <cnet_buffer.h>
#include <cnet_route.h>

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

typedef enum
{
	CNET_N_FND = -1,
	CNET_FND = 0,
} cnet_errno_t;

int CNET_RAND(int min, int max);
void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2);
static inline void CNET_RAND_IP(void *nip);
cnet_errno_t CNET_GET_CONFIGURE(struct cnet_route *route, pcap_t *socket);
static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index);
static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, size_t nlen);
void CNET_BUFFER_INIT(struct cnet_buffer *cbuf);
void CNET_BUFFER_PACKETS(struct cnet_buffer *cbuf, int ind);
pcap_t* CNET_SOCK(struct cnet_route *route);
static inline void CNET_BURST(struct cnet_buffer *cbuf, pcap_t* socket);
static inline void CNET_MEMSET(void *cstruct, size_t nlen);
static inline uint16_t CNET_BIG16(uint16_t bdata);
static inline uint16_t CNET_LITTLE16(uint16_t bdata);
static inline uint32_t CNET_BIG32(uint32_t bdata);
static inline uint32_t CNET_LITTLE32(uint32_t bdata);
uint16_t CNET_L3_CSUM(void *ndata, size_t nlen);
uint16_t CNET_L4_CSUM(void *cip, void *ndata, size_t nlen);
void CNET_IP_BYTES(void *cipb, char sip[]);
static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate);
int IP_IHL_LEN(int data);
int IP_IHL(int data);

int IP_IHL_LEN(int data) {
	return data*4;
}

int IP_IHL(int data) {
	return (20+data)/4;
}

static inline void cnet_xorrand(uint32_t state, uint32_t *xorstate)
{
	if (state==0) {state=1;}
	*xorstate = state;
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

static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf, int index)
{
	uint8_t* uptr = (uint8_t *)cbuf->buff[index];
	CNET_MEMSET(uptr, APX);
	return uptr;
}

static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, int index, size_t nlen)
{
	size_t allen = 0;

    allen = sizeof(struct cnet_ether) + sizeof(struct cnet_ip) + nlen;
	cbuf->size[index] = allen;
}

static inline void CNET_BURST(struct cnet_buffer *cbuf, pcap_t *socket)
{
	uint32_t bytes = pcap_sendqueue_transmit(socket, cbuf->buffer, 0);
	if (bytes < 1) {fprintf(stderr, "CNET/BURST-ERR");}
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
