cmake_minimum_required(VERSION 3.22)
project(JuceChorusPlugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── JUCE ──────────────────────────────────────────────────────────────────────
include(FetchContent)

if(DEFINED JUCE_PATH AND EXISTS "${JUCE_PATH}/CMakeLists.txt")
    message(STATUS "Using local JUCE at ${JUCE_PATH}")
    add_subdirectory("${JUCE_PATH}" JUCE)
else()
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        7.0.12
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(JUCE)
endif()

# ── Platform-aware format list ─────────────────────────────────────────────────
# AU is macOS/iOS only — including it on Windows causes MSB3073 linker errors.
if(APPLE)
    set(PLUGIN_FORMATS AU VST3 Standalone)
else()
    set(PLUGIN_FORMATS VST3 Standalone)
endif()

# ── Plugin target ─────────────────────────────────────────────────────────────
juce_add_plugin(ChorusPlugin
    COMPANY_NAME             "YourCompany"
    PLUGIN_MANUFACTURER_CODE YrCo
    PLUGIN_CODE              Chrs
    FORMATS                  ${PLUGIN_FORMATS}
    PRODUCT_NAME             "Chorus Plugin"
    IS_SYNTH                 FALSE
    NEEDS_MIDI_INPUT         FALSE
    NEEDS_MIDI_OUTPUT        FALSE
    IS_MIDI_EFFECT           FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    # COPY_PLUGIN_AFTER_BUILD requires admin rights on Windows (writes to
    # C:\Program Files\Common Files\VST3). Disabled to avoid MSB3073.
    # Built artefacts are in: Build/ChorusPlugin_artefacts/<Debug|Release>/VST3/
    COPY_PLUGIN_AFTER_BUILD  FALSE
)

# ── Source files ──────────────────────────────────────────────────────────────
target_sources(ChorusPlugin PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/DSP/ChorusProcessor.cpp
    Source/DSP/ChorusProcessor.h
    Source/Parameters/ParameterIDs.h
    Source/Parameters/ParameterLayout.cpp
    Source/Parameters/ParameterLayout.h
    Source/GUI/LookAndFeel.cpp
    Source/GUI/LookAndFeel.h
    Source/GUI/RotaryKnob.cpp
    Source/GUI/RotaryKnob.h
    Source/Utils/Constants.h
    Source/Utils/Helpers.h
)

# ── Compile definitions ───────────────────────────────────────────────────────
target_compile_definitions(ChorusPlugin PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# ── MSVC-specific flags ────────────────────────────────────────────────────────
if(MSVC)
    target_compile_options(ChorusPlugin PRIVATE
        /W3     # reasonable warning level
        /MP     # parallel file compilation
        /EHsc   # standard C++ exception model
    )
    target_compile_definitions(ChorusPlugin PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        NOMINMAX   # prevent Windows.h min/max macros clashing with std::
    )
endif()

# ── JUCE modules ──────────────────────────────────────────────────────────────
target_link_libraries(ChorusPlugin PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()
add_executable(ChorusTests Tests/ChorusTests.cpp)
target_link_libraries(ChorusTests PRIVATE
    juce::juce_audio_processors
    juce::juce_dsp
)
add_test(NAME ChorusTests COMMAND ChorusTests)
