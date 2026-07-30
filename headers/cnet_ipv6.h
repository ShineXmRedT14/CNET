#ifndef CNET_IPV6_H
#define CNET_IPV6_H

#include <stdint.h>
#include <endian.h>

#define CNET_IPV6_ICMP 0x01
#define CNET_IPV6_TCP 0x06
#define CNET_IPV6_UDP 0x11
#define CNET_IPV6_ICMPV6 0x3A

#define CNET_IPV6_PAD1 0
#define CNET_IPV6_PADN 1

struct cnet_ipv6_ext
{
	uint8_t proto;
	uint8_t len;
}__attribute__((packed));

struct cnet_ipv6_hbh
{
	uint8_t proto;
	uint8_t len;
}__attribute__((packed));

struct cnet_ipv6_dst
{
	uint8_t proto;
	uint8_t len;
}__attribute__((packed));

struct cnet_ipv6_route
{
	uint8_t proto;
	uint8_t len;
	uint8_t type;
	uint8_t sleft;
}__attribute__((packed));

struct cnet_ipv6_route0
{
	uint8_t proto;
	uint8_t len;
	uint8_t type;
	uint8_t sleft;
	uint8_t res;
	uint8_t smap[3];
}__attribute__((packed));

struct cnet_ipv6_frag
{
	uint8_t proto;
	uint8_t res;
	uint16_t off;
	uint32_t id;
}__attribute__((packed));

struct cnet_ipv6_opt
{
	uint8_t type;
	uint8_t len;
}__attribute__((packed));

struct cnet_ipv6_opt_jumbo
{
	uint8_t type;
	uint8_t len;
	uint8_t jumbo[4];
}__attribute__((packed));

struct cnet_ipv6_opt_nsap
{
	uint8_t type;
	uint8_t len;
	uint8_t lsrc;
	uint8_t ldst;
}__attribute__((packed));

struct cnet_ipv6_opt_tunnel
{
	uint8_t type;
	uint8_t len;
	uint8_t limit;	
}__attribute__((packed));

struct cnet_ipv6_opt_router
{
	uint8_t type;
	uint8_t len;
	uint8_t value[2];
}__attribute__((packed));

struct cnet_ipv6
{
	uint32_t vtc;
	uint16_t plen;
	uint8_t proto;
	uint8_t ttl;
	uint8_t src[16];
	uint8_t dst[16];
}__attribute__((packed));

#endif