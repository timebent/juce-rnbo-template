#pragma once

#include <stack/StackProcessor.h>

class MyProcessor : public stack::StackProcessor {
public:
    MyProcessor (const nlohmann::json& patcher_desc,
                 const nlohmann::json& presets,
                 const RNBO::BinaryData& data);
    void prepare (double sampleRate, int maxBlockSize) override;
    void processPostRNBO(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
private:
    foleys::MagicPlotSource* oscilliscope { nullptr }; // non-owning
};
