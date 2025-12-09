#pragma once

#include <JuceHeader.h>
#include <foleys_gui_magic/foleys_gui_magic.h>

class DebugWindow : public juce::DocumentWindow, private juce::Timer
{
public:
    DebugWindow();
    ~DebugWindow() override;

    void closeButtonPressed() override;
    void resized() override;
    void timerCallback() override;
    
    void prepareToPlay(double sampleRate, int samplesPerBlockExpected);
    void pushAudioSamples(const juce::AudioBuffer<float>& buffer);
    
private:
    foleys::MagicGUIState magicState;
    std::unique_ptr<foleys::MagicOscilloscope> oscilloscope;
    foleys::MagicPlotSource* analyser = nullptr;
    foleys::MagicPlotComponent* oscilloscopeComponent = nullptr;
    foleys::MagicPlotComponent* analyserComponent = nullptr;
    
    juce::Component* container = nullptr;
    juce::AudioBuffer<float> monoBuffer;
};
