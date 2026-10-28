#ifndef CNET_BUFFER_H
#define CNET_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <pcap.h>

#define APY 64
#define APX 2048

struct cnet_buffer
{
	uint8_t buff[APY][APX];
	size_t size[APY];
	int pos;

	pcap_send_queue *buffer;
};

#endif