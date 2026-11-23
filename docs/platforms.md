# Platforms

CNET has one shared protocol core and three transmit backends. The public
headers expose the same structures on every target; only the socket, buffer
and burst layer changes.

---

## Linux

**Backend:** kernel sockets (`AF_PACKET` for Layer 2, `AF_INET` raw for
Layer 3), `sendmmsg()`, optional `PACKET_TX_RING`.

### Headers used
`sys/socket.h`, `net/if.h`, `netinet/tcp.h`, `linux/if_packet.h`,
`arpa/inet.h`, `ifaddrs.h`, `endian.h` (plus `_GNU_SOURCE`).

### Discovery

```c
struct cnet_route route;
CNET_MEMSET(&route, sizeof(route));
CNET_CONFIGURE(&route);
```

`CNET_CONFIGURE()` walks `getifaddrs()`, skips `lo`, `tun*` and `ppp*`, picks
the first interface that is up and not a loopback, and fills:

| Field | Content |
|---|---|
| `route.iface[][]`, `route.count` | discovered interfaces |
| `route.active` | selected interface name |
| `route.ip.i32p` | local IPv4 address (network order) |
| `route.mac[6]` | local MAC address |

Returns `CNET_SCFL` or a negative `cnet_errno_t`.

### Sockets

```c
int fd = CNET_SOCK(&route, CNET_LAYER_ETHER, 0);     /* full Ethernet frames */
int fd = CNET_SOCK(&route, CNET_LAYER_IP, CNET_IP_TCP); /* raw IP, protocol TCP */
```

Layer 2 sockets are bound to `route.active`, use `PACKET_QDISC_BYPASS` and a
4 MiB `SO_SNDBUF`. Layer 3 sockets set `IP_HDRINCL`, so the IP header you
build is the header on the wire.

Sending helpers:

- `CNET_SOCK_ADDR_IN(&ip)` — `sockaddr_in` for `sendto()`.
- `CNET_SOCK_ADDR_LL(fd)` — `sockaddr_ll` for Layer 2.

### Batched send (`sendmmsg`)

```c
struct cnet_buffer buf;
CNET_MEMSET(&buf, sizeof(buf));

uint8_t *pkt0 = CNET_PACKET_UPTR(&buf, 0);
/* build packet 0 ... */
CNET_SIZE_SET(&buf, 0, offset0);

uint8_t *pkt1 = CNET_PACKET_UPTR(&buf, 1);
/* build packet 1 ... */
CNET_SIZE_SET(&buf, 1, offset1);

struct sockaddr_in dst = CNET_SOCK_ADDR_IN(ip);
CNET_BUFFER_INIT_ALL(&buf, 2, &dst, CNET_LAYER_IP);
CNET_BURST(&buf, fd, 2);
```

`CNET_BUFFER_INIT_SET()` prepares one slot; `CNET_BUFFER_INIT_ALL()` prepares
a range. `CNET_BUFFER_CLONE(&buf, dst_slot, src_slot)` copies a finished
packet.

### TX ring

```c
struct cnet_ring ring = CNET_RING_INIT(CNET_LAYER_ETHER, &route);
if (ring.sockfd < 0) {
    /* ring.code holds the reason */
}

/* fill buf with N packets and sizes */
CNET_RING_BURST(&ring, &buf, n);
```

The ring bypasses the qdisc and is intended for high packet rates. It uses
`PACKET_TX_RING`, `mmap`, `PACKET_QDISC_BYPASS` and `PACKET_LOSS`.

### TCP statistics

```c
struct cnet_tcp_info info;
if (CNET_GET_TCP_INFO(tcp_fd, &info) == CNET_SCFL) {
    printf("state=%u cwnd=%u rtt=%u us\n", info.state, info.cwnd, info.rtt);
}
```

Backed by `getsockopt(..., TCP_INFO, ...)`. `rtt`, `rttv`, `rto`, `ato` are in
microseconds.

### Installation

```bash
sudo ./installers/cnet_linux.sh
```

Installs headers to `$PREFIX/include` (default `/usr/local/include`) and
`libcnet.so` to `$PREFIX/lib`, plus a `cnet.so` symlink and an `ldconfig`
entry. Requirements: `gcc`, write access to the prefix (use `sudo`).

---

## Windows

**Backend:** Npcap. CNET opens a `pcap_t` on a device, fills a
`pcap_send_queue`, and transmits it. There is no OS TCP connection, because
CNET sends raw frames.

### Prerequisites

1. **Npcap runtime** installed with *WinPcap API-compatible Mode*. The DLLs
   live in `%SystemRoot%\System32\Npcap`.
2. **Npcap SDK** (headers + import libraries `wpcap.lib`, `Packet.lib`).
3. **MinGW-w64 GCC** on `PATH`.
4. Administrator rights for the installer (it writes under `%ProgramFiles%`
   and updates the machine `PATH`).

### Discovery

```c
struct cnet_route route;
CNET_MEMSET(&route, sizeof(route));

pcap_t *adapter = CNET_SOCK(&route);      /* uses route.active */
CNET_CONFIGURE(&route, adapter);
```

`CNET_CONFIGURE()` uses `GetBestRoute()` for the default gateway, enumerates
Npcap devices, reads the adapter MAC with
`pcap_oid_get_request(OID_802_3_CURRENT_ADDRESS)`, and resolves the gateway
MAC through `GetIpNetTable()`. Results:

| Field | Content |
|---|---|
| `route.iface[][]`, `route.count` | Npcap device names |
| `route.local[][]`, `route.local_count` | local IPv4 strings |
| `route.ip.i32p` | gateway address |
| `route.smac[6]` / `route.dmac[6]` | local / gateway MAC |

`CNET_SOCK(struct cnet_route *route)` takes only the route and returns a
`pcap_t *` (or `NULL`). It calls `pcap_open_live(..., 65535, 1, 1, ...)`.

### Batched send

```c
struct cnet_buffer buf;
CNET_BUFFER_INIT(&buf);                       /* allocates the send queue */

uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);
/* build a full Ethernet frame ... */
CNET_SIZE_SET(&buf, 0, offset);

CNET_BUFFER_PACKETS(&buf, 1);                 /* queue slot 0 */
CNET_BURST(&buf, adapter);                    /* transmit the queue */
```

`CNET_BUFFER_PACKETS()` queues up to 64 packets (`cbuf->pos` tracks the queue
position). `CNET_BURST()` calls `pcap_sendqueue_transmit()`.

### Compiling

```
gcc program.c src\cnet.c ^
  -Iheaders -I"%NPCAP_SDK%\Include" ^
  "%NPCAP_SDK%\Lib\x64\wpcap.lib" "%NPCAP_SDK%\Lib\x64\Packet.lib" ^
  -liphlpapi -lws2_32 -o program.exe
```

`cnet.h` uses `#pragma comment(lib, "iphlpapi.lib")` and
`#pragma comment(lib, "ws2_32.lib")`, which only MSVC honours; MinGW needs the
explicit `-l` flags shown above. The Npcap import libraries must also be
listed.

### Installation

Run `installers\cnet_win.bat` from an Administrator terminal. It:

1. re-launches itself elevated if needed;
2. locates the Npcap runtime and SDK (env var `NPCAP_SDK` or usual folders);
3. selects `Lib\x64` for 64-bit GCC, otherwise `Lib`;
4. copies headers to `%ProgramFiles%\CNET\include`;
5. builds `%ProgramFiles%\CNET\lib\cnet.dll`;
6. appends the library and Npcap runtime directories to the system `PATH`.

Open a new terminal afterwards.

---

## ESP32

**Backend:** `esp_wifi_80211_tx()` for raw 802.11 frames and lwIP for TCP
statistics. ESP32 has no `CNET_SOCK`, `CNET_CONFIGURE`, `CNET_BUFFER_INIT_*`
or BSD-socket wrapper: the radio is already open.

### Headers used
`lwip/api.h`, `lwip/sockets.h`, `lwip/priv/tcp_priv.h`, `WiFi.h`,
`esp_wifi.h`, `esp_log.h`, `esp_err.h`, `nvs_flash.h`, `esp_event.h`,
`esp_task_wdt.h`, `esp_bt.h`.

### Initialisation

```cpp
#include <cnet.hpp>

void setup() {
    CNET_ESP32_IWIP_INIT();       /* NVS, netif, event loop, STA, esp_wifi_start */
    CNET_ESP32_MAX_POWER(6);      /* 2.4 GHz channel 6, max TX power, promiscuous */
}
```

`CNET_ESP32_MAX_POWER(channel)`:

- disables brown-out reset, releases Bluetooth memory;
- calls `CNET_ESP32_IWIP_INIT()`;
- sets max TX power (80 = 20 dBm), 240 MHz CPU, no power save;
- enables 11b/g/n, deletes the task watchdog, disables logs;
- sets HT20, promiscuous mode and the requested channel.

### Sending a raw frame

```cpp
struct cnet_buffer buff;
CNET_MEMSET(&buff, sizeof(buff));

int offset = 0;
uint8_t *pkt = CNET_PACKET_UPTR(&buff);

struct cnet_frame_beacon *bcn =
    CNET_EXTRA_LAYER(struct cnet_frame_beacon, pkt, &offset);
/* fill bcn ... */

CNET_SIZE_SET(&buff, offset);
CNET_BURST(&buff, 0 /* WIFI_IF_STA */, false);
```

`CNET_BURST()` is defined in `cnet.hpp` and returns `esp_err_t`. It needs the
buffer already sized. FCS is normally added by the hardware.

### TCP statistics

`CNET_GET_TCP_INFO()` is declared in `cnet.hpp` and reads the lwIP PCB:

```cpp
struct cnet_tcp_info info;
if (CNET_GET_TCP_INFO(sock, &info) == CNET_SCFL) {
    printf("cwnd=%u rtt=%u unack=%u ca=%u\n",
           info.cwnd, info.rtt, info.unack, info.cnet_state_ca);
}
```

`rtt`, `rttv` and `rto` are converted from lwIP ticks to microseconds;
`unack` is the number of segments on `pcb->unacked`; `cnet_state_ca` is one of
the `enum cnet_state_ca` values.

### Packaging

Copy the headers and both sources into the Arduino library layout described
in [`extension/src/INSTALL.md`](../extension/src/INSTALL.md). In `.ino` files:

```cpp
extern "C" {
  #include <cnet_init.h>
  #include <cnet_frame/probe.h>
}
```

For ESP-IDF, add `headers/` to the component include path and compile
`src/cnet.c` and `src/cnet.cpp`. The C++ file is guarded by
`CNET_PLATFORM_ESP32`, so it is a no-op elsewhere.

### Installation

There is no installer, see
[`installers/cnet_esp32.md`](../installers/cnet_esp32.md).

---

## API availability

| Function | Linux | Windows | ESP32 |
|---|:---:|:---:|:---:|
| `CNET_SOCK` | `int` | `pcap_t *` | — |
| `CNET_CONFIGURE` | 1 arg | 2 args | — |
| `CNET_BURST` | `sendmmsg` | `pcap_send_queue` | `esp_wifi_80211_tx` |
| `CNET_BUFFER_INIT_SET` / `_ALL` | yes | — | — |
| `CNET_BUFFER_INIT` | — | yes | — |
| `CNET_BUFFER_PACKETS` | — | yes | — |
| `CNET_BUFFER_CLONE` | yes | yes | — |
| `CNET_RING_INIT` / `CNET_RING_BURST` | yes | — | — |
| `CNET_SOCK_ADDR_IN` / `_LL` | yes | — | — |
| `CNET_GET_TCP_INFO` | `TCP_INFO` | — | lwIP PCB |
| `CNET_ESP32_IWIP_INIT`, `CNET_ESP32_MAX_POWER` | — | — | yes |
| 802.11 frame structs | yes | yes | yes |
| `CNET_FRAME_FCS` | yes | yes | yes (hardware normally adds FCS) |
