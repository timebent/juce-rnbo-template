#pragma once

#include <JuceHeader.h>
#include <foleys_gui_magic/foleys_gui_magic.h>

class DebugComponent : public juce::Component
{
public:
    DebugComponent(juce::AudioProcessor* processor);
    ~DebugComponent();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void processAudioBlock(const juce::AudioBuffer<float>& buffer);
    
private:
    juce::AudioProcessor* audioProcessor;
    foleys::MagicGUIState magicState;
    
    std::unique_ptr<foleys::MagicOscilloscope> oscilloscope;
    foleys::MagicPlotSource* analyser = nullptr;
    std::unique_ptr<foleys::MagicPlotComponent> oscilloscopeDisplay;
    std::unique_ptr<foleys::MagicPlotComponent> analyserDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugComponent)
};
