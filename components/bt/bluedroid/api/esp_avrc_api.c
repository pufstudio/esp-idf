// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bt_target.h"
#include <string.h>
#include "esp_err.h"
#include "esp_avrc_api.h"
#include "esp_bt_main.h"
#include "btc_manage.h"
#include "btc_avrc.h"

#if BTC_AV_INCLUDED

esp_err_t esp_avrc_ct_register_callback(esp_avrc_ct_cb_t callback)
{
    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        return ESP_ERR_INVALID_STATE;
    }

    if (callback == NULL) {
        return ESP_FAIL;
    }

    btc_profile_cb_set(BTC_PID_AVRC, callback);
    return ESP_OK;
}

esp_err_t esp_avrc_ct_init(void)
{
    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        return ESP_ERR_INVALID_STATE;
    }

    btc_msg_t msg;

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_AVRC;
    msg.act = BTC_AVRC_CTRL_API_INIT_EVT;

    /* Switch to BTC context */
    bt_status_t stat = btc_transfer_context(&msg, NULL, 0, NULL);
    return (stat == BT_STATUS_SUCCESS) ? ESP_OK : ESP_FAIL;
}

esp_err_t esp_avrc_ct_deinit(void)
{
    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        return ESP_ERR_INVALID_STATE;
    }

    btc_msg_t msg;

    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_AVRC;
    msg.act = BTC_AVRC_CTRL_API_DEINIT_EVT;

    /* Switch to BTC context */
    bt_status_t stat = btc_transfer_context(&msg, NULL, 0, NULL);
    return (stat == BT_STATUS_SUCCESS) ? ESP_OK : ESP_FAIL;
}

/* Request metadata from CT */
esp_err_t esp_avrc_ct_send_metadata_cmd(uint8_t tl, uint32_t* attr_list, uint8_t attr_num)
{
    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        return ESP_ERR_INVALID_STATE;
    }

    btc_msg_t msg;
    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_AVRC;
    msg.act = BTC_AVRC_STATUS_API_SND_META_EVT;

    btc_avrc_args_t arg;
    memset(&arg, 0, sizeof(btc_avrc_args_t));

    arg.md_cmd.tl = tl;
    arg.md_cmd.attr_list = attr_list;
    arg.md_cmd.attr_num = attr_num;

    /* Switch to BTC context */
    bt_status_t stat = btc_transfer_context(&msg, &arg, sizeof(btc_avrc_args_t), NULL);
    return (stat == BT_STATUS_SUCCESS) ? ESP_OK : ESP_FAIL;
}

esp_err_t esp_avrc_ct_send_passthrough_cmd(uint8_t tl, uint8_t key_code, uint8_t key_state)
{
    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        return ESP_ERR_INVALID_STATE;
    }

    btc_msg_t msg;
    msg.sig = BTC_SIG_API_CALL;
    msg.pid = BTC_PID_AVRC;
    msg.act = BTC_AVRC_CTRL_API_SND_PTCMD_EVT;

    btc_avrc_args_t arg;
    memset(&arg, 0, sizeof(btc_avrc_args_t));

    arg.pt_cmd.tl = tl;
    arg.pt_cmd.key_code = key_code;
    arg.pt_cmd.key_state = key_state;

    /* Switch to BTC context */
    bt_status_t stat = btc_transfer_context(&msg, &arg, sizeof(btc_avrc_args_t), NULL);
    return (stat == BT_STATUS_SUCCESS) ? ESP_OK : ESP_FAIL;
}

#endif /* #if BTC_AV_INCLUDED */
