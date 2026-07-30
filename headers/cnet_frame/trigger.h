#ifndef CNET_FRAME_TRIGGER_H
#define CNET_FRAME_TRIGGER_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_trigger
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint8_t ta[6];
	uint64_t info;
}__attribute__((packed));

#endif