# JuceChorusPlugin

A polished, production-ready chorus audio plug-in built with [JUCE](https://juce.com/).

## Features

- Stereo BBD-style chorus with multiple voices
- Rate, Depth, Delay, Mix, and Feedback controls
- Custom look-and-feel with smooth rotary knobs
- Parameter automation & preset support
- Builds as **AU**, **VST3**, and **Standalone**

## Requirements

| Tool | Minimum version |
|------|----------------|
| CMake | 3.22 |
| JUCE | 7.x |
| C++ | 17 |
| Xcode (macOS) | 14+ |
| Visual Studio (Windows) | 2022 |

## Building

```bash
# 1. Clone JUCE alongside this repo
git clone https://github.com/juce-framework/JUCE.git

# 2. Configure
cmake -B Build -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build Build --config Release
```

Plug-in binaries will be copied to your system's standard plug-in folders automatically.

## Project Structure

```
JuceChorusPlugin/
├── CMakeLists.txt
├── Source/
│   ├── PluginProcessor.*     # AudioProcessor subclass + APVTS
│   ├── PluginEditor.*        # Main editor window
│   ├── DSP/
│   │   └── ChorusProcessor.* # Core chorus DSP engine
│   ├── Parameters/
│   │   ├── ParameterIDs.h    # Compile-time parameter ID strings
│   │   └── ParameterLayout.* # APVTS layout factory
│   ├── GUI/
│   │   ├── LookAndFeel.*     # Custom JUCE LookAndFeel
│   │   └── RotaryKnob.*      # Reusable knob component
│   └── Utils/
│       ├── Constants.h       # Shared numeric constants
│       └── Helpers.h         # Inline utility functions
├── Tests/
│   └── ChorusTests.cpp       # Unit tests
└── Assets/
    ├── Images/
    ├── Fonts/
    └── Presets/
```

## License

JAIDEV KAMBOJ © YourCompany
