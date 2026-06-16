#include "PluginProcessor.h"
#include "PluginEditor.h"

ChorusAudioProcessor::ChorusAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", Parameters::createLayout())
{
}

void ChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    chorus.prepare (sampleRate, samplesPerBlock,
                    getTotalNumOutputChannels());
}

void ChorusAudioProcessor::releaseResources()
{
    chorus.reset();
}

void ChorusAudioProcessor::syncParamsToProcessor() noexcept
{
    ChorusProcessor::Parameters p;
    p.rateHz    = apvts.getRawParameterValue (ParameterIDs::rate)    ->load();
    p.depth     = apvts.getRawParameterValue (ParameterIDs::depth)   ->load();
    p.delayMs   = apvts.getRawParameterValue (ParameterIDs::delay)   ->load();
    p.feedback  = apvts.getRawParameterValue (ParameterIDs::feedback)->load();
    p.mix       = apvts.getRawParameterValue (ParameterIDs::mix)     ->load();
    p.numVoices = static_cast<int> (
                    apvts.getRawParameterValue (ParameterIDs::voices)->load());
    p.stereo    = apvts.getRawParameterValue (ParameterIDs::stereo)  ->load() > 0.5f;
    chorus.setParameters (p);
}

void ChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    syncParamsToProcessor();
    chorus.process (buffer);
}

juce::AudioProcessorEditor* ChorusAudioProcessor::createEditor()
{
    return new ChorusAudioProcessorEditor (*this);
}

void ChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusAudioProcessor();
}
