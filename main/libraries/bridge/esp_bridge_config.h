/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Maximum MAC size
 *
 */
#define BRIDGE_MAC_MAX_LEN     (6)

/**
 * @brief Maximum SSID size
 *
 */
#define BRIDGE_SSID_MAX_LEN    (32)

#ifdef CONFIG_BRIDGE_SOFTAP_MAX_CONNECT_NUMBER
#define BRIDGE_SOFTAP_MAX_CONNECT_NUMBER CONFIG_BRIDGE_SOFTAP_MAX_CONNECT_NUMBER
#else
#define BRIDGE_SOFTAP_MAX_CONNECT_NUMBER 8
#endif

#if CONFIG_BRIDGE_SOFTAP_SSID_END_WITH_THE_MAC
#define SSID_MAC_LEN    7  // _XXYYZZ
#else
#define SSID_MAC_LEN    0
#endif

#if defined(CONFIG_BRIDGE_SOFTAP_SSID) && defined(CONFIG_BRIDGE_SOFTAP_PASSWORD)
#define STATIC_ASSERT(condition) typedef char p__LINE__[ (condition) ? 1 : -1];
STATIC_ASSERT((sizeof(CONFIG_BRIDGE_SOFTAP_SSID) + SSID_MAC_LEN) < (32 + 2))
STATIC_ASSERT(sizeof(CONFIG_BRIDGE_SOFTAP_PASSWORD) < (63 + 2))
#endif

enum {
    BRIDGE_EXTERNAL_NETIF_INVALID = -1,
#ifdef CONFIG_BRIDGE_EXTERNAL_NETIF_STATION
    BRIDGE_EXTERNAL_NETIF_STATION,
#endif
#ifdef CONFIG_BRIDGE_EXTERNAL_NETIF_MODEM
    BRIDGE_EXTERNAL_NETIF_MODEM,
#endif
#ifdef CONFIG_BRIDGE_EXTERNAL_NETIF_ETHERNET
    BRIDGE_EXTERNAL_NETIF_ETHERNET,
#endif
    BRIDGE_EXTERNAL_NETIF_MAX
};

#ifdef __cplusplus
}
#endif
