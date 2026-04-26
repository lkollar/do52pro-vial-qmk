#pragma once

#define EE_HANDS

#define SERIAL_USART_TX_PIN GP3

#define PS2_CLOCK_PIN GP23
#define PS2_DATA_PIN GP22
#define PS2_PIO_USE_PIO1
#define PS2_MOUSE_ROTATE 90
#define PS2_MOUSE_INIT_DELAY 2000

#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 0
#define BOOTMAGIC_ROW_RIGHT 6
#define BOOTMAGIC_COLUMN_RIGHT 5

// Keep split transport debug available in `qmk console`.
#define SERIAL_DEBUG
