#ifnder CNET_FRAME_ACTION_H
#define CNET_FRAME_ACTION_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_action
{
	struct cnet_frame_hdr cnet_hdr;
	uint8_t category;
	uint8_t action;
}__attribute__((packed));

struct cnet_frame_action_na
{
	struct cnet_frame_hdr cnet_hdr;
	uint8_t category;
	uint8_t action;
}__attribute__((packed));

#endif