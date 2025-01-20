#ifndef __P2SV_CONF_H__
#define __P2SV_CONF_H__

// ADC pin left and right
#define ADC_CH_0 26 // GPIO26
#define ADC_CH_1 27 // GPIO27

// this will make about 44100 samples / sec
#define ADC_CLOCK_DIV 1088
// sampleing frequence is 48MHz, to get 44.1KHz
// 48,000,000 / 1088 = about 44,118
#define SAMPLE_RATE 44118

// sample about 20msec -> 44,118 * 20 msec -> 882
#define N_SAMPLE 882

// DISPLAY power control pin
// - DISPLAY power is provided from separate source not consume from PICO
// - this results need to reset LCD with software control
#define DISPLAY_PWR_CTRL 10

// I2C pins for display
#define DISPLAY_I2C_PORT i2c0
#define DISPLAY_I2C_SDA 12
#define DISPLAY_I2C_SCL 13

// MAX7219 pins
#define MAX7219_DIO 2
#define MAX7219_CS 3
#define MAX7219_CLK 4

#endif // __P2SV_CONF_H__
