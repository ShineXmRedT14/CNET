#ifndef CNET_FRAME_CTLW_H
#define CNET_FRAME_CTLW_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_ctlw
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint16_t frame;
	uint8_t ht[4];
}__attribute__((packed));

#endif