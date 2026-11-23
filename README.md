# CNET

CNET is a compact C library for **crafting and sending raw network packets**.
It gives you wire-accurate protocol structures, byte-order and checksum
helpers, a layer-builder for stacking headers in one buffer, and a transmit
backend for Linux, Windows (Npcap) and ESP32.

```c
#include <cnet.h>

struct cnet_ip  ip  = {0};
struct cnet_tcp tcp = {0};

ip.version = CNET_IP_VERSION;
ip.ihl     = IP_IHL_DECODE(sizeof(ip));
ip.ttl     = CNET_IP_TTL;
ip.proto   = CNET_IP_TCP;
/* addresses ... */
ip.sum = CNET_L3_CSUM(&ip, sizeof(ip));

tcp.dst  = CNET_BIG16(80);
tcp.seq  = CNET_BIG32(1);
tcp.doff = CNET_TCP_DOFF;
tcp.flags = CNET_TCP_SYN;
tcp.sum  = CNET_L4_CSUM(CNET_ETH_IP, &ip, &tcp, sizeof(tcp));
```

## What is included

| Area | Highlights |
|---|---|
| Protocol headers | Ethernet, VLAN, ARP, IPv4, IPv6, ICMP, ICMPv6, IGMP, TCP, UDP, SCTP, DHCP, DNS, NTP, TLS |
| 802.11 frames | 22 headers under `headers/cnet_frame/` (management, control, data) |
| Packet building | `CNET_EXTRA_LAYER`, `CNET_CREATE_LAYER`, `CNET_L4_EXPAND`, `CNET_PACKET_*_FAST_SET` |
| Checksums | `CNET_L3_CSUM`, `CNET_L4_CSUM`, `CNET_SCTP_CSUM`, `CNET_FRAME_FCS` |
| Utilities | byte order, address parsing, random IP/MAC, NTP timestamps, TCP statistics |
| Sending | Linux `sendmmsg` / TX ring, Windows `pcap_send_queue`, ESP32 `esp_wifi_80211_tx` |

## Repository layout

```text
CNET/
├── headers/            all public headers
│   └── cnet_frame/     802.11 frame definitions
├── src/
│   ├── cnet.c          portable core + Linux/Windows backends
│   └── cnet.cpp        ESP32 backend (lwIP / esp_wifi)
├── installers/         cnet_linux.sh, cnet_win.bat, cnet_esp32.md
├── extension/          Arduino library packaging
├── docs/               full documentation
├── INSTALL.md
├── VERSION.md
└── LICENSE
```

## Platform support

| Platform | Transmit backend | Install |
|---|---|---|
| Linux | `AF_PACKET` / raw `AF_INET`, `sendmmsg`, `PACKET_TX_RING` | `sudo ./installers/cnet_linux.sh` |
| Windows | Npcap (`pcap_t`, `pcap_send_queue`) | `installers\cnet_win.bat` (Administrator) |
| ESP32 | `esp_wifi_80211_tx` (raw 802.11), lwIP for TCP info | manual copy, see `extension/src/INSTALL.md` |

Linux needs root / `CAP_NET_RAW`. Windows needs Npcap with **WinPcap
API-compatible Mode** plus the Npcap SDK. ESP32 needs ESP-IDF or
Arduino-ESP32.

## Build directly

```bash
gcc program.c src/cnet.c -Iheaders -o program
```

Windows:

```bat
gcc program.c src\cnet.c -Iheaders -I"%NPCAP_SDK%\Include" ^
  "%NPCAP_SDK%\Lib\x64\wpcap.lib" "%NPCAP_SDK%\Lib\x64\Packet.lib" ^
  -liphlpapi -lws2_32 -o program.exe
```

## Documentation

- [docs/getting-started.md](docs/getting-started.md) — install and first packet
- [docs/platforms.md](docs/platforms.md) — Linux / Windows / ESP32 backends
- [docs/api.md](docs/api.md) — every function
- [docs/structures.md](docs/structures.md) — every struct and enum
- [docs/defines.md](docs/defines.md) — every macro and constant
- [docs/protocols.md](docs/protocols.md) — wire layouts and usage
- [docs/examples.md](docs/examples.md) — complete examples
- [docs/caveats.md](docs/caveats.md) — limits and known issues

Also see [INSTALL.md](INSTALL.md) for detailed installation steps.

## Status

Current version: (see [VERSION.md](VERSION.md)).
Many protocol definitions are provided without state machines or parsers, and
not every protocol has been tested. Read
[docs/caveats.md](docs/caveats.md) before using CNET in production.

## License

MIT — see [LICENSE](LICENSE). Author: ShineXmRedT14 (Shine).
