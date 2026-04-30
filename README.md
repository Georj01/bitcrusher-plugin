# Bitcrusher VST3 Plugin

A minimalist, high-performance VST3 Bitcrusher and Decimator audio plugin. It reduces the bit depth of the audio signal and applies sample-and-hold downsampling to generate classic digital distortion and aliasing effects.

## Stack

- C++20
- JUCE Framework (8.0.0)
- CMake

## Installation

```bash
git clone https://github.com/Georj01/bitcrusher-plugin.git
cd bitcrusher-plugin
cmake -B build
cmake --build build --config Release
```

## Usage

1. Copy the generated `.vst3` file from the `build` directory into your DAW's standard VST3 plugin folder.
2. Load the plugin on an audio track.
3. Use the **Bit Depth** knob to quantize the signal amplitude (range: 2 to 32 bits).
4. Use the **Downsample** knob to trigger a sample-and-hold effect, reducing the effective sample rate (range: 1x to 50x).
