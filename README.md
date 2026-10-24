
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

- **How digital audio represents sound:** Audio can be represented as a sequence of numerical samples, each describing the amplitude of a waveform at a particular instant. In this project, I generate those values mathematically using a sine function rather than recording an existing sound.

- **What the sample rate means:** The sample rate determines how many samples are generated per second. With a sample rate of 44,100 Hz, each sample corresponds to a time interval of approximately 1/44,100 of a second.

- **How a mathematical function becomes audio data:** I use a sine function to generate a 440 Hz tone. Each iteration of the loop calculates the waveform's amplitude at a particular sample index, scales it to fit within a 16-bit integer's range, and rounds it to an integer.

- **How binary files store numbers:** The audio samples are stored as binary values rather than as human-readable text. Using `fwrite()`, I write the contents of the sample buffer directly to the WAV file.

- **How the WAV/RIFF file format is structured:** A WAV file uses the RIFF container format. Its header contains identifiers such as `RIFF` and `WAVE`, followed by metadata chunks such as `fmt ` and `data`. These fields tell audio software how to interpret the bytes that follow.

- **How C structures represent file-format fields:** I use nested structures to organise the RIFF header, format chunk, and data header. Fixed-width integer types such as `uint16_t` and `uint32_t` help represent fields with specific bit widths, while `sizeof()` calculates the sizes of types and objects.

- **How file sizes are calculated:** The RIFF size field counts the bytes following the first eight bytes of the file. For this standard PCM layout, the header contributes 36 bytes to that count, and the audio data contributes `NSAMPLES * sizeof(sample_t)` bytes.

- **Why floating-point arithmetic matters:** The sine function requires floating-point calculations. Using floating-point arithmetic preserves the fractional values needed to calculate the waveform accurately; integer division at the wrong point would discard those fractions and distort the result.

- **How digital data becomes physical sound:** The WAV file contains the digital representation of the waveform, not sound itself. An audio player interprets the samples and sends them through an audio system, where a digital-to-analog converter and speaker can turn them into physical vibrations.

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