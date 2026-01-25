#include "MyProcessor.h"

MyProcessor::MyProcessor(const nlohmann::json& patcher_desc,
                         const nlohmann::json& presets,
                         const RNBO::BinaryData& data)
 : StackProcessor(patcher_desc, presets, data)
{
    oscilliscope = getMagicState().createAndAddObject<foleys::MagicOscilloscope> ("oscilloscope");
}

void MyProcessor::prepare(double sampleRate, int maxBlockSize) {
    juce::ignoreUnused(sampleRate, maxBlockSize);
    oscilliscope->prepareToPlay(sampleRate, maxBlockSize);
}

void MyProcessor::processPostRNBO(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midiBuffer)
{
    juce::ignoreUnused (midiBuffer);
    oscilliscope->pushSamples(audioBuffer);
}
