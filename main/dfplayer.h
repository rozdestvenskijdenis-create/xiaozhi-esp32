#ifndef DFPLAYER_H
#define DFPLAYER_H

#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Пины UART
#define DFPLAYER_UART_NUM    UART_NUM_1
#define DFPLAYER_TX_PIN      17
#define DFPLAYER_RX_PIN      16
#define DFPLAYER_BAUD_RATE   9600

// Команды DFPlayer
#define DFPLAYER_CMD_PLAY_FOLDER  0x0F  // Воспроизвести из папки
#define DFPLAYER_CMD_STOP         0x16  // Стоп
#define DFPLAYER_CMD_NEXT         0x01  // Следующий трек
#define DFPLAYER_CMD_PREV         0x02  // Предыдущий трек
#define DFPLAYER_CMD_VOLUME       0x06  // Громкость (0-30)
#define DFPLAYER_CMD_PLAY         0x0D  // Воспроизвести
#define DFPLAYER_CMD_PAUSE        0x0E  // Пауза

void dfplayer_init(void);
void dfplayer_play_folder(uint8_t folder, uint8_t track);
void dfplayer_play_track(uint16_t track);
void dfplayer_stop(void);
void dfplayer_next(void);
void dfplayer_prev(void);
void dfplayer_set_volume(uint8_t volume);
void dfplayer_send_cmd(uint8_t cmd, uint8_t param1, uint8_t param2);

#endif