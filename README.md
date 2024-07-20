# FLAC Decoder

Port of: https://www.nayuki.io/res/simple-flac-implementation/simple-decode-flac-to-wav.py

Uses some small parts from: https://github.com/schreibfaul1/ESP32-audioI2S/


## Building and Running

``` sh
make
./main < input.flac > output.raw
```

FLAC data is read from `stdin` and raw audio samples are written to `stdout`.

Audio samples are always 16-bit signed integers, but you need to know the sample rate and number of channels from the FLAC file in order to play the raw audio.
For example:

``` sh
ffprobe input.flac

[lots of info]
Stream #0:0: Audio: flac, 48000 Hz, stereo, s16
```

With this information, you can convert the raw audio to a WAV file:

``` sh
sox -r 48000 -c 2 -b 16 -e signed-integer -t raw output.raw output.wav
```


## Customizing

Adjust the code in `fill_buffer()` to get data from the appropriate source (currently `stdin`).

Make sure to keep the same logic of copying the unread chunk at the end of the buffer to the beginning before filling the rest with new FLAC data.
