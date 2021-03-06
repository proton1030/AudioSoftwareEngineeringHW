/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VibratoPluginAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public Button::Listener
{
public:
    VibratoPluginAudioProcessorEditor (VibratoPluginAudioProcessor&);
    ~VibratoPluginAudioProcessorEditor();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VibratoPluginAudioProcessor& processor;
    ScopedPointer<Slider> pSliderVibratoWidth, pSliderVibratoFreq;
    ScopedPointer<TextButton> pButtonBypass;
    ScopedPointer<Label> pLabelVibratoWidth, pLabelVibratoFreq;

    void buttonClicked (Button* buttonThatWasClicked) override;
    void buttonStateChanged(Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoPluginAudioProcessorEditor)
};
