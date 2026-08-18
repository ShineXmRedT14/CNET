# Defines — `cnet.h`

Core umbrella defines: layers, well-known ports, and internal sizing macros.

## Layers

| Define | Value | Meaning |
|---|---|---|
| `CNET_LAYER_ETHER` | `2` | Layer 2 (Ethernet) — used with `CNET_SOCK()` / `CNET_BUFFER_INIT_*` to select an `AF_PACKET` raw socket |
| `CNET_LAYER_IP` | `3` | Layer 3 (IP) — used to select an `AF_INET` raw socket |

## Misc

| Define | Value | Meaning |
|---|---|---|
| `CNET_BROADCAST` | `{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}` | Broadcast MAC address literal, ready to assign to a 6-byte MAC array |
| `CNET_IP_ICMP` | `0x01` | IP protocol number for ICMP (duplicated from `cnet_ip.h` for convenience) |
| `CNET_IP_TCP` | `0x06` | IP protocol number for TCP |
| `CNET_IP_UDP` | `0x11` | IP protocol number for UDP |

## Well-known ports

Every `CNET_PORT_*` define is a plain `uint16_t`-sized decimal port number in **host byte order** — remember to run it through `CNET_BIG16()` before placing it in a packet header.

| Define | Port | Service |
|---|---|---|
| `CNET_PORT_FTP_DAT` | 20 | FTP data |
| `CNET_PORT_FTP_SET` | 21 | FTP control |
| `CNET_PORT_SSH` | 22 | SSH |
| `CNET_PORT_TELNET` | 23 | Telnet |
| `CNET_PORT_SMTP` | 25 | SMTP |
| `CNET_PORT_DNS` | 53 | DNS |
| `CNET_PORT_DHCP_IP` | 67 | DHCP server |
| `CNET_PORT_DHCP_CLIENT` | 68 | DHCP client |
| `CNET_PORT_TFTP` | 69 | TFTP |
| `CNET_PORT_HTTP` | 80 | HTTP |
| `CNET_PORT_POP3` | 110 | POP3 |
| `CNET_PORT_NTP` | 123 | NTP |
| `CNET_PORT_IMAP` | 143 | IMAP |
| `CNET_PORT_SNMP_N1` | 161 | SNMP |
| `CNET_PORT_SNMP_N2` | 162 | SNMP trap |
| `CNET_PORT_LDAP` | 389 | LDAP |
| `CNET_PORT_HTTPS` | 443 | HTTPS |
| `CNET_PORT_SMB` | 445 | SMB |
| `CNET_PORT_SYSLOG` | 514 | Syslog |
| `CNET_PORT_LDAPS` | 636 | LDAP over TLS |
| `CNET_PORT_IMAPS` | 993 | IMAP over TLS |
| `CNET_PORT_POP3S` | 995 | POP3 over TLS |
| `CNET_PORT_MSSQL` | 1433 | Microsoft SQL Server |
| `CNET_PORT_ORACLE` | 1521 | Oracle DB |
| `CNET_PORT_MQTT` | 1883 | MQTT |
| `CNET_PORT_MYSQL` | 3306 | MySQL |
| `CNET_PORT_RDP` | 3389 | RDP |
| `CNET_PORT_SIP_N1` | 5060 | SIP |
| `CNET_PORT_SIP_N2` | 5061 | SIP over TLS |
| `CNET_PORT_PSQL` | 5432 | PostgreSQL |
| `CNET_PORT_RMQ` | 5672 | RabbitMQ / AMQP |
| `CNET_PORT_VNC` | 5900 | VNC |
| `CNET_PORT_REDIS` | 6379 | Redis |
| `CNET_PORT_HTTP8` | 8080 | HTTP (alt) |
| `CNET_PORT_HTTPS8` | 8443 | HTTPS (alt) |
| `CNET_PORT_FPM` | 9000 | PHP-FPM |
| `CNET_PORT_KAFKA` | 9092 | Kafka |
| `CNET_PORT_MDB` | 27017 | MongoDB |

## Internal sizing macros

These back the send buffer and IP header-length math; you rarely need them directly.

| Define | Expansion | Meaning |
|---|---|---|
| `MEGASIZE` | `4` | Multiplier used by `SIZEBUFF()` |
| `SIZEBUFF()` | `(MEGASIZE*1024*1024)` | Socket send-buffer size in bytes (4 MiB), used to size `SO_SNDBUF` in `CNET_SOCK()` |
| `IHL_LEN(s)` | `(s*4)` | Converts an IHL word-count (`s`) into a byte length |
| `IHL(s)` | `((20+s)/4)` | Converts extra option bytes (`s`) into an IHL word-count for `struct cnet_ip.ihl` |

## Macros

Macros for structures

| Define | Expansion | Meaning |
|---|---|---|
| `CNET_FRAME_MACROS_FCS32` | `fcs.fcs32` | Macros for struct cnet_frame_fcs |
| `CNET_FRAME_MACROS_FCS16` | `fcs.fcs16` | Macros for struct cnet_frame_fcs |
| `CNET_FRAME_MACROS_FCS8` | `fcs.fcs8` | Macros for struct cnet_frame_fcs |

## See also

- Struct reference: [`docs/structs/cnet.md`](../structs/cnet.md)
- Functions built on these defines: [`docs/functions/`](../functions/)
