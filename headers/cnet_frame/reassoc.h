#ifndef CNET_FRAME_REASSOC_H
#define CNET_FRAME_REASSOC_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_reassoc_req
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t info;
	uint16_t interval;
	uint8_t ap[6];
}__attribute__((packed));

struct cnet_frame_reassoc_resp
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t info;
	uint16_t code;
	uint16_t id;
}__attribute__((packed));

#endif