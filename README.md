# CNET

CNET is a small C library of macros and inline helpers for crafting and
sending raw network packets — sockets, checksums, byte-order swaps,
address parsing, and a multi-packet send buffer — without repeating the
usual raw-socket boilerplate in every project.

This directory contains the full reference documentation:

- [`headers.md`](headers.md) — every public header and what it's for
- [`functions/`](functions/) — one page per `CNET_*` function
- [`defines/`](defines/) — every `#define` macro, grouped by header, with values and meanings
- [`structs/`](structs/) — every `struct` / `enum`, grouped by header, with field-by-field explanations
- [`examples/`](examples/) — worked examples

## Quick start

A minimal raw IP socket send, using only what's documented here:

```c
#include <cnet.h>

int main(void)
{
    // 1. Open a raw socket at the IP layer, for TCP
    int sockfd = CNET_SOCK(CNET_LAYER_IP, CNET_IP_TCP);

    // 2. Build the packet
    struct cnet_ip ip = {0};
    // fill in ip fields (version, ttl, protocol, addresses)
    ip.sum = CNET_L3_CSUM(&ip, sizeof(ip));

    struct cnet_tcp tcp = {0};
    tcp.src = CNET_BIG16(35420);
    tcp.dst = CNET_BIG16(80);
    tcp.seq = CNET_BIG32(123);
    // again, what you do with ip
    tcp.sum = CNET_L4_CSUM(&ip, &tcp, sizeof(tcp));

    // 3. Get a destination address and send
    struct sockaddr_in addr = CNET_SOCK_ADDR_IN(&ip);
    sendto(sockfd, &tcp, sizeof(tcp), 0,
           (struct sockaddr *)&addr, sizeof(addr));

    return 0;
}
```

For sending many packets at once (up to 64 per call), see
[`CNET_BUFFER_INIT_ALL`](functions/CNET_BUFFER_INIT_ALL.md),
[`CNET_BUFFER_INIT_SET`](functions/CNET_BUFFER_INIT_SET.md), and
[`CNET_BURST`](functions/CNET_BURST.md).

## Function reference

### Sockets & addressing

- [`CNET_SOCK`](functions/CNET_SOCK.md) - Creating socket 2 or 3 layer
- [`CNET_SOCK_ADDR_IN`](functions/CNET_SOCK_ADDR_IN.md) — build a `sockaddr_in` for Layer 3 sending
- [`CNET_SOCK_ADDR_LL`](functions/CNET_SOCK_ADDR_LL.md) — build a `sockaddr_ll` for Layer 2 sending
- [`CNET_GET_IF`](functions/CNET_GET_IF.md) — find the first usable network interface
- [`CNET_GET_IP`](functions/CNET_GET_IP.md) — resolve the local IP address
- [`CNET_GET_MAC`](functions/CNET_GET_MAC.md) — resolve the local MAC address

> `CNET_SOCK_ADDR` (singular, one function for both layers) no longer exists in `cnet.h` — it was split into the two functions above. See [`functions/CNET_SOCK_ADDR.md`](functions/CNET_SOCK_ADDR.md) if you have old code still calling it.

### Checksums

- [`CNET_L3_CSUM`](functions/CNET_L3_CSUM.md) — checksum for L3 headers (IP)
- [`CNET_L4_CSUM`](functions/CNET_L4_CSUM.md) — checksum for L4 headers (TCP/UDP/ICMP)
- [`CNET_SCTP_CSUM`](functions/CNET_SCTP_CSUM.md) — checksum for SCTP headers

### Byte order

- [`CNET_BIG16`](functions/CNET_BIG16.md) / [`CNET_BIG32`](functions/CNET_BIG32.md) — convert to Big Endian
- [`CNET_LITTLE16`](functions/CNET_LITTLE16.md) / [`CNET_LITTLE32`](functions/CNET_LITTLE32.md) — convert to Little Endian
- [`CNET_DIRECT_SWAP16`](functions/CNET_DIRECT_SWAP16.md) / [`CNET_DIRECT_SWAP32`](functions/CNET_DIRECT_SWAP32.md) — in-place swap on raw byte buffers

### Addresses & random values

- [`CNET_IP_BYTES`](functions/CNET_IP_BYTES.md) — parse a string IP into bytes
- [`CNET_MAC_BYTES`](functions/CNET_MAC_BYTES.md) — parse a string MAC into bytes
- [`CNET_RAND`](functions/CNET_RAND.md) — thread-safe random integer in a range
- [`CNET_RAND_IP`](functions/CNET_RAND_IP.md) — generate a random IP address
- [`CNET_RAND_MAC`](functions/CNET_RAND_MAC.md) — generate a random MAC address
- [`CNET_SET_SMAC`](functions/CNET_SET_SMAC.md) / [`CNET_SET_DMAC`](functions/CNET_SET_DMAC.md) — copy a MAC into an Ethernet header

### Packet buffers & sending

- [`CNET_BUFFER_INIT_ALL`](functions/CNET_BUFFER_INIT_ALL.md) — configure a range of buffer slots
- [`CNET_BUFFER_INIT_SET`](functions/CNET_BUFFER_INIT_SET.md) — configure a single buffer slot
- [`CNET_SIZE_SET`](functions/CNET_SIZE_SET.md) — set a packet's size in the buffer
- [`CNET_PACKET_UPTR`](functions/CNET_PACKET_UPTR.md) — get a raw pointer into a buffer slot
- [`CNET_BUFFER_CLONE`](functions/CNET_BUFFER_CLONE.md) — duplicate a buffer slot
- [`CNET_BURST`](functions/CNET_BURST.md) — send up to 64 buffered packets

### Bit-packing helpers

Each of these takes a small "decomposed" struct of plain integers and packs it into the single bit-packed field a protocol header actually stores on the wire.

- [`CNET_IPV6_VTC`](functions/CNET_IPV6_VTC.md) — pack IPv6's Version/Traffic Class/Flow Label field, from a `struct cnet_dipv6`
- [`CNET_ICMPV6_RSO`](functions/CNET_ICMPV6_RSO.md) — pack a Neighbor Advertisement's Router/Solicited/Override flags (`struct cnet_icmpv6_na.rso`), from a `struct cnet_dicmpv6`
- [`CNET_VLAN_VDP`](functions/CNET_VLAN_VDP.md) — pack an 802.1Q tag's Priority/Drop-Eligible/VLAN-ID field, from a `struct cnet_dvlan`

### Misc

- [`CNET_MEMSET`](functions/CNET_MEMSET.md) — zero-fill a struct
- [`unuseable.md`](functions/unuseable.md) — `cnet_xorrand()`, internal helper for `CNET_RAND`

License (MIT)
