#pragma once

#include "JuceHeader.h"
#include "CustomKnob.h"
#include <foleys_gui_magic/foleys_gui_magic.h>

namespace CustomComponents
{

class CustomKnobItem : public foleys::GuiItem
{
public:
    FOLEYS_DECLARE_GUI_FACTORY(CustomKnobItem)
    
    CustomKnobItem(foleys::MagicGUIBuilder& builder, const juce::ValueTree& node)
        : foleys::GuiItem(builder, node)
    {
        setColourTranslation({
            {"slider-background", juce::Slider::backgroundColourId},
            {"slider-thumb", juce::Slider::thumbColourId},
            {"slider-track", juce::Slider::trackColourId},
            {"rotary-fill", juce::Slider::rotarySliderFillColourId},
            {"rotary-outline", juce::Slider::rotarySliderOutlineColourId},
            {"slider-text", juce::Slider::textBoxTextColourId}
        });
        
        addAndMakeVisible(customKnob);
    }
    
    void update() override
    {
        attachment.reset();
        
        auto paramID = getControlledParameterID({});
        if (paramID.isNotEmpty())
        {
            attachment = getMagicState().createAttachment(paramID, customKnob);
        }
    }
    
    std::vector<juce::Component*> getComponents() 
    {
        return {&customKnob};
    }
    
    juce::Component* getWrappedComponent() override
    {
        return &customKnob;
    }
    
private:
    CustomKnob customKnob;
    std::unique_ptr<juce::SliderParameterAttachment> attachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomKnobItem)
};

} // namespace CustomComponents
