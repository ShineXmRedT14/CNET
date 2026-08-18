#ifndef CNET_FRAME_ASSOC_H
#define CNET_FRAME_ASSOC_H

#include <stdio.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_assoc_req
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t info;
	uint16_t interval;
    uint8_t fcs[4];
}__attribute__((packed));

struct cnet_frame_assoc_resp
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t info;
	uint16_t code;
	uint16_t id;
    uint8_t fcs[4];
}__attribute__((packed));

#endif
