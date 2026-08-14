#ifndef DFPLAYER_H
#define DFPLAYER_H

#include "driver/uart.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Пины UART (измените под вашу плату если нужно)
#define DFPLAYER_UART_NUM    UART_NUM_1
#define DFPLAYER_TX_PIN      17
#define DFPLAYER_RX_PIN      16
#define DFPLAYER_BAUD_RATE   9600

void dfplayer_init(void);
void dfplayer_play_folder(uint8_t folder, uint8_t track);
void dfplayer_play_track(uint16_t track);
void dfplayer_stop(void);
void dfplayer_next(void);
void dfplayer_prev(void);
void dfplayer_set_volume(uint8_t volume);
void dfplayer_send_cmd(uint8_t cmd, uint8_t param1, uint8_t param2);

#ifdef __cplusplus
}
#endif

#endif
