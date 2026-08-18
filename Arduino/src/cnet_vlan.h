#ifndef CNET_VLAN_H
#define CNET_VLAN_H

#include <stdint.h>
#include <endian.h>

#define CNET_VLAN_ID_Q 0x8100
#define CNET_VLAN_ID_AD 0x88A8

#define CNET_VLAN_PCP_BK 0x0001
#define CNET_VLAN_PCP_BE 0x0000
#define CNET_VLAN_PCP_EE 0x0002
#define CNET_VLAN_PCP_CA 0x0003
#define CNET_VLAN_PCP_VI 0x0004
#define CNET_VLAN_PCP_VO 0x0005
#define CNET_VLAN_PCP_IC 0x0006
#define CNET_VLAN_PCP_NC 0x0007

struct cnet_vlan
{
	uint16_t id;
	uint16_t vdp;
    uint16_t ether;
}__attribute__((packed));

#endif