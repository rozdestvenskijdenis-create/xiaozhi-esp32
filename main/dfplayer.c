#include "dfplayer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DFPLAYER";

// Команды DFPlayer Mini
#define DFPLAYER_CMD_PLAY_FOLDER  0x0F
#define DFPLAYER_CMD_STOP         0x16
#define DFPLAYER_CMD_NEXT         0x01
#define DFPLAYER_CMD_PREV         0x02
#define DFPLAYER_CMD_VOLUME       0x06
#define DFPLAYER_CMD_PLAY         0x0D
#define DFPLAYER_CMD_PAUSE        0x0E

void dfplayer_send_cmd(uint8_t cmd, uint8_t param1, uint8_t param2) {
    uint8_t cmd_buf[10] = {0};
    
    cmd_buf[0] = 0x7E;
    cmd_buf[1] = 0xFF;
    cmd_buf[2] = 0x06;
    cmd_buf[3] = cmd;
    cmd_buf[4] = 0x00;
    cmd_buf[5] = param1;
    cmd_buf[6] = param2;
    
    uint16_t checksum = 0;
    for (int i = 1; i <= 6; i++) {
        checksum += cmd_buf[i];
    }
    checksum = -checksum;
    cmd_buf[7] = (checksum >> 8) & 0xFF;
    cmd_buf[8] = checksum & 0xFF;
    
    cmd_buf[9] = 0xEF;
    
    uart_write_bytes(DFPLAYER_UART_NUM, (const char *)cmd_buf, 10);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void dfplayer_init(void) {
    uart_config_t uart_config = {
        .baud_rate = DFPLAYER_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    uart_param_config(DFPLAYER_UART_NUM, &uart_config);
    uart_set_pin(DFPLAYER_UART_NUM, DFPLAYER_TX_PIN, DFPLAYER_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(DFPLAYER_UART_NUM, 1024, 1024, 0, NULL, 0);
    
    ESP_LOGI(TAG, "DFPlayer инициализирован");
    dfplayer_set_volume(15);
    vTaskDelay(pdMS_TO_TICKS(500));
}

void dfplayer_play_folder(uint8_t folder, uint8_t track) {
    uint16_t param = (folder << 8) | track;
    dfplayer_send_cmd(DFPLAYER_CMD_PLAY_FOLDER, 0x00, param);
}

void dfplayer_play_track(uint16_t track) {
    dfplayer_send_cmd(DFPLAYER_CMD_PLAY, 0x00, track);
}

void dfplayer_stop(void) {
    dfplayer_send_cmd(DFPLAYER_CMD_STOP, 0x00, 0x00);
}

void dfplayer_next(void) {
    dfplayer_send_cmd(DFPLAYER_CMD_NEXT, 0x00, 0x00);
}

void dfplayer_prev(void) {
    dfplayer_send_cmd(DFPLAYER_CMD_PREV, 0x00, 0x00);
}

void dfplayer_set_volume(uint8_t volume) {
    if (volume > 30) volume = 30;
    dfplayer_send_cmd(DFPLAYER_CMD_VOLUME, 0x00, volume);
}
