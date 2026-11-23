#ifndef CNET_PLATFORM_H
#define CNET_PLATFORM_H

#if !defined(CNET_PLATFORM_ESP32) && \
    !defined(CNET_PLATFORM_LINUX) && \
    !defined(CNET_PLATFORM_WINDOWS)

    #if defined(_WIN32)
        #define CNET_PLATFORM_WINDOWS
    #elif defined(__linux__)
        #define CNET_PLATFORM_LINUX
    #elif defined(ESP32) || \
          defined(ESP_PLATFORM) || \
          defined(ARDUINO_ARCH_ESP32)
        #define CNET_PLATFORM_ESP32
    #else
    #error "CNET-Platform declaration error. Please, select by yourself in the CNET/headers/cnet_platform.h file"
    #endif
#endif

#if (defined(CNET_PLATFORM_ESP32) + \
    defined(CNET_PLATFORM_WINDOWS) + \
    defined(CNET_PLATFORM_LINUX)) > 1
#error "Multiple platforms defined simultaneously!"
#endif

#endif