# Macros and constants

Every public `#define` in `headers/`, grouped by header. Values are as written
in the source.

---

## Platform (`cnet_platform.h`)

`cnet_platform.h` auto-detects the target unless one of these is predefined:

| Macro | Detected from |
|---|---|
| `CNET_PLATFORM_WINDOWS` | `_WIN32` |
| `CNET_PLATFORM_LINUX` | `__linux__` |
| `CNET_PLATFORM_ESP32` | `ESP32`, `ESP_PLATFORM`, `ARDUINO_ARCH_ESP32` |

Defining more than one is a compile error. You may define exactly one yourself
before including any CNET header.

---

## General (`cnet_helper.h`)

### Layers
| Macro | Value |
|---|---|
| `CNET_LAYER_ETHER` | 2 |
| `CNET_LAYER_IP` | 3 |

### Addresses
| Macro | Value |
|---|---|
| `CNET_BROADCAST` | `{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}` |

### IP protocols
| Macro | Value |
|---|---|
| `CNET_IP_ICMP` | 0x01 |
| `CNET_IP_IGMP` | 0x02 |
| `CNET_IP_TCP` | 0x06 |
| `CNET_IP_UDP` | 0x11 |
| `CNET_IP_RAW` | 0xFF |

### Ports
| Macro | Value | Macro | Value |
|---|---:|---|---:|
| `CNET_PORT_FTP_DAT` | 20 | `CNET_PORT_FTP_SET` | 21 |
| `CNET_PORT_SSH` | 22 | `CNET_PORT_TELNET` | 23 |
| `CNET_PORT_SMTP` | 25 | `CNET_PORT_DNS` | 53 |
| `CNET_PORT_DHCP_IP` | 67 | `CNET_PORT_DHCP_CLIENT` | 68 |
| `CNET_PORT_TFTP` | 69 | `CNET_PORT_HTTP` | 80 |
| `CNET_PORT_POP3` | 110 | `CNET_PORT_NTP` | 123 |
| `CNET_PORT_IMAP` | 143 | `CNET_PORT_SNMP_N1` | 161 |
| `CNET_PORT_SNMP_N2` | 162 | `CNET_PORT_LDAP` | 389 |
| `CNET_PORT_HTTPS` | 443 | `CNET_PORT_SMB` | 445 |
| `CNET_PORT_SYSLOG` | 514 | `CNET_PORT_LDAPS` | 636 |
| `CNET_PORT_IMAPS` | 993 | `CNET_PORT_POP3S` | 995 |
| `CNET_PORT_MSSQL` | 1433 | `CNET_PORT_ORACLE` | 1521 |
| `CNET_PORT_MQTT` | 1883 | `CNET_PORT_MYSQL` | 3306 |
| `CNET_PORT_RDP` | 3389 | `CNET_PORT_SIP_N1` | 5060 |
| `CNET_PORT_SIP_N2` | 5061 | `CNET_PORT_PSQL` | 5432 |
| `CNET_PORT_RMQ` | 5672 | `CNET_PORT_VNC` | 5900 |
| `CNET_PORT_REDIS` | 6379 | `CNET_PORT_HTTP8` | 8080 |
| `CNET_PORT_HTTPS8` | 8443 | `CNET_PORT_FPM` | 9000 |
| `CNET_PORT_KAFKA` | 9092 | `CNET_PORT_MDB` | 27017 |

### Limits
| Macro | Value |
|---|---|
| `MEGASIZE` | 4 |
| `SIZEBUFF()` | `(MEGASIZE*1024*1024)` |
| `CNET_MAX_IPLEN` | 16 |
| `CNET_MAX_ETHLEN` | 18 |

### Accessor aliases
`CNET_FRAME_MACROS_FCS32` → `fcs.fcs32`,
`CNET_FRAME_MACROS_FCS16` → `fcs.fcs16`,
`CNET_FRAME_MACROS_FCS8` → `fcs.fcs8`.

---

## Ethernet (`cnet_ether.h`)

| Macro | Value |
|---|---|
| `CNET_ETH_IP` | 0x0800 |
| `CNET_ETH_ARP` | 0x0806 |
| `CNET_ETH_RARP` | 0x8035 |
| `CNET_ETH_IPV6` | 0x86DD |
| `CNET_ETH_VLAN` | 0x8100 |

## VLAN (`cnet_vlan.h`)

| Macro | Value |
|---|---|
| `CNET_VLAN_ID_Q` | 0x8100 |
| `CNET_VLAN_ID_AD` | 0x88A8 |
| `CNET_VLAN_PCP_BK` | 0x0001 (background) |
| `CNET_VLAN_PCP_BE` | 0x0000 (best effort) |
| `CNET_VLAN_PCP_EE` | 0x0002 (excellent effort) |
| `CNET_VLAN_PCP_CA` | 0x0003 (critical applications) |
| `CNET_VLAN_PCP_VI` | 0x0004 (video) |
| `CNET_VLAN_PCP_VO` | 0x0005 (voice) |
| `CNET_VLAN_PCP_IC` | 0x0006 (internetwork control) |
| `CNET_VLAN_PCP_NC` | 0x0007 (network control) |

## ARP (`cnet_arp.h`)

| Macro | Value |
|---|---|
| `CNET_ARP_REQ` | 0x01 |
| `CNET_ARP_REP` | 0x02 |
| `CNET_ARP_RREQ` | 0x03 |
| `CNET_ARP_RREP` | 0x04 |
| `CNET_ARP_IREQ` | 0x08 |
| `CNET_ARP_IREP` | 0x09 |
| `CNET_ARP_NAK` | 0x0A |

---

## IPv4 (`cnet_ip.h`)

| Macro | Value |
|---|---|
| `CNET_IP_ICMP` | 0x01 |
| `CNET_IP_IGMP` | 0x02 |
| `CNET_IP_TCP` | 0x06 |
| `CNET_IP_UDP` | 0x11 |
| `CNET_IP_RAW` | 0xFF |
| `CNET_IP_OPTIONS` | 0xDA |
| `CNET_IP_NOPTIONS` | 0xDB |
| `CNET_IP_CUSTOM` | 0xCA |
| `CNET_IP_NCUSTOM` | 0xCB |
| `CNET_IP_VERSION` | 0x04 |
| `CNET_IP_TYPE` | 0x00 |
| `CNET_IP_TTL` | 0x40 |
| `CNET_IP_TSP_ONLY` | 0x00 |
| `CNET_IP_TSP_ADDR` | 0x01 |
| `CNET_IP_TSP_DEFINED` | 0x03 |

Field aliases: `CNET_IP_MACROS_SRC8`, `_SRC32`, `_DST8`, `_DST32`,
`CNET_IP_MACROS_BYTES8`, `_BYTES32`.

## IPv6 (`cnet_ipv6.h`)

| Macro | Value |
|---|---|
| `CNET_IPV6_ICMP` | 0x01 |
| `CNET_IPV6_TCP` | 0x06 |
| `CNET_IPV6_UDP` | 0x11 |
| `CNET_IPV6_ICMPV6` | 0x3A |
| `CNET_IPV6_PAD1` | 0 |
| `CNET_IPV6_PADN` | 1 |
| `CNET_IPV6_STLEN` | 16 |

---

## ICMP (`cnet_icmp.h`)

### Types
| Macro | Value |
|---|---|
| `CNET_ICMP_EREPLY` | 0x00 |
| `CNET_ICMP_UNREACH` | 0x03 |
| `CNET_ICMP_QUENCH` | 0x04 |
| `CNET_ICMP_ECHO` | 0x08 |
| `CNET_ICMP_EXCEEDED` | 0x0B |
| `CNET_ICMP_PROB` | 0x0C |
| `CNET_ICMP_TIMESTAMP` | 0x0D |
| `CNET_ICMP_TIMEREPLY` | 0x0E |
| `CNET_ICMP_IREQ` | 0x0F |
| `CNET_ICMP_IREP` | 0x10 |
| `CNET_ICMP_ADDR` | 0x11 |
| `CNET_ICMP_ADDRREPLY` | 0x12 |

### Destination-unreachable codes
`CNET_ICMP_CODE_NET_UNREACH` 0x00, `_HOST_UNREACH` 0x01,
`_PROTOCOL_UNREACH` 0x02, `_PORT_UNREACH` 0x03, `_FRAG_NEEDED` 0x04,
`_SOURCE_ROUTE_FAILED` 0x05, `_NET_UNKNOWN` 0x06, `_HOST_UNKNOWN` 0x07,
`_HOST_ISOLATED` 0x08, `_NET_PROHIBITED` 0x09, `_HOST_PROHIBITED` 0x0A,
`_NET_UNREACH_TOS` 0x0B, `_HOST_UNREACH_TOS` 0x0C,
`_FILTER_PROHIBITED` 0x0D, `_HOST_PRECEDENCE` 0x0E,
`_PRECEDENCE_CUTOFF` 0x0F`.

### Redirect codes
`CNET_ICMP_CODE_REDIRECT_NET` 0x00, `_REDIRECT_HOST` 0x01,
`_REDIRECT_TOS_NET` 0x02, `_REDIRECT_TOS_HOST` 0x03.

### Time-exceeded codes
`CNET_ICMP_CODE_TTL_EXPIRED` 0x00, `_FRAG_REASSEMBLY` 0x01.

### Parameter-problem codes
`CNET_ICMP_CODE_POINTER_ERROR` 0x00, `_MISSING_OPTION` 0x01,
`_BAD_LENGTH` 0x02.

Aliases: `CNET_ICMP_MACROS_RAW`, `_ECHO`, `_GATE`, `_MTU`.

## ICMPv6 (`cnet_icmpv6.h`)

| Macro | Value |
|---|---|
| `CNET_ICMPV6_RA_MANAGED` | 0x80 |
| `CNET_ICMPV6_RA_OTHER` | 0x40 |
| `CNET_ICMPV6_RA_AGENT` | 0x20 |
| `CNET_ICMPV6_RA_PRF` | 0x18 |
| `CNET_ICMPV6_RA_PROXY` | 0x04 |
| `CNET_ICMPV6_RA_PRF_HIGH` | `(0x1 << 3)` |
| `CNET_ICMPV6_RA_PRF_MEDIUM` | `(0x0 << 3)` |
| `CNET_ICMPV6_RA_PRF_LOW` | `(0x3 << 3)` |
| `CNET_ICMPV6_RA_PRF_RESERVE` | `(0x2 << 3)` |
| `CNET_ICMPV6_ECHO_REQ` | 0x80 |
| `CNET_ICMPV6_ECHO_REP` | 0x81 |
| `CNET_ICMPV6_RA` | 0x86 |
| `CNET_ICMPV6_NA` | 0x88 |
| `CNET_ICMPV6_MTU` | 0x02 |

Aliases: `CNET_ICMPV6_MACROS_RAW`, `_ECHO`, `_RA`, `_NA`, `_MTU`.

---

## TCP (`cnet_tcp.h`)

### Flags
| Macro | Value |
|---|---|
| `CNET_TCP_FIN` | 0x0001 |
| `CNET_TCP_SYN` | 0x0002 |
| `CNET_TCP_RST` | 0x0004 |
| `CNET_TCP_PSH` | 0x0008 |
| `CNET_TCP_ACK` | 0x0010 |
| `CNET_TCP_URG` | 0x0020 |

### Defaults
| Macro | Value |
|---|---|
| `CNET_TCP_DOFF` | 0x0005 |
| `CNET_TCP_RES1` | 0x0000 |
| `CNET_TCP_RES2` | 0x0000 |
| `CNET_TCP_WIN` | 0xFFFF |

### `CNET_PACKET_TCP_FAST_SET_INFO` mode
| Macro | Value |
|---|---|
| `CNET_TCP_INFO_LOCAL` | 0xBC |
| `CNET_TCP_INFO_WIFI` | 0xBA |

---

## SCTP (`cnet_sctp.h`)

### Chunk types
`CNET_SCTP_DATA` 0x00, `_INIT` 0x01, `_INIT_ACK` 0x02, `_SACK` 0x03,
`_HBEAT` 0x04, `_HBEAT_ACK` 0x05, `_ABORT` 0x06, `_SHUTDOWN` 0x07,
`_SHUTDOWN_ACK` 0x08, `_ERR` 0x09, `_COOKIE_ECHO` 0x0A,
`_COOKIE_ACK` 0x0B, `_ECN_ECNE` 0x0C, `_ECN_CWR` 0x0D,
`_SHUTDOWN_COMPLETE` 0x0E, `_AUTH` 0x0F, `_TSN` 0xC0, `_ASCONF` 0xC1,
`_ASCONF_ACK` 0x80.

### DATA flags
`CNET_SCTP_DATA_E` 0x01, `_B` 0x02, `_U` 0x04, `_I` 0x08.

### Parameters
`CNET_SCTP_PARAM_HBEAT_INFO` 0x0001, `_IPV4_ADDR` 0x0005,
`_IPV6_ADDR` 0x0006, `_STCOOKIE` 0x0007, `_UNR_PARAM` 0x0008,
`_COOKIE_PRESERV` 0x0009, `_HOST_ADDR` 0x000B, `_SUPPORTED_ADDR` 0x000C,
`_ECN_CAP` 0x8000, `_RANDOM` 0x8002, `_LCHUNK` 0x8003, `_HMAC` 0x8004,
`_SUPPORTED_EXT` 0x8008, `_SUPPORTED_TSN` 0xC000.

### ASCONF parameters
`CNET_SCTP_PARAM_ADD_IP` 0xC001, `_DEL_IP` 0xC002, `_ERR_CAUSE` 0xC003,
`_SET_PRIMARY` 0xC004, `_SUCCESS` 0xC005, `_ADAPT_LAYER` 0xC006.

### Misc
`CNET_SCTP_ABORT_T` 0x01, `CNET_SCTP_SHUTDOWN_T` 0x01.

### Error causes
`CNET_SCTP_ERROR_INVALID_STREAM` 0x0001, `_MISSING_PARAM` 0x0002,
`_STALE_COOKIE` 0x0003, `_OOR` 0x0004, `_UNR_ADDR` 0x0005,
`_UNR_CHUNK` 0x0006, `_INVALID_PARAM` 0x0007, `_UNR_PARAM` 0x0008,
`_NUSER_DATA` 0x0009, `_COOKIE_SHUTDOWN` 0x000A, `_RASSOC` 0x000B,
`_UIA` 0x000C, `_PV` 0x000D`.

---

## DHCP (`cnet_dhcp.h`)

| Macro | Value |
|---|---|
| `CNET_DHCP_OP_REQ` | 0x01 |
| `CNET_DHCP_OP_REP` | 0x02 |
| `CNET_DHCP_HTYPE_ETHER` | 0x01 |
| `CNET_DHCP_MAGIC_COOKIE` | 0x63825363 |
| `CNET_DHCP_BROADCAST` | 0x8000 |

### Option codes
`CNET_DHCP_OP_PAD` 0x00, `_MASK` 0x01, `_ROUTER` 0x03, `_DNS_SERV` 0x06,
`_HOSTNAME` 0x0C, `_DOMAIN_NAME` 0x0F, `_REQIP` 0x32, `_LTIME` 0x33,
`_MTYPE` 0x35, `_SERV_ID` 0x36, `_PREQL` 0x37, `_RENT` 0x3A,
`_REBT` 0x3B, `_CID` 0x3D, `_END` 0xFF.

### Messages
`CNET_DHCP_MSG_DISCOVER` 0x01, `_OFFER` 0x02, `_REQUEST` 0x03,
`_DECLINE` 0x04, `_ACK` 0x05, `_NAK` 0x06, `_RELEASE` 0x07,
`_INFORM` 0x08.

Aliases: `CNET_DHCP_MACROS_CADDR8/32`, `_YADDR8/32`, `_NADDR8/32`,
`_GADDR8/32`.

---

## DNS (`cnet_dns.h`)

### Flags (bit masks)
| Macro | Value |
|---|---|
| `CNET_DNS_FLAG_QR` | 0x8000 |
| `CNET_DNS_FLAG_OPCODE` | 0x7800 |
| `CNET_DNS_FLAG_AA` | 0x0400 |
| `CNET_DNS_FLAG_TC` | 0x0200 |
| `CNET_DNS_FLAG_RD` | 0x0100 |
| `CNET_DNS_FLAG_RA` | 0x0080 |
| `CNET_DNS_FLAG_RES` | 0x0070 |
| `CNET_DNS_FLAG_RCODE` | 0x000F |

### Opcodes
`CNET_DNS_OPCODE_QUERY` 0x0000, `_IQUERY` 0x0001, `_STATUS` 0x0002,
`_NOTIFY` 0x0004, `_UPDATE` 0x0005.

### Response codes
`CNET_DNS_RCODE_NERR` 0x0000, `_FMERR` 0x0001, `_SFAIL` 0x0002,
`_NXDOMAIN` 0x0003, `_NTIMP` 0x0004, `_REFUSED` 0x0005.

### Record types
`CNET_DNS_TYPE_A` 0x0001, `_NS` 0x0002, `_CNAME` 0x0005, `_SOA` 0x0006,
`_PTR` 0x000C, `_MX` 0x000F, `_TXT` 0x0010, `_AAAA` 0x001C, `_SRV` 0x0021,
`_OPT` 0x0029.

`CNET_DNS_CLASS` 0x0001.

---

## NTP (`cnet_ntp.h`)

| Macro | Value |
|---|---|
| `CNET_NTP_VERSION` | 0x4 |
| `CNET_NTP_STRAT_KOD` | 0x00 |
| `CNET_NTP_STRAT_PRIMARY` | 0x01 |
| `CNET_NTP_LI_NONE` | 0x0 |
| `CNET_NTP_LI_LM61` | 0x1 |
| `CNET_NTP_LI_LM59` | 0x2 |
| `CNET_NTP_LI_ALARM` | 0x3 |
| `CNET_NTP_MD_RES` | 0x0 |
| `CNET_NTP_MD_SACT` | 0x1 |
| `CNET_NTP_MD_SPAS` | 0x2 |
| `CNET_NTP_MD_CLIENT` | 0x3 |
| `CNET_NTP_MD_SERVER` | 0x4 |
| `CNET_NTP_MD_BCAST` | 0x5 |
| `CNET_NTP_MD_CNTRL` | 0x6 |
| `CNET_NTP_MD_PRIVATE` | 0x7 |

---

## TLS (`cnet_tls.h`)

### Content types
`CNET_TLS_TYPE_CCS` 0x14, `_ALERT` 0x15, `_HANDSHAKE` 0x16, `_DATA` 0x17.

### Versions
`CNET_TLS_VERSION_1_0` 0x0301, `_1_1` 0x0302, `_1_2` 0x0303,
`_1_3` 0x0304.

### Handshake types
`CNET_TLS_HANDSHAKE_CLIENT` 0x01, `_SERVER` 0x02, `_CERT` 0x0B,
`_SERVER_KE` 0x0C, `_SERVER_DONE` 0x0E, `_CLIENT_KE` 0x10,
`_FINISHED` 0x14.

---

## 802.11 (`cnet_frame/hdr.h`)

### Frame types
`CNET_FRAME_TYPE_MGT` 0x0, `_CTL` 0x1, `_DATA` 0x2.

### Management subtypes
| Macro | Value | Macro | Value |
|---|---:|---|---:|
| `CNET_FRAME_STYPE_ASSOC_REQ` | 0x0 | `CNET_FRAME_STYPE_ASSOC_RESP` | 0x1 |
| `CNET_FRAME_STYPE_REASSOC_REQ` | 0x2 | `CNET_FRAME_STYPE_REASSOC_RESP` | 0x3 |
| `CNET_FRAME_STYPE_PROBE_REQ` | 0x4 | `CNET_FRAME_STYPE_PROBE_RESP` | 0x5 |
| `CNET_FRAME_STYPE_ADV` | 0x6 | `CNET_FRAME_STYPE_RES7` | 0x7 |
| `CNET_FRAME_STYPE_BEACON` | 0x8 | `CNET_FRAME_STYPE_ATIM` | 0x9 |
| `CNET_FRAME_STYPE_DIASSOC` | 0xA | `CNET_FRAME_STYPE_AUTH` | 0xB |
| `CNET_FRAME_STYPE_DEAUTH` | 0xC | `CNET_FRAME_STYPE_ACTION` | 0xD |
| `CNET_FRAME_STYPE_ACTION_NA` | 0xE | `CNET_FRAME_STYPE_RES15` | 0xF |

### Control subtypes
`CNET_FRAME_STYPE_TRIGGER` 0x2, `_BFRPT` 0x4, `_VNA` 0x5, `_CTLE` 0x6,
`_CTLW` 0x7, `_BAR` 0x8, `_BA` 0x9, `_PSP` 0xA, `_RTS` 0xB, `_CTS` 0xC,
`_ACK` 0xD, `_CFE` 0xE, `_CFEA` 0xF.

### Data subtypes
`CNET_FRAME_STYPE_DATA` 0x0, `_DATA_CFA` 0x1, `_DATA_CFP` 0x2,
`_DATA_CFAP` 0x3, `_NF` 0x4, `_CF` 0x5, `_CFA` 0x5, `_CFP` 0x6,
`_CFAP` 0x7, `_QOSD` 0x8, `_QOSD_CFA` 0x9, `_QOSD_CFP` 0xA,
`_QOSD_CFAP` 0xB, `_QOS_NF` 0xC, `_QOS_CFA` 0xD, `_QOS_CFP` 0xE,
`_QOS_CFAP` 0xF.

### Flags
| Macro | Value |
|---|---|
| `CNET_FRAME_FLAG_TO_DS` | 0x01 |
| `CNET_FRAME_FLAG_FROM_DS` | 0x02 |
| `CNET_FRAME_FLAG_MORE_FRAG` | 0x04 |
| `CNET_FRAME_FLAG_RETRY` | 0x08 |
| `CNET_FRAME_FLAG_PWR` | 0x10 |
| `CNET_FRAME_FLAG_MORE_DATA` | 0x20 |
| `CNET_FRAME_FLAG_PROTECT` | 0x40 |
| `CNET_FRAME_FLAG_ORDER` | 0x80 |

`CNET_FRAME_VERSION` 0x0.

### Aliases
`CNET_FRAME_MACROS_ID`, `_ADDR1`, `_ADDR2`, `_ADDR3`, `_SEQ`, `_VERSION`,
`_TYPE`, `_STYPE`, `_FLAGS`, `_RVERSION`, `_RTYPE`, `_RSTYPE`, `_RFLAGS`.

---

## 802.11 definitions (`cnet_frame/definition.h`)

### Status codes
`CNET_FRAME_STATUS_S` 0x0000, `_UF` 0x0001, `_CU` 0x000A, `_REANA` 0x000B,
`_ADU` 0x000C, `_AAD` 0x000D, `_ASI` 0x000E, `_CF` 0x000F, `_AT` 0x0010,
`_AUHSA` 0x0011, `_ADR` 0x0012, `_ADSP` 0x0013, `_ADP` 0x0014,
`_ADCA` 0x0015, `_ADSM` 0x0016, `_ADBPC` 0x0017, `_ADBC` 0x0018,
`_ADSST` 0x0019, `_ADDO` 0x001A, `_II` 0x0028, `_IGC` 0x0029,
`_IPC` 0x002A, `_IA` 0x002B, `_URV` 0x002C, `_IRC` 0x002D, `_CR` 0x002E.

### Authentication algorithms
`CNET_FRAME_AUTH_OPEN` 0x0000, `_WEP` 0x0001, `_FT` 0x0002, `_WPA3` 0x0003.

### Reason codes
`CNET_FRAME_REASON_UNSPECIFIED` 0x0001, `_ANLV` 0x0002, `_DL` 0x0003,
`_DI` 0x0004, `_DAB` 0x0005, `_C2FN` 0x0006, `_C3FN` 0x0007, `_DSL` 0x0008,
`_SNA` 0x0009, `_BPC` 0x000A, `_BSC` 0x000B, `_II` 0x000D, `_MF` 0x000E,
`_4WHT` 0x000F, `_GKT` 0x0010, `_ID` 0x0011, `_IGC` 0x0012, `_IPC` 0x0013,
`_IA` 0x0014, `_URV` 0x0015, `_IRC` 0x0016, `_8021XAF` 0x0017,
`_CSR` 0x0018.

### Action categories
`CNET_FRAME_ACTION_CSM` 0x00, `_CQ` 0x01, `_CD` 0x02, `_CBA` 0x03,
`_CP` 0x04, `_CRM` 0x05, `_CFBT` 0x06, `_CH` 0x07, `_CSQ` 0x08,
`_CPD` 0x09, `_CW` 0x0A, `_CM` 0x0D, `_CMH` 0x0E, `_CSP` 0x0F,
`_CV` 0x15, `_CVS` 0x7F.

---

## Probe request element ids (`cnet_frame/probe.h`)

| Macro | Value |
|---|---|
| `CNET_FRAME_PROBE_BODY_EID` | 0x00 |
| `CNET_FRAME_PROBE_BODY_LEN` | 0x00 |
| `CNET_FRAME_PROBE_RATES_EID` | 0x01 |
| `CNET_FRAME_PROBE_RATES_LEN` | 0x08 |
| `CNET_FRAME_PROBE_RATES_MB1` | 0x82 |
| `CNET_FRAME_PROBE_RATES_MB2` | 0x84 |
| `CNET_FRAME_PROBE_RATES_MB5_5` | 0x8B |
| `CNET_FRAME_PROBE_RATES_MB11` | 0x96 |
| `CNET_FRAME_PROBE_RATES_MB6` | 0x0C |
| `CNET_FRAME_PROBE_RATES_MB9` | 0x12 |
| `CNET_FRAME_PROBE_RATES_MB12` | 0x18 |
| `CNET_FRAME_PROBE_RATES_MB18` | 0x24 |

---

## Block-ack bitmap aliases (`cnet_frame/block.h`)

`CNET_FRAME_BLOCK_MACROS_BIT8` → `map.b8it`,
`_BIT16` → `map.b16it`, `_BIT32` → `map.b32it`, `_BIT64` → `map.b64it`.

## Buffer aliases (`cnet_buffer.h`)

`APY` = 64 (slots), `APX` = 2048 (bytes per slot).

## Route limits (`cnet_route.h`)

`CNET_IFMAX` = 32, `CNET_LOCMAX` = 16.
