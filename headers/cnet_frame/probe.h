#ifndef CNET_FRAME_PROBE_H
#define CNET_FRAME_PROBE_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

#define CNET_FRAME_PROBE_BODY_EID 0x00
#define CNET_FRAME_PROBE_BODY_LEN 0x00
#define CNET_FRAME_PROBE_RATES_EID 0x01
#define CNET_FRAME_PROBE_RATES_LEN 0x08
#define CNET_FRAME_PROBE_RATES_MB1 0x82
#define CNET_FRAME_PROBE_RATES_MB2 0x84
#define CNET_FRAME_PROBE_RATES_MB5_5 0x8B
#define CNET_FRAME_PROBE_RATES_MB11 0x96
#define CNET_FRAME_PROBE_RATES_MB6 0x0C
#define CNET_FRAME_PROBE_RATES_MB9 0x12
#define CNET_FRAME_PROBE_RATES_MB12 0x18
#define CNET_FRAME_PROBE_RATES_MB18 0x24

struct cnet_frame_probe_body
{
    uint8_t eid; // CNET_FRAME_PROBE_BODY_EID
    uint8_t len; // CNET_FRAME_PROBE_BODY_LEN
}__attribute__((packed));

struct cnet_frame_probe_rates
{
    uint8_t eid; // CNET_FRAME_PROBE_RATES_EID
    uint8_t len; // CNET_FRAME_PROBE_RATES_LEN
    uint8_t rates[8]; // CNET_FRAME_PROBE_RATES_MB
}__attribute__((packed));

struct cnet_frame_probe_req
{
	struct cnet_frame_hdr cnet_hdr;
}__attribute__((packed));

struct cnet_frame_probe_resp
{
	struct cnet_frame_hdr cnet_hdr;
	uint64_t tstamp;
	uint16_t interval;
	uint16_t info
}__attribute__((packed));

#endif
