#ifndef CNET_IP_H
#define CNET_IP_H

#include <stdint.h>
#include <sys/types.h>
#include <endian.h>

#define CNET_IP_ICMP 0x01
#define CNET_IP_IGMP 0x02
#define CNET_IP_TCP 0x06
#define CNET_IP_UDP 0x11
#define CNET_IP_RAW 0xFF

#define CNET_IP_VERSION 0x04
#define CNET_IP_TYPE 0x00
#define CNET_IP_TTL 0x40

#define CNET_IP_TSP_ONLY 0x00
#define CNET_IP_TSP_ADDR 0x01
#define CNET_IP_TSP_DEFINED 0x03

struct cnet_ip_bytes
{
	union
	{
		uint8_t b8its[4];
		uint32_t b32its;
	} bytes;
};

struct cnet_ip_timestamp
{
	uint8_t len;
	uint8_t ptr;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t flags:4;
    uint8_t flow:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t flow:4;
    uint8_t flags:4;
#else
#error "Big/Little Endians bytes-error"
#endif
    uint32_t data[9];
}__attribute__((packed));

struct cnet_ip
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t ihl:4;
    uint8_t version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t version:4;
    uint8_t ihl:4;
#else
#error "Big/Little Endians bytes-error"
#endif
    uint8_t type;
    uint16_t len;
    uint16_t id;
    uint16_t frag;
    uint8_t ttl;
    uint8_t proto;
    uint16_t sum;
    
    union
    {
    	uint8_t s8rc[4];
    	uint32_t s32rc;
    } src;
    union
    {
    	uint8_t d8st[4];
    	uint32_t d32st;
    } dst;
}__attribute__((packed));

#define CNET_IP_MACROS_SRC8 src.s8rc
#define CNET_IP_MACROS_SRC32 src.s32rc
#define CNET_IP_MACROS_DST8 dst.d8st
#define CNET_IP_MACROS_DST32 dst.d32st
#define CNET_IP_MACROS_BYTES8 bytes.b8its
#define CNET_IP_MACROS_BYTES32 bytes.b32its

#endif