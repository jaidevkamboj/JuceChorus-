#pragma once

namespace Constants
{
    // ── DSP ──────────────────────────────────────────────────────────────────
    inline constexpr int   kMaxDelayLineSamples = 96000;   // 2 s @ 48 kHz
    inline constexpr int   kMaxVoices           = 4;
    inline constexpr float kMinDelayMs          = 1.0f;
    inline constexpr float kMaxDelayMs          = 30.0f;
    inline constexpr float kDefaultDelayMs      = 7.0f;
    inline constexpr float kLfoPhaseOffsetDeg   = 90.0f;  // stereo spread

    // ── GUI ──────────────────────────────────────────────────────────────────
    inline constexpr int kEditorWidth  = 600;
    inline constexpr int kEditorHeight = 340;
    inline constexpr int kKnobSize     = 80;
    inline constexpr int kLabelHeight  = 20;

    // ── Colours (ARGB hex) ───────────────────────────────────────────────────
    inline constexpr unsigned int kColourBackground = 0xFF1A1A2E;
    inline constexpr unsigned int kColourAccent     = 0xFF9B5DE5;
    inline constexpr unsigned int kColourKnobTrack  = 0xFF2E2E48;
    inline constexpr unsigned int kColourText       = 0xFFE0E0E0;
} // namespace Constants
