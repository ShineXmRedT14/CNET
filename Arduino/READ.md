## READ

To use CNET library on the Arduino
ALWAYS in the start of your .ino file write this

```c
#include <cnet_init.h>
//After you can include other files
#include <cnet_frame/probe.h>
#include <cnet_arp.h>
// ...
```