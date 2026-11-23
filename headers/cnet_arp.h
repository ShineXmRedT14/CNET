#ifndef CNET_ARP_H
#define CNET_ARP_H

#include <stdint.h>

#define CNET_ARP_REQ 0x01
#define CNET_ARP_REP 0x02
#define CNET_ARP_RREQ 0x03
#define CNET_ARP_RREP 0x04
#define CNET_ARP_IREQ 0x08
#define CNET_ARP_IREP 0x09
#define CNET_ARP_NAK 0x0A

struct cnet_arp
{
	uint16_t hwtype;
	uint16_t ptype;
	uint8_t hwlen;
	uint8_t plen;
	uint16_t code;
	uint8_t hwsrc[6];
	uint8_t psrc[4];
	uint8_t hwdst[6];
	uint8_t pdst[4];
}__attribute__((packed));

#endif