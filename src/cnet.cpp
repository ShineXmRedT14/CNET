#include <cnet.hpp>

#if defined(CNET_PLATFORM_ESP32)
  #include <lwip/api.h>
  #include <lwip/sockets.h>
  #include <lwip/priv/tcp_priv.h>

  #include <stddef.h>
  #include <stdbool.h>
  #include <string.h>

  #include <WiFi.h>
  #include "soc/soc.h"
  #include "soc/rtc_cntl_reg.h"
  #include <esp_wifi.h>
  #include <esp_log.h>
  #include <esp_err.h>
  #include <nvs_flash.h>
  #include <esp_event.h>
  #include <esp_task_wdt.h>
  #include <esp_bt.h>

  cnet_errno_t CNET_GET_TCP_INFO(int socket, struct cnet_tcp_info *info)
  {
    if (info == NULL) {
        return CNET_ERR_SOCKET;
    }

    CNET_MEMSET(info, sizeof(*info));

    if (socket < 0) {
        return CNET_ERR_SOCKET;
    }

    struct lwip_sock *lwip = lwip_socket_dbg_get_socket(socket);
    if (lwip == NULL || lwip->conn == NULL) {
        return CNET_ERR_SOCKET;
    }

    if (NETCONNTYPE_GROUP(netconn_type(lwip->conn)) != NETCONN_TCP) {
        return CNET_ERR_SOCKET;
    }

    struct tcp_pcb *pcb = lwip->conn->pcb.tcp;
    if (pcb == NULL) {
        return CNET_TCP_INFO_BAD_CONNECTION;
    }

    const uint32_t tick_us = (uint32_t)TCP_SLOW_INTERVAL * 1000U;

    info->state = (uint8_t)pcb->state;
    info->cwnd = (uint32_t)pcb->cwnd;
    info->rtt = (pcb->sa > 0) ? (uint32_t)pcb->sa * tick_us : 0U;
    info->rttv = (pcb->sv > 0) ? (uint32_t)pcb->sv * tick_us : 0U;
    info->smss = (uint32_t)pcb->mss;
    info->rmss = (uint32_t)pcb->mss;
    info->rto = (pcb->rto > 0) ? (uint32_t)pcb->rto * tick_us : 0U;
    info->sth = (uint32_t)pcb->ssthresh;
    info->off = pcb->nrtx;
    info->options = (uint8_t)(pcb->flags & 0xFFU);

    #if defined(LWIP_WND_SCALE) && LWIP_WND_SCALE
        info->sscale = pcb->snd_scale;
        info->rscale = pcb->rcv_scale;
    #endif

    uint32_t unacked = 0;
    for (struct tcp_seg *seg = pcb->unacked; seg != NULL; seg = seg->next) {
        unacked++;
    }
    info->unack = unacked;

    if (pcb->flags & TF_INFR) {
        info->cnet_state_ca = CNET_TCP_RECOV;
    } else if (pcb->nrtx > 0) {
        info->cnet_state_ca = CNET_TCP_LOSS;
    } else {
        info->cnet_state_ca = CNET_TCP_OPEN;
    }

    return CNET_SCFL;
  }

  void CNET_ESP32_MAX_POWER(uint8_t channel)
  {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    esp_bt_controller_mem_release((esp_bt_mode_t)ESP_BT_MODE_BTDM);
    CNET_ESP32_IWIP_INIT();

    esp_wifi_set_max_tx_power(80);
    setCpuFrequencyMhz(240);
    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N);
    esp_task_wdt_delete(NULL);
    esp_log_level_set("*", ESP_LOG_NONE);
    esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT20);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  }

  void CNET_ESP32_IWIP_INIT()
  {
    esp_err_t nvserr = nvs_flash_init();

    if (nvserr == ESP_ERR_NVS_NO_FREE_PAGES || nvserr == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      vTaskDelay(pdMS_TO_TICKS(250));
      ESP_ERROR_CHECK(nvs_flash_erase());
      nvserr = nvs_flash_init();
    }

    ESP_ERROR_CHECK(nvserr);
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    esp_err_t esperr = esp_wifi_start();
    if (esperr != ESP_OK)
    {
      ESP.restart();
    }
  }

#endif