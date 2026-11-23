# Caveats and known issues

The library is labelled **Alpha** and the project's own
[`IMPORTANTLY.md`](../IMPORTANTLY.md) states that many protocols have not been
fully tested. This page lists behaviours you should know before relying on
CNET in production. It documents the current source; it does not change it.

---

## General

- **`CNET_MEMSET(ptr, n)` takes a byte count, not a fill value.** It always
  writes zero. `CNET_MEMSET(&x, sizeof(x))` is correct; `CNET_MEMSET(&x, 0xFF)`
  is not a way to fill with `0xFF`.
- **`CNET_DIRECT_SWAP32()` swaps 16-bit halves.** The implementation maps
  bytes `b0 b1 b2 b3` to `b2 b3 b0 b1`. It is not a full 32-bit byte reversal;
  use `CNET_BIG32()` / `CNET_LITTLE32()` for typed values.
- **`CNET_EXTRA_LAYER` is a GNU statement expression.** It requires GCC/Clang
  with GNU extensions. MSVC cannot compile it.
- **No bounds checking inside packet structs.** `CNET_EXTRA_LAYER` trusts the
  buffer size. Keep packets below `APX` (2048) bytes per slot.
- **`CNET_L4_CSUM` expects an EtherType**, not an IP protocol number:
  `CNET_ETH_IP` (0x0800) or `CNET_ETH_IPV6` (0x86DD). Any other value returns
  `0x0000`.
- **Bitfields are endian-guarded but not portable across compilers' bitfield
  allocation rules.** The headers include `#error` branches for unknown byte
  order. Access ports/flags through the provided aliases where possible.
- **Unaligned casts.** Checksum code reads `uint16_t`/`uint32_t` through casts
  from packet buffers. On strict-alignment architectures this can fault;
  x86/ARM generally tolerate it.

## Linux

- **`CNET_SOCK_ADDR_LL()` calls `CNET_GET_IF()`, which is not declared or
  defined** in the current headers. Build the `sockaddr_ll` manually (as in
  [examples.md](examples.md#1-linux-ethernet--ipv4--tcp-layer-2)) until this
  is resolved.
- **`CNET_PACKET_IP_FAST_SET(..., CNET_IP_OPTIONS)`** sets IHL/length for a
  60-byte header but still computes the IP checksum over
  `sizeof(struct cnet_ip)` (20 bytes). After filling the option bytes,
  recompute `ip->sum` over the full 60-byte header.
- **Raw sockets need privileges.** Run as root or grant `CAP_NET_RAW`.
- **`CNET_RING_INIT()` ignores its `layer` argument** when constructing the
  ring socket; the TX ring always uses a raw packet socket.
- **`CNET_BURST()` only prints errors**, it does not return an error code.
  Check the return of `sendmmsg()` yourself if you need reliability.

## Windows

- **No TCP statistics.** `CNET_GET_TCP_INFO()` is not declared or implemented
  for Windows. CNET sends pre-built frames through Npcap; it does not create
  an OS TCP connection, so `TCP_INFO` is not applicable.
- **`CNET_CONFIGURE()` returns `CNET_N_FND` / `CNET_FND`**, which are not
  defined in the public headers. If the build fails on these names, the
  function cannot be used as shipped.
- **`cnet.h` includes `<pcap/pcap.h>`.** Some Npcap SDK layouts provide
  `<pcap.h>` instead. The installer warns when `Include\pcap\pcap.h` is
  missing; add the appropriate `-I` path or adjust the include for your SDK.
- **`#pragma comment(lib, ...)` is MSVC-only.** MinGW needs
  `-liphlpapi -lws2_32` and the Npcap `.lib` files explicitly.
- **Administrator rights are required** to install into `%ProgramFiles%` and
  edit the machine `PATH`.
- **`CNET_CONFIGURE()` needs `route.active` set before `CNET_SOCK()`**, but
  `route.active` is normally filled by `CNET_CONFIGURE()`. Enumerate Npcap
  devices (`pcap_findalldevs`) and set `route.active` yourself first.

## ESP32

- **`CNET_GET_TCP_INFO()` lives in `cnet.hpp`**, not `cnet.h`. Include
  `cnet.hpp` from C++ code, or declare the prototype yourself in C.
- **lwIP internals are required.** The implementation includes
  `lwip/priv/tcp_priv.h` and calls `lwip_socket_dbg_get_socket()`. These are
  not part of the stable public lwIP API and may change between ESP-IDF
  versions.
- **Tick conversion assumes `TCP_SLOW_INTERVAL * 1000` microseconds per
  lwIP tick.** If `TCP_SLOW_INTERVAL` is configured differently, `rtt`,
  `rttv` and `rto` will be scaled incorrectly.
- **`CNET_ESP32_MAX_POWER()` deletes the task watchdog and disables logs**,
  and releases Bluetooth memory. Call it only when you control the firmware.
- **FCS handling is hardware-dependent.** Frame structs carry `fcs[4]`, but
  `esp_wifi_80211_tx()` normally expects/creates the FCS itself. Verify on
  your target whether you must fill or omit it.

## Protocol coverage

- The protocol structs for ARP, ICMP, ICMPv6, IGMP, IPv6 extension headers,
  SCTP, DHCP, DNS, NTP, TLS and the 802.11 frames are **definitions only**.
  CNET does not implement state machines, parsing, reassembly or
  retransmission for them; you build and interpret the bytes yourself.
- `CNET_SCTP_CSUM()` is the only SCTP helper; chunk sequencing is your job.
- TLS support covers only the record and handshake prefixes.
- DHCP support covers the fixed header, the magic cookie, option codes and
  message types; option encoding is manual.

## Header-specific notes

- **`cnet_ntp.h`**: `struct cnet_ntp` declares `delay` and `disper` as
  `struct cnet_ntp_short`, but the header defines `struct cnet_short`. This
  is inconsistent in the current source.
- **802.11 FCS macros**: `CNET_FRAME_MACROS_FCS32` etc. refer to the `fcs`
  union inside `struct cnet_frame_fcs`, not to the `fcs[4]` array in frame
  structs. Cast the array to `struct cnet_frame_fcs *` when computing it.
- **`cnet_frame/rts.h`** uses `struct cnet_frame_types` (plural), while the
  header defines `struct cnet_frame_type`. This is inconsistent in the
  current source.
- **`cnet_frame_action_na`** has the same layout as `cnet_frame_action`; it
  exists to make the subtype explicit in code.
- **`cnet_init.h` / `cnet_frame/cnet_init_file.h`** are intentionally almost
  empty; they exist so Arduino `.ino` files can include a single header
  before the frame headers inside `extern "C"`.

## Versioning

Current version: (see [VERSION.md](../VERSION.md)).
