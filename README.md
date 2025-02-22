# p2sv

`p2sv` is simple project to use Raspberry Pi pico 2 as an audio spectrum visualizer.

`p2sv` uses [kissfft](https://github.com/mborgerding/kissfft) for as FFT and
some algorithms from [cava](https://github.com/karlstav/cava) to get basic visualization.

Two ADCs are used to digitize stereo audio signals with about 44.1KHz sampling rate.

Test connection on breadboard: 

<img src="docs/p2sv_capture.png">

Audio signals need level shift to positve voltage for input to ADC.

Used this simple circuit to do so:

<img src="docs/audio_adc_level.png">

Pico pins are 3.3V but some LCD1602 and LED dot matrix uses 5V.

To shift signal voltage, used this circuit to do so:

<img src="docs/i2c_level_shifter.png">

Youtube demo video with LCD 16 characters 2 lines:

[![LCD1602](http://i.ytimg.com/vi/k9Akgbx1h0w/hqdefault.jpg)](https://www.youtube.com/watch?v=k9Akgbx1h0w)
- I2C0 SDA: GP12
- I2C0 SCL: GP13
- some LCD works on 5V so need to use 2 level shifter for each pins

Youtube demo video with OLED 128x64:

[![OLED128x64](http://i.ytimg.com/vi/okPapBplkcM/hqdefault.jpg)](https://www.youtube.com/watch?v=okPapBplkcM)
- I2C0 SDA: GP12
- I2C0 SCL: GP13

Youtube demo video with LED 32x8:

[![LED32x8](http://i.ytimg.com/vi/Sz2ba43FdY0/hqdefault.jpg)](https://www.youtube.com/watch?v=Sz2ba43FdY0)
- MAX7219 DIO: GP2
- MAX7219 CS : GP3
- MAX7219 CLK: GP4
- this dot matrix works on 5V so need to use 3 level shifter for each pins

## SDK version

This projectd uses Pico SDK 2.1.0.
