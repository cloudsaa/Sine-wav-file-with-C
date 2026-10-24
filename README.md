
# sine-wav

A tiny C program that generates a **5-second, 440 Hz sine wave** (the note A4) and writes it to a playable `output.wav` file, with no audio libraries. It writes the WAV header by hand.

This is project #1 in my journey toward building a mini DAW and a collection of audio filters in C.

## What it does

- Builds a 44-byte WAV header from C structs
- Computes 220,500 samples of a sine wave (16-bit, mono, 44.1 kHz)
- Writes header + samples to `output.wav`

The result opens in any audio player or editor.

## Build & run

```bash
gcc wav.c -o sine -lm
./sine
```

This creates `output.wav` in the current folder. On Windows, use MinGW/MSYS2 or WSL with the same commands.

Output check: the file should be **441,044 bytes** (44 header + 441,000 audio data).

## How it works

### 1. The sine wave

Each sample is the wave's height at one instant in time:

```
sample[i] = SAMPLE_MAX * sin(2π · 440 · i / SR)
```

- `i / SR` is the time in seconds of sample number `i`
- `440` is the frequency in Hz (cycles per second)
- `SAMPLE_MAX` (32767) scales the result from -1…1 to the 16-bit integer range
- `lrint()` rounds to the nearest integer

### 2. The WAV file format

A WAV file is a RIFF container made of a header followed by raw samples:

| Part | Size | Contents |
|---|---|---|
| RIFF header | 12 bytes | `"RIFF"`, file size − 8, `"WAVE"` |
| `fmt ` chunk | 24 bytes | format tag (1 = PCM), channels, sample rate, byte rate, block align, bits per sample |
| `data` header | 8 bytes | `"data"`, number of audio bytes |
| Samples | 441,000 bytes | 16-bit signed integers, little-endian |

The RIFF size is `36 + data size`: the 36 comes from everything in the header after the first 8 bytes.

### Settings you can change

| Macro | Meaning |
|---|---|
| `DURATION` | length in seconds |
| `SR` | sample rate (44100 is CD quality) |
| `NCHANNELS` | 1 = mono |
| `440` in the loop | pitch in Hz |

## What I learned

- What a sample rate is, and how sample number maps to time
- How audio is stored as plain numbers, and how a binary file format is laid out
- Using `uint16_t`/`uint32_t` structs to match a file format byte for byte
- Why the loop must be written in floating point: integer division would break the math

## Known limitations / next steps

- [ ] Check that `fopen` succeeded
- [ ] Lower the volume (full-scale output is loud); multiply by something like `0.5`
- [ ] Take frequency, duration and output name from command-line arguments
- [ ] Support stereo
- [ ] Add a Python script that plots the first few milliseconds of the wave
- [ ] Add other waveforms (square, saw, triangle), the next project

## Notes

- The header struct has no padding on common compilers, so it is exactly 44 bytes.
- The code assumes a little-endian machine (x86 and ARM), which matches WAV.

## Credits

Based on the WAV tutorial at <https://mu.krj.st/wave/> from *projet μ*, a free introduction to audio programming and DSP in C. The code here is my own implementation and notes while following along.

## License

MIT