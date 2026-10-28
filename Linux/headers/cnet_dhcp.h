#ifndef CNET_DHCP_H
#define CNET_DHCP_H

#include <stdint.h>

#define CNET_DHCP_OP_REQ 0x01
#define CNET_DHCP_OP_REP 0x02

#define CNET_DHCP_HTYPE_ETHER 0x01
#define CNET_DHCP_MAGIC_COOKIE 0x63825363
#define CNET_DHCP_BROADCAST 0x8000

#define CNET_DHCP_OP_PAD 0x00
#define CNET_DHCP_OP_MASK 0x01
#define CNET_DHCP_OP_ROUTER 0x03
#define CNET_DHCP_OP_DNS_SERV 0x06
#define CNET_DHCP_OP_HOSTNAME 0x0C
#define CNET_DHCP_OP_DOMAIN_NAME 0x0F
#define CNET_DHCP_OP_REQIP 0x32
#define CNET_DHCP_OP_LTIME 0x33
#define CNET_DHCP_OP_MTYPE 0x35
#define CNET_DHCP_OP_SERV_ID 0x36
#define CNET_DHCP_OP_PREQL 0x37
#define CNET_DHCP_OP_RENT 0x3A
#define CNET_DHCP_OP_REBT 0x3B
#define CNET_DHCP_OP_CID 0x3D
#define CNET_DHCP_OP_END 0xFF

#define CNET_DHCP_MSG_DISCOVER 0x01
#define CNET_DHCP_MSG_OFFER 0x02
#define CNET_DHCP_MSG_REQUEST 0x03
#define CNET_DHCP_MSG_DECLINE 0x04
#define CNET_DHCP_MSG_ACK 0x05
#define CNET_DHCP_MSG_NAK 0x06
#define CNET_DHCP_MSG_RELEASE 0x07
#define CNET_DHCP_MSG_INFORM 0x08

struct cnet_dhcp
{
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint32_t xid;
	uint16_t secs;
	uint16_t flags;
	
	union
	{
		uint8_t c8addr[4];
		uint32_t c32addr;
	} caddr;
	
	union
	{
		uint8_t y8addr[4];
		uint32_t y32addr;
	} yaddr;
	
	union
	{
		uint8_t n8addr[4];
		uint32_t n32addr;
	} naddr;
	
	union
	{
		uint8_t g8addr[4];
		uint32_t g32addr;
	} gaddr;
	
	uint8_t chaddr[16];
	uint8_t hostname[64];
	uint8_t file[128];
	uint32_t cookie;
}__attribute__((packed));

struct cnet_dhcp_option
{
	uint8_t code;
	uint8_t len;
}__attribute__((packed));

#define CNET_DHCP_MACROS_CADDR8 caddr.c8addr
#define CNET_DHCP_MACROS_CADDR32 caddr.c32addr
#define CNET_DHCP_MACROS_YADDR8 yaddr.y8addr
#define CNET_DHCP_MACROS_YADDR32 yaddr.y32addr
#define CNET_DHCP_MACROS_NADDR8 naddr.n8addr
#define CNET_DHCP_MACROS_NADDR32 naddr.n32addr
#define CNET_DHCP_MACROS_GADDR8 gaddr.g8addr
#define CNET_DHCP_MACROS_GADDR32 gaddr.g32addr

#endif