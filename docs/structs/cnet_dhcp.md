# Structs — `cnet_dhcp.h`

## `struct cnet_dhcp`

The fixed-size portion of a DHCP/BOOTP message. Options (tag/length/value, see `struct cnet_dhcp_option`) follow immediately after `cookie`.

```c
struct cnet_dhcp
{
    uint8_t op;
    uint8_t htype;
    uint8_t hlen;
    uint8_t hops;
    uint32_t xid;
    uint16_t secs;
    uint16_t flags;
    union { uint8_t c8addr[4]; uint32_t c32addr; } caddr;
    union { uint8_t y8addr[4]; uint32_t y32addr; } yaddr;
    union { uint8_t n8addr[4]; uint32_t n32addr; } naddr;
    union { uint8_t g8addr[4]; uint32_t g32addr; } gaddr;
    uint8_t chaddr[16];
    uint8_t hostname[64];
    uint8_t file[128];
    uint32_t cookie;
} __attribute__((packed));
```

| Field | Type | Meaning |
|---|---|---|
| `op` | `uint8_t` | `CNET_DHCP_OP_REQ` (BOOTREQUEST) or `CNET_DHCP_OP_REP` (BOOTREPLY) |
| `htype` | `uint8_t` | Hardware type — `CNET_DHCP_HTYPE_ETHER` |
| `hlen` | `uint8_t` | Hardware address length — `6` for Ethernet |
| `hops` | `uint8_t` | Relay hop count |
| `xid` | `uint32_t` | Transaction ID |
| `secs` | `uint16_t` | Seconds elapsed since client began the exchange |
| `flags` | `uint16_t` | `CNET_DHCP_BROADCAST` or 0 |
| `caddr` | union | Client IP address — `CNET_DHCP_MACROS_CADDR8`/`CADDR32` |
| `yaddr` | union | "Your" (offered client) IP address — `CNET_DHCP_MACROS_YADDR8`/`YADDR32` |
| `naddr` | union | Next server IP address — `CNET_DHCP_MACROS_NADDR8`/`NADDR32` |
| `gaddr` | union | Relay agent (gateway) IP address — `CNET_DHCP_MACROS_GADDR8`/`GADDR32` |
| `chaddr` | `uint8_t[16]` | Client hardware address (padded to 16 bytes) |
| `hostname` | `uint8_t[64]` | Optional server host name, null-padded |
| `file` | `uint8_t[128]` | Optional boot file name, null-padded |
| `cookie` | `uint32_t` | `CNET_DHCP_MAGIC_COOKIE` — marks the start of the options area |

## `struct cnet_dhcp_option`

TLV header for one DHCP option (the value bytes follow, `len` of them; `CNET_DHCP_OP_PAD` and `CNET_DHCP_OP_END` have no length/value).

| Field | Type | Meaning |
|---|---|---|
| `code` | `uint8_t` | Option code — one of `CNET_DHCP_OP_*` |
| `len` | `uint8_t` | Length of the option value in bytes |

## See also

- Defines: [`docs/defines/cnet_dhcp.md`](../defines/cnet_dhcp.md)
