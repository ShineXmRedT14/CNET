#ifndef CNET_FRAME_DEAUTH_H
#define CNET_FRAME_DEAUTH_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_deauth
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t code;
       uint8_t fcs[4];
}__attribute__((packed));

#endif
