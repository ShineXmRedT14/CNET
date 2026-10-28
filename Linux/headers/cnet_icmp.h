#ifndef CNET_ICMP_H
#define CNET_ICMP_H

#include <stdint.h>

#define CNET_ICMP_EREPLY 0x00
#define CNET_ICMP_UNREACH 0x03
#define CNET_ICMP_QUENCH 0x04
#define CNET_ICMP_ECHO 0x08
#define CNET_ICMP_EXCEEDED 0x0B
#define CNET_ICMP_PROB 0x0C
#define CNET_ICMP_TIMESTAMP 0x0D
#define CNET_ICMP_TIMEREPLY 0x0E
#define CNET_ICMP_IREQ 0x0F
#define CNET_ICMP_IREP 0x10
#define CNET_ICMP_ADDR 0x11
#define CNET_ICMP_ADDRREPLY 0x12

#define CNET_ICMP_CODE_NET_UNREACH 0x00
#define CNET_ICMP_CODE_HOST_UNREACH 0x01
#define CNET_ICMP_CODE_PROTOCOL_UNREACH 0x02
#define CNET_ICMP_CODE_PORT_UNREACH 0x03
#define CNET_ICMP_CODE_FRAG_NEEDED 0x04
#define CNET_ICMP_CODE_SOURCE_ROUTE_FAILED 0x05
#define CNET_ICMP_CODE_NET_UNKNOWN 0x06
#define CNET_ICMP_CODE_HOST_UNKNOWN 0x07
#define CNET_ICMP_CODE_HOST_ISOLATED 0x08
#define CNET_ICMP_CODE_NET_PROHIBITED 0x09
#define CNET_ICMP_CODE_HOST_PROHIBITED 0x0A
#define CNET_ICMP_CODE_NET_UNREACH_TOS 0x0B
#define CNET_ICMP_CODE_HOST_UNREACH_TOS 0x0C
#define CNET_ICMP_CODE_FILTER_PROHIBITED 0x0D
#define CNET_ICMP_CODE_HOST_PRECEDENCE 0x0E
#define CNET_ICMP_CODE_PRECEDENCE_CUTOFF 0x0F
#define CNET_ICMP_CODE_REDIRECT_NET 0x00
#define CNET_ICMP_CODE_REDIRECT_HOST 0x01
#define CNET_ICMP_CODE_REDIRECT_TOS_NET 0x02
#define CNET_ICMP_CODE_REDIRECT_TOS_HOST 0x03
#define CNET_ICMP_CODE_TTL_EXPIRED 0x00
#define CNET_ICMP_CODE_FRAG_REASSEMBLY 0x01
#define CNET_ICMP_CODE_POINTER_ERROR 0x00
#define CNET_ICMP_CODE_MISSING_OPTION 0x01
#define CNET_ICMP_CODE_BAD_LENGTH 0x02

struct cnet_icmp_echo
{
	uint16_t id;
	uint16_t seq;
};

struct cnet_icmp_mtu
{
	uint16_t unused;
	uint16_t mtu;
};

struct cnet_icmp_gate
{
	uint32_t gate;
};

struct cnet_icmp
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
		
		struct cnet_icmp_echo echo;
		struct cnet_icmp_gate gate;
		struct cnet_icmp_mtu mtu;
	} icmp;
}__attribute__((packed));

#define CNET_ICMP_MACROS_RAW icmp.raw
#define CNET_ICMP_MACROS_ECHO icmp.echo
#define CNET_ICMP_MACROS_GATE icmp.gate
#define CNET_ICMP_MACROS_MTU icmp.mtu

#endif