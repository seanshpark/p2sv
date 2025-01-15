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

// LCD1602 power control pin
// - LCD1602 power is provided from separate source not consume from PICO
// - this results need to reset LCD with software control
#define LCD1602_PWR_CTRL 10

// LCD1602 I2C
#define LCD1602_I2C_PORT i2c0
#define LCD1602_I2C_SDA 12
#define LCD1602_I2C_SCL 13

#define LCD1602_NUM_LEVELS 16
#define LCD1602_BARS_CHN 8

#endif // __P2SV_CONF_H__