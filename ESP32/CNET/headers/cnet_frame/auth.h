#ifndef CNET_FRAME_AUTH_H
#define CNET_FRAME_AUTH_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_auth
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t auth;
	uint16_t seq;
	uint16_t code;
}__attribute__((packed));

#endif