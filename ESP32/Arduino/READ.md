## READ

To use CNET library (cnet_frame/*.h) in the Arduino
ALWAYS in the start of your .ino file write this

```c
extern "C" {
  #include <cnet_init.h>
  //After you can include cnet_frame/*.h files
  #include <cnet_frame/probe.h>
  #include <cnet_frame/beacon.h>
}
```