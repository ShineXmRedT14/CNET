#ifndef CNET_TCP_H
#define CNET_TCP_H

#include <stdint.h>
#include <sys/types.h>
#include <endian.h>

#define CNET_TCP_FIN 0x0001
#define CNET_TCP_SYN 0x0002
#define CNET_TCP_RST 0x0004
#define CNET_TCP_PSH 0x0008
#define CNET_TCP_ACK 0x0010
#define CNET_TCP_URG 0x0020

#define CNET_TCP_DOFF 0x0005
#define CNET_TCP_RES1 0x0000
#define CNET_TCP_RES2 0x0000
#define CNET_TCP_WIN 0xFFFF

enum cnet_state_ca
{
	CNET_TCP_OPEN = 0,
	CNET_TCP_DISC = 1,
	CNET_TCP_CWR = 2,
	CNET_TCP_RECOV = 3,
	CNET_TCP_LOSS = 4
};

struct cnet_tcp_info
{
	uint8_t state;
	uint8_t cnet_state_ca;
	uint8_t probes;
	uint8_t off;
	uint8_t options;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t sscale:4;
    uint8_t rscale:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t rscale:4;
    uint8_t sscale:4;
#else
#error "Big/Little Endians bytes-error"
#endif
    uint32_t rto;
    uint32_t ato;
    uint32_t smss;
    uint32_t rmss;
    uint32_t unack;
    uint32_t sack;
    uint32_t lost;
    uint32_t ret;
    uint32_t ackets;
    uint32_t lds;
    uint32_t las;
    uint32_t ldr;
    uint32_t lar;
    uint32_t pmtu;
    uint32_t rth;
    uint32_t rtt;
    uint32_t rttv;
    uint32_t sth;
    uint32_t cwnd;
    uint32_t advmss;
    uint32_t reorder;
    uint32_t rrtt;
    uint32_t rspace;
    uint32_t tret;
};

struct cnet_tcp
{
	uint16_t src;
	uint16_t dst;
	uint32_t seq;
	uint32_t aseq;

#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t res1:4;
    uint16_t doff:4;
    uint16_t flags:6;
    uint16_t res2:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint16_t doff:4;
    uint16_t res1:4;
    uint16_t res2:2;
    uint16_t flags:6;
#else
#error "Big/Little Endians bytes-error"
#endif
    uint16_t win;
    uint16_t sum;
    uint16_t uptr;
}__attribute__((packed));

#endif