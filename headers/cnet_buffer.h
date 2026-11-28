#ifndef CNET_BUFFER_H
#define CNET_BUFFER_H

#define APY 64
#define APX 2048

#include <stdint.h>
#include <stddef.h>
#include <cnet_platform.h>

#if defined(CNET_PLATFORM_LINUX)
    #include <sys/socket.h>
    #include <cnet_helper.h>

    struct cnet_ring
    {
	    int sockfd;
	    uint8_t *ring_ptr;
	    size_t ring_size;
	    uint32_t frame_size;
	    uint32_t frame_count;
	    uint32_t position;

	    cnet_errno_t code;
    };

    struct cnet_buffer
    {
	    uint8_t buff[APY][APX];
	    size_t size[APY];
	    struct iovec iov[APY];
	    struct mmsghdr msg[APY];
    };

#elif defined(CNET_PLATFORM_ESP32)
    struct cnet_buffer
    {
        uint8_t buff[APX];
        size_t size;
    }__attribute__((packed));

#elif defined(CNET_PLATFORM_WINDOWS)
    #include <pcap/pcap.h>

    struct cnet_buffer
    {
	    uint8_t buff[APY][APX];
	    size_t size[APY];
	    int pos;

	    pcap_send_queue *buffer;
    };

#else
#error "cnet_buffer.h: platform definition error."
#endif
#endif
