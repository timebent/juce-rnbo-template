# Alterations:

## You have to do this each time you export your RNBO code to the export folder.


### 0. Find the RNBO_JuceAudioProcessor.h and the RNBO_JuceAudioProcssor.cpp in 
    export->rnbo->adpaters->juce

        0a. Change the .h file by adding 
            ```
            #include "JuceHeader.h"
            ```

        0b. Also, change what class RNBO_JuceAudioProcessor inherits from ... from:
            ```
            juce::AudioProcessor
            ```
            to 

            ```
            foleys::MagicProcessor
            ```

        0c. Change the .cpp files, by changing the constructor initialization list to use MagicProcessor instead of AudioProcessor
        
        0d. Also, remove these function: 
            ```
            bool JuceAudioProcessor::hasEditor() const
            ```

            ```
            AudioProcessorEditor* JuceAudioProcessor::createEditor()
            ```

            ```
            void JuceAudioProcessor::getStateInformation (MemoryBlock& destData)
            ```

            ```
            void JuceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
            ```


## These are the steps that were taken to make this work. These are baked into the template.

### 1. An updated version (JUCE 8) of juce is retrieved as a submodule. The Foleys Plugin GUI Magic is also retrieved as a submodule. 

### 2. Added foley to the App.cmake file 
        - this line has to go above juce_generate_juce_header(RNBOApp)
            ```
            juce_add_module(thirdparty/foleys_gui_magic/modules/foleys_gui_magic)
            ```
        - Also, added to the "target link libraries"
            ```
            juce::juce_dsp
            juce::juce_cryptography
            foleys_gui_magic 
            ```

### 3. Added foley to the Plugin.cmake file

        - Add to the "target link libraries"
            ```
            foleys_gui_magic 
            ```

### 4.  Erased this bit of code in MainComponent.cpp
            ```
            // let's listen to all midi inputs
            // enable all midi inputs
            StringArray midiInputDevices = MidiInput::getAvailableDevices();
            for (const auto& input : midiInputDevices) {
            _deviceManager.setMidiInputEnabled(input, true);
            }
            _deviceManager.addMidiInputCallback("", &_audioProcessorPlayer);
            ```

### 5. In the src folder, changed the customAudioProcessor.h removing the following function:
            ```
            juce::AudioProcessorEditor* createEditor()
            ```
            
### 6. In the src folder, changed the customAudioProcessor.cpp so that you removed the following function:
            ```
            AudioProcessorEditor* CustomAudioProcessor::createEditor() 
            ```



