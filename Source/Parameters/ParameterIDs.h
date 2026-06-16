#pragma once
#include <juce_core/juce_core.h>

namespace ParameterIDs
{
    // Use inline constexpr so each ID is a single object with no ODR issues
    inline constexpr const char* rate     = "rate";
    inline constexpr const char* depth    = "depth";
    inline constexpr const char* delay    = "delay";
    inline constexpr const char* feedback = "feedback";
    inline constexpr const char* mix      = "mix";
    inline constexpr const char* voices   = "voices";
    inline constexpr const char* stereo   = "stereo";
} // namespace ParameterIDs
