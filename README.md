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

Youtube demo video with LCD 16 characters 2 lines:

[![demo video](http://i.ytimg.com/vi/k9Akgbx1h0w/hqdefault.jpg)](https://www.youtube.com/watch?v=k9Akgbx1h0w)


Youtube demo video with OLED 128x64:

[![YouTube](http://i.ytimg.com/vi/okPapBplkcM/hqdefault.jpg)](https://www.youtube.com/watch?v=okPapBplkcM)

## SDK version

This projectd uses Pico SDK 2.1.0.
