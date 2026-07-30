# Structs — `cnet_arp.h`

## `struct cnet_arp`

The ARP packet (and RARP, using the same layout with `CNET_ARP_RREQ`/`CNET_ARP_RREP`).

```c
struct cnet_arp
{
    uint16_t hwtype;
    uint16_t ptype;
    uint8_t hwlen;
    uint8_t plen;
    uint16_t code;
    uint8_t hwsrc[6];
    uint8_t psrc[4];
    uint8_t hwdst[6];
    uint8_t pdst[4];
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `hwtype` | `uint16_t` | Hardware type — `1` for Ethernet, big-endian |
| `ptype` | `uint16_t` | Protocol type — `CNET_ETH_IP` (0x0800) for IPv4, big-endian |
| `hwlen` | `uint8_t` | Hardware address length in bytes — `6` for Ethernet |
| `plen` | `uint8_t` | Protocol address length in bytes — `4` for IPv4 |
| `code` | `uint16_t` | Operation — one of `CNET_ARP_*` (REQ/REP/RREQ/RREP/IREQ/IREP/NAK), big-endian |
| `hwsrc` | `uint8_t[6]` | Sender hardware (MAC) address |
| `psrc` | `uint8_t[4]` | Sender protocol (IPv4) address |
| `hwdst` | `uint8_t[6]` | Target hardware (MAC) address |
| `pdst` | `uint8_t[4]` | Target protocol (IPv4) address |

## See also

- Defines: [`docs/defines/cnet_arp.md`](../defines/cnet_arp.md)
