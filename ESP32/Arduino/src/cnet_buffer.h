#ifndef CNET_BUFFER_H
#define CNET_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#define APX 2048

struct cnet_buffer
{
    uint8_t buff[APX];
    size_t bytesize;
}__attribute__((packed));

#endif
