#include "dfplayer.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "DFPLAYER";

// Отправка команды DFPlayer
void dfplayer_send_cmd(uint8_t cmd, uint8_t param1, uint8_t param2) {
    uint8_t cmd_buf[10] = {0};
    
    // Стартовый байт
    cmd_buf[0] = 0x7E;
    // Версия
    cmd_buf[1] = 0xFF;
    // Длина данных (всегда 6)
    cmd_buf[2] = 0x06;
    // Команда
    cmd_buf[3] = cmd;
    // Feedback (0 = нет ответа, 1 = запросить ответ)
    cmd_buf[4] = 0x00;
    // Параметр 1 (старший байт)
    cmd_buf[5] = param1;
    // Параметр 2 (младший байт)
    cmd_buf[6] = param2;
    // Контрольная сумма
    uint16_t checksum = 0;
    for (int i = 1; i <= 6; i++) {
        checksum += cmd_buf[i];
    }
    checksum = -checksum;
    cmd_buf[7] = (checksum >> 8) & 0xFF;  // Старший байт
    cmd_buf[8] = checksum & 0xFF;         // Младший байт
    // Стоповый байт
    cmd_buf[9] = 0xEF;
    
    uart_write_bytes(DFPLAYER_UART_NUM, (const char *)cmd_buf, 10);
    ESP_LOGI(TAG, "Отправлена команда: 0x%02X, params: %d, %d", cmd, param1, param2);
    
    vTaskDelay(pdMS_TO_TICKS(100)); // Небольшая задержка между командами
}

void dfplayer_init(void) {
    // Конфигурация UART
    uart_config_t uart_config = {
        .baud_rate = DFPLAYER_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    uart_driver_install(DFPLAYER_UART_NUM, 1024, 1024, 0, NULL, 0);
    uart_param_config(DFPLAYER_UART_NUM, &uart_config);
    uart_set_pin(DFPLAYER_UART_NUM, DFPLAYER_TX_PIN, DFPLAYER_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    ESP_LOGI(TAG, "DFPlayer Mini инициализирован");
    
    // Начальная громкость (0-30)
    dfplayer_set_volume(15);
}

// Воспроизвести трек из папки (folder: 1-99, track: 1-255)
void dfplayer_play_folder(uint8_t folder, uint8_t track) {
    uint16_t param = (folder << 8) | track;
    dfplayer_send_cmd(DFPLAYER_CMD_PLAY_FOLDER, 0x00, param);
}

// Воспроизвести конкретный трек по номеру (1-3000)
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