#ifndef CNET_FRAME_RTS_H
#define CNET_FRAME_RTS_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_rts
{
	struct cnet_frame_types cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint8_t ta[6];
}__attribute__((packed));

#endif