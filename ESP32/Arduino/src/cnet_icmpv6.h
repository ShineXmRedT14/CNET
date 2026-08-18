#ifndef CNET_ICMPV6_H
#define CNET_ICMPV6_H

#include <stdint.h>
#include <endian.h>

#define CNET_ICMPV6_RA_MANAGED 0x80
#define CNET_ICMPV6_RA_OTHER 0x40
#define CNET_ICMPV6_RA_AGENT 0x20
#define CNET_ICMPV6_RA_PRF 0x18
#define CNET_ICMPV6_RA_PROXY 0x04
#define CNET_ICMPV6_RA_PRF_HIGH (0x1 << 3)
#define CNET_ICMPV6_RA_PRF_MEDIUM (0x0 << 3)
#define CNET_ICMPV6_RA_PRF_LOW (0x3 << 3)
#define CNET_ICMPV6_RA_PRF_RESERVE (0x2 << 3)
#define CNET_ICMPV6_ECHO_REQ 0x80
#define CNET_ICMPV6_ECHO_REP 0x81
#define CNET_ICMPV6_RA 0x86
#define CNET_ICMPV6_NA 0x88
#define CNET_ICMPV6_MTU 0x02

struct cnet_icmpv6_echo
{
	uint16_t id;
	uint16_t seq;
};

struct cnet_icmpv6_ra
{
	uint8_t ttl;
	uint8_t flags;
	uint16_t ltime;
};

struct cnet_icmpv6_na
{
	uint32_t rso;
};

struct cnet_icmpv6_mtu
{
	uint32_t mtu;
};

struct cnet_icmpv6
{
	uint8_t type;
	uint8_t code;
	uint16_t sum;
	
	union
	{
		union
		{
			uint32_t raw32;
			uint16_t raw16[2];
			uint8_t raw8[4];
		} raw;
		
		struct cnet_icmpv6_echo echo;
		struct cnet_icmpv6_ra ra;
		struct cnet_icmpv6_na na;
		struct cnet_icmpv6_mtu mtu;
	} icmp;
}__attribute__((packed));

#define CNET_ICMPV6_MACROS_RAW icmp.raw
#define CNET_ICMPV6_MACROS_ECHO icmp.echo
#define CNET_ICMPV6_MACROS_RA icmp.ra
#define CNET_ICMPV6_MACROS_NA icmp.na
#define CNET_ICMPV6_MACROS_MTU icmp.mtu

#endif