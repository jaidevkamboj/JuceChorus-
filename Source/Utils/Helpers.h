#pragma once
#include <cmath>
#include <juce_core/juce_core.h>

namespace Helpers
{
    /** Linear interpolation. */
    template <typename T>
    inline T lerp (T a, T b, T t) noexcept { return a + t * (b - a); }

    /** Clamp value to [lo, hi]. */
    template <typename T>
    inline T clamp (T v, T lo, T hi) noexcept { return v < lo ? lo : v > hi ? hi : v; }

    /** Convert milliseconds to samples. */
    inline int msToSamples (float ms, double sampleRate) noexcept
    {
        return static_cast<int> (ms * 0.001 * sampleRate);
    }

    /** Convert samples to milliseconds. */
    inline float samplesToMs (int samples, double sampleRate) noexcept
    {
        return static_cast<float> (samples) / static_cast<float> (sampleRate) * 1000.0f;
    }

    /** Map a normalised [0,1] value to decibels in range [minDb, maxDb]. */
    inline float normToDb (float norm, float minDb = -60.0f, float maxDb = 0.0f) noexcept
    {
        return minDb + norm * (maxDb - minDb);
    }

    /** Smoothstep easing (useful for animating GUI elements). */
    inline float smoothstep (float edge0, float edge1, float x) noexcept
    {
        x = clamp ((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return x * x * (3.0f - 2.0f * x);
    }
} // namespace Helpers
