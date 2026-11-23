#ifndef CNET_HPP
#define CNET_HPP

#include <cnet_platform.h>

#if defined(CNET_PLATFORM_ESP32)
  #include <cnet.h>

  #include <stdint.h>
  #include <stdbool.h>
  #include <esp_err.h>
  #include <esp_wifi.h>

  #ifdef __cplusplus
  extern "C" {
  #endif

  void CNET_ESP32_IWIP_INIT();
  void CNET_ESP32_MAX_POWER(uint8_t channel);
  cnet_errno_t CNET_GET_TCP_INFO(int socket, struct cnet_tcp_info *info);

  static inline esp_err_t CNET_BURST(struct cnet_buffer *buff, uint8_t mode, bool switcher)
  {
      return esp_wifi_80211_tx((wifi_interface_t)mode, buff->buff, buff->size, switcher);
  }

  #ifdef __cplusplus
  }
  #endif

#endif
#endif