# Installing CNET

CNET ships as source: headers in `headers/` and two source files in `src/`.
You can either run the platform installer or compile the sources directly.

```
CNET/
├── headers/
├── src/cnet.c
├── src/cnet.cpp        (ESP32 only)
└── installers/
    ├── cnet_linux.sh
    ├── cnet_win.bat
    └── cnet_esp32.md
```

---

## Linux

### Automated install

```bash
chmod +x installers/cnet_linux.sh
sudo ./installers/cnet_linux.sh
```

Requirements: `bash`, `gcc` (or `build-essential`), write access to the
prefix (use `sudo`).

What the script does:

1. resolves its own directory, so it works from any working directory;
2. checks `headers/cnet.h` and `src/cnet.c` exist;
3. copies `headers/` into `/usr/local/include`;
4. compiles `src/cnet.c` with `-shared -fPIC -O2 -DNDEBUG -I<headers>` into
   `/usr/local/lib/libcnet.so`;
5. creates the symlink `/usr/local/lib/cnet.so`;
6. writes `/etc/ld.so.conf.d/cnet.conf` and runs `ldconfig` when possible.

Choose another prefix:

```bash
sudo PREFIX=/opt/cnet ./installers/cnet_linux.sh
```

If the prefix is not on the loader path, add it manually:

```bash
export LD_LIBRARY_PATH=/opt/cnet/lib:$LD_LIBRARY_PATH
```

### Use it

```bash
gcc program.c -I/usr/local/include -L/usr/local/lib -lcnet -o program
```

### Manual install

```bash
sudo mkdir -p /usr/local/include /usr/local/lib
sudo cp -r headers/. /usr/local/include/
gcc -shared -fPIC -O2 -Iheaders src/cnet.c -o /usr/local/lib/libcnet.so
sudo ldconfig
```

---

## Windows

### Prerequisites

1. **Npcap runtime** — install from <https://npcap.com/> and enable
   **WinPcap API-compatible Mode**. The installer expects
   `%SystemRoot%\System32\Npcap\wpcap.dll`.
2. **Npcap SDK** — extract it anywhere. The installer looks for
   `Include\pcap\pcap.h` (or `Include\pcap.h`) in:
   - `%NPCAP_SDK%` (if you set it),
   - `%ProgramFiles%\Npcap SDK`,
   - `%ProgramFiles(x86)%\Npcap SDK`,
   - `C:\npcap-sdk`, `C:\Npcap SDK`,
   - `%USERPROFILE%\Desktop\npcap-sdk`, `%USERPROFILE%\Downloads\npcap-sdk`,
   - the repository root and its parent.
3. **MinGW-w64 GCC** on `PATH`.
4. **Administrator** terminal.

### Automated install

```bat
installers\cnet_win.bat
```

The script re-launches itself elevated if needed. It then:

1. checks the Npcap runtime (`wpcap.dll`);
2. locates the SDK and selects `Lib\x64` for 64-bit GCC, otherwise `Lib`;
3. copies `headers\` to `%ProgramFiles%\CNET\include`;
4. builds `%ProgramFiles%\CNET\lib\cnet.dll` with the Npcap import libraries
   and `-liphlpapi -lws2_32`;
5. adds `%ProgramFiles%\CNET\lib` and the Npcap runtime directory to the
   machine `PATH` (idempotently, via the registry).

Open a **new** terminal so the updated `PATH` is visible.

### Use it

```bat
gcc program.c -I"%ProgramFiles%\CNET\include" ^
  -I"%NPCAP_SDK%\Include" ^
  "%ProgramFiles%\CNET\lib\cnet.dll" ^
  -o program.exe
```

If you link the sources directly instead of the DLL:

```bat
gcc program.c src\cnet.c -Iheaders -I"%NPCAP_SDK%\Include" ^
  "%NPCAP_SDK%\Lib\x64\wpcap.lib" "%NPCAP_SDK%\Lib\x64\Packet.lib" ^
  -liphlpapi -lws2_32 -o program.exe
```

### Notes

- `cnet.h` contains `#pragma comment(lib, "iphlpapi.lib")` and
  `#pragma comment(lib, "ws2_32.lib")`. These are MSVC-only; MinGW needs the
  explicit `-l` flags above.
- `cnet.h` includes `<pcap/pcap.h>`. If your SDK only has `<pcap.h>`, add the
  matching include directory or adjust the include.
- Uninstall: delete `%ProgramFiles%\CNET` and remove its `lib` directory and
  the Npcap runtime directory from `PATH` if you no longer need them.

---

## ESP32

There is no installer. CNET is packaged as a normal Arduino library or an
ESP-IDF component.

### Arduino

Create this layout under your Arduino `libraries` directory:

```text
libraries/CNET/
├── library.properties
├── READ.md
└── src/
    ├── cnet.h
    ├── cnet.hpp
    ├── cnet_*.h
    ├── cnet_frame/*.h
    ├── cnet.c
    └── cnet.cpp
```

In `.ino` files, include `cnet_init.h` before any `cnet_frame/*` header and
wrap everything in `extern "C"`:

```cpp
extern "C" {
  #include <cnet_init.h>
  #include <cnet_frame/probe.h>
  #include <cnet_frame/beacon.h>
}
```

Include `cnet.hpp` (C++) for the ESP32-specific functions
(`CNET_ESP32_IWIP_INIT`, `CNET_ESP32_MAX_POWER`, `CNET_BURST`,
`CNET_GET_TCP_INFO`).

### ESP-IDF

1. Add `headers/` to the component include path.
2. Compile both `src/cnet.c` and `src/cnet.cpp`.
3. Provide the ESP-IDF Wi-Fi/lwIP headers and the `esp_wifi` component.

### Notes

- Use `CNET_ESP32_MAX_POWER(channel)` to prepare the radio for raw 802.11
  transmission.
- FCS handling is normally done by the hardware; check your target before
  filling `fcs[4]`.
- See [`docs/platforms.md`](docs/platforms.md#esp32) and
  [`extension/src/INSTALL.md`](extension/src/INSTALL.md).

---

## Verifying the installation

A minimal compile check:

```c
#include <cnet.h>

int main(void)
{
    struct cnet_tcp tcp = {0};
    tcp.dst = CNET_BIG16(CNET_PORT_HTTP);
    return CNET_BIG16(tcp.dst) == CNET_PORT_HTTP ? 0 : 1;
}
```

```bash
gcc check.c -Iheaders src/cnet.c -o check && ./check
```

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| `cnet.h: No such file or directory` | `headers/` is not on the include path |
| `pcap/pcap.h: No such file or directory` | Npcap SDK include path missing, or SDK uses `pcap.h` |
| `undefined reference to pcap_*` | `wpcap.lib` / `Packet.lib` not linked |
| `undefined reference to sendmmsg` | old glibc or missing `_GNU_SOURCE` (the header sets it) |
| `Operation not permitted` on Linux | raw sockets need root / `CAP_NET_RAW` |
| `wpcap.dll` not found at runtime | Npcap runtime not installed or not on `PATH` |

More details in [docs/caveats.md](docs/caveats.md).
