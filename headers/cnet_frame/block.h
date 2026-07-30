#ifndef CNET_FRAME_BLOCK_H
#define CNET_FRAME_BLOCK_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_block_ackr
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint8_t ta[6];
	uint16_t ctl;
	uint16_t info;
}__attribute__((packed));

struct cnet_frame_block_ack
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint8_t ta[6];
	uint16_t ctl;
	uint16_t info;
	
	union
	{
		uint8_t b8it[128];
		uint16_t b16it[64];
		uint32_t b32it[32];
		uint64_t b64it[16];
	} map;
}__attribute__((packed));

#define CNET_FRAME_BLOCK_MACROS_BIT8 map.b8it
#define CNET_FRAME_BLOCK_MACROS_BIT16 map.b16it
#define CNET_FRAME_BLOCK_MACROS_BIT32 map.b32it
#define CNET_FRAME_BLOCK_MACROS_BIT64 map.b64it

#endif