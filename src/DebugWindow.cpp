#include "DebugWindow.h"

// Simple container component that paints a background
class PlotContainer : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        
        // Draw a dividing line in the middle
        g.setColour(juce::Colours::darkgrey);
        auto bounds = getLocalBounds();
        g.drawLine(0, bounds.getHeight() / 2.0f, bounds.getWidth(), bounds.getHeight() / 2.0f, 2.0f);
    }
};

DebugWindow::DebugWindow()
    : juce::DocumentWindow ("Debug", juce::Colours::darkgrey, juce::DocumentWindow::closeButton)
{
    try
    {
        setUsingNativeTitleBar(true);
        setAlwaysOnTop(true);
        
        // Initialize mono buffer
        monoBuffer.setSize(1, 512);
        
        // Create the plot sources
        oscilloscope.reset(new foleys::MagicOscilloscope());
        analyser = magicState.createAndAddObject<foleys::MagicAnalyser>("input");
        
        if (oscilloscope)
            oscilloscope->prepareToPlay(44100.0, 512);
        if (analyser)
            analyser->prepareToPlay(44100.0, 512);
        
        // Create the plot components
        oscilloscopeComponent = new foleys::MagicPlotComponent();
        analyserComponent = new foleys::MagicPlotComponent();
        
        if (!oscilloscopeComponent || !analyserComponent)
            throw std::runtime_error("Failed to create components");
        
        // Set the plot sources
        oscilloscopeComponent->setPlotSource(oscilloscope.get());
        analyserComponent->setPlotSource(analyser);
        
        oscilloscopeComponent->setColour(foleys::MagicPlotComponent::plotColourId, juce::Colours::lime);
        analyserComponent->setColour(foleys::MagicPlotComponent::plotColourId, juce::Colours::cyan);
        
        // Set fill colors transparent
        oscilloscopeComponent->setColour(foleys::MagicPlotComponent::plotFillColourId, juce::Colours::transparentBlack);
        analyserComponent->setColour(foleys::MagicPlotComponent::plotFillColourId, juce::Colours::transparentBlack);
        
        // Make sure components are visible
        oscilloscopeComponent->setVisible(true);
        analyserComponent->setVisible(true);
        
        // Make them non-opaque
        oscilloscopeComponent->setOpaque(false);
        analyserComponent->setOpaque(false);
        
        // Create container
        container = new PlotContainer();
        container->setOpaque(true);
        container->addAndMakeVisible(oscilloscopeComponent);
        container->addAndMakeVisible(analyserComponent);
        
        setContentOwned(container, true);
        centreWithSize(600, 400);
        setVisible(true);
        
        // Start timer to trigger repaints
        startTimer(50); // 20 fps refresh rate
    }
    catch (const std::exception& e)
    {
        juce::Logger::writeToLog(juce::String("DebugWindow error: ") + e.what());
    }
}

DebugWindow::~DebugWindow()
{
    stopTimer();
}

void DebugWindow::closeButtonPressed()
{
    setVisible(false);
}

void DebugWindow::timerCallback()
{
    // Trigger repaints
    if (container)
        container->repaint();
    if (analyserComponent)
        analyserComponent->repaint();
}

void DebugWindow::resized()
{
    if (container)
    {
        container->setBounds(getLocalBounds());
        
        auto area = container->getLocalBounds();
        auto oscArea = area.removeFromTop(area.getHeight() / 2);
        
        if (oscilloscopeComponent)
            oscilloscopeComponent->setBounds(oscArea);
        if (analyserComponent)
            analyserComponent->setBounds(area);
    }
    
    juce::DocumentWindow::resized();
}

void DebugWindow::prepareToPlay(double sampleRate, int samplesPerBlockExpected)
{
    if (oscilloscope)
        oscilloscope->prepareToPlay(sampleRate, samplesPerBlockExpected);
    if (analyser)
        analyser->prepareToPlay(sampleRate, samplesPerBlockExpected);
}

void DebugWindow::pushAudioSamples(const juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumSamples() <= 0 || buffer.getNumChannels() <= 0)
        return;
    
    try
    {
        // Ensure our mono buffer has the right size
        if (monoBuffer.getNumSamples() != buffer.getNumSamples())
            monoBuffer.setSize(1, buffer.getNumSamples(), false, true, false);
        
        monoBuffer.clear();
        
        // Get write pointer for our mono buffer
        float* monoData = monoBuffer.getWritePointer(0);
        
        if (monoData == nullptr)
            return;
        
        if (buffer.getNumChannels() == 1)
        {
            // Already mono, copy it directly
            const float* sourceData = buffer.getReadPointer(0);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                monoData[i] = sourceData[i];
        }
        else if (buffer.getNumChannels() >= 2)
        {
            // Mix first two channels to mono (average them)
            const float* channel0 = buffer.getReadPointer(0);
            const float* channel1 = buffer.getReadPointer(1);
            
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                monoData[i] = (channel0[i] + channel1[i]) * 0.5f;
        }
        
        // Push to oscilloscope and analyser
        if (oscilloscope)
            oscilloscope->pushSamples(monoBuffer);
        if (analyser)
            analyser->pushSamples(monoBuffer);
    }
    catch (const std::exception& e)
    {
        juce::Logger::writeToLog(juce::String("Error pushing audio samples: ") + e.what());
    }
}