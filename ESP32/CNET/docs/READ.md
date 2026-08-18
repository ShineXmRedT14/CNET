# Deleted

Deleted functions and files for esp32

## Functions

| Function | Refactored or Deleted |
|---|---|
| `CNET_SOCK` | Deleted |
| `CNET_SOCK_ADDR_IN` | Deleted |
| `CNET_SOCK_ADDR_LL` | Deleted |
| `CNET_BUFFER_CLONE` | Deleted |
| `CNET_GET_IF` | Deleted |
| `CNET_GET_IP` | Deleted |
| `CNET_GET_MAC` | Deleted |
| `CNET_PACKET_UPTR` | Refactored |
| `CNET_SIZE_SET` | Refactored |
| `CNET_BUFFER_INIT_ALL` | Deleted |
| `CNET_BUFFER_INIT_SET` | Deleted |
| `CNET_BURST` | Deleted |

## Files

| File | Refactored or Deleted |
|---|---|
| `cnet_route` | Deleted |
| `cnet_buffer.h` | Refactored |

## Buffer Refactored

```c
#include <sys/types.h>
#include <stdint.h>

#define APX 2048

struct cnet_buffer
{
    uint8_t buff[APX]
    size_t bytesize;
}__attribute__((packed));
```

## Functions Refactored

CNET_PACKET_UPTR
```c
static inline uint8_t* CNET_PACKET_UPTR(struct cnet_buffer *cbuf)
{
	uint8_t* uptr = (uint8_t *)cbuf->buff;
	CNET_MEMSET(uptr, APX);
	return uptr;
}
```
---
CNET_SIZE_SET
```c
static inline void CNET_SIZE_SET(struct cnet_buffer *cbuf, size_t len)
{
	cbuf->bytesize = len;
}
```

## Bugs
If you find bugs, please issue this on the github