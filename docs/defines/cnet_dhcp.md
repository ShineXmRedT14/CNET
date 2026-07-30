# Defines — `cnet_dhcp.h`

## Op codes (for `struct cnet_dhcp.op`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DHCP_OP_REQ` | `0x01` | BOOTREQUEST |
| `CNET_DHCP_OP_REP` | `0x02` | BOOTREPLY |

## Header constants

| Define | Value | Meaning |
|---|---|---|
| `CNET_DHCP_HTYPE_ETHER` | `0x01` | Hardware type: Ethernet, for `struct cnet_dhcp.htype` |
| `CNET_DHCP_MAGIC_COOKIE` | `0x63825363` | Fixed magic cookie marking the start of DHCP options, for `struct cnet_dhcp.cookie` |
| `CNET_DHCP_BROADCAST` | `0x8000` | Broadcast flag bit, for `struct cnet_dhcp.flags` |

## Option codes (DHCP option-list tag byte, `struct cnet_dhcp_option.code`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DHCP_OP_PAD` | `0x00` | Pad |
| `CNET_DHCP_OP_MASK` | `0x01` | Subnet mask |
| `CNET_DHCP_OP_ROUTER` | `0x03` | Router |
| `CNET_DHCP_OP_DNS_SERV` | `0x06` | DNS server |
| `CNET_DHCP_OP_HOSTNAME` | `0x0C` | Hostname |
| `CNET_DHCP_OP_DOMAIN_NAME` | `0x0F` | Domain name |
| `CNET_DHCP_OP_REQIP` | `0x32` | Requested IP address |
| `CNET_DHCP_OP_LTIME` | `0x33` | Address lease time |
| `CNET_DHCP_OP_MTYPE` | `0x35` | DHCP message type |
| `CNET_DHCP_OP_SERV_ID` | `0x36` | Server identifier |
| `CNET_DHCP_OP_PREQL` | `0x37` | Parameter request list |
| `CNET_DHCP_OP_RENT` | `0x3A` | Renewal (T1) time |
| `CNET_DHCP_OP_REBT` | `0x3B` | Rebinding (T2) time |
| `CNET_DHCP_OP_CID` | `0x3D` | Client identifier |
| `CNET_DHCP_OP_END` | `0xFF` | End of options |

## Message types (value carried by option `CNET_DHCP_OP_MTYPE`)

| Define | Value | Meaning |
|---|---|---|
| `CNET_DHCP_MSG_DISCOVER` | `0x01` | DHCPDISCOVER |
| `CNET_DHCP_MSG_OFFER` | `0x02` | DHCPOFFER |
| `CNET_DHCP_MSG_REQUEST` | `0x03` | DHCPREQUEST |
| `CNET_DHCP_MSG_DECLINE` | `0x04` | DHCPDECLINE |
| `CNET_DHCP_MSG_ACK` | `0x05` | DHCPACK |
| `CNET_DHCP_MSG_NAK` | `0x06` | DHCPNAK |
| `CNET_DHCP_MSG_RELEASE` | `0x07` | DHCPRELEASE |
| `CNET_DHCP_MSG_INFORM` | `0x08` | DHCPINFORM |

## Field-access macros

| Define | Expands to | Use |
|---|---|---|
| `CNET_DHCP_MACROS_CADDR8` / `CADDR32` | `caddr.c8addr` / `caddr.c32addr` | Client IP address, byte / 32-bit view |
| `CNET_DHCP_MACROS_YADDR8` / `YADDR32` | `yaddr.y8addr` / `yaddr.y32addr` | "Your" (client) IP address, byte / 32-bit view |
| `CNET_DHCP_MACROS_NADDR8` / `NADDR32` | `naddr.n8addr` / `naddr.n32addr` | Next server IP address, byte / 32-bit view |
| `CNET_DHCP_MACROS_GADDR8` / `GADDR32` | `gaddr.g8addr` / `gaddr.g32addr` | Relay agent (gateway) IP address, byte / 32-bit view |

## See also

- Struct reference: [`docs/structs/cnet_dhcp.md`](../structs/cnet_dhcp.md)
