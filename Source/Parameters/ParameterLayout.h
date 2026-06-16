#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace Parameters
{
    /** Creates and returns the AudioProcessorValueTreeState parameter layout. */
    juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
} // namespace Parameters
