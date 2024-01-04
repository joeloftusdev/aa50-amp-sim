
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"
#include "VerticalGradientMeter.h"
#include "LookAndFeel.h"

//==============================================================================

class AA50AudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    AA50AudioProcessorEditor(AA50AudioProcessor&);
    ~AA50AudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    void fileLoader();
    void setSliderProperties(juce::Slider* sliderToSet);
    void makeSliderAttachments();
    

private:
    juce::LookAndFeel_V4 lookAndFeel;
    MXRLookAndFeel sliderLookAndFeel;

    Gui::VerticalGradientMeter meterOutput;

    juce::TextButton loadButton;
    juce::Label irName;
    juce::Image backgroundImage;
    
    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Slider sliderInput;
    juce::Label labelInput;

    juce::Slider sliderPreGain;
    juce::Label labelPreGain; 

    juce::Slider sliderPostGain;
    juce::Label labelPostGain; 

    juce::Slider sliderResonance;
    juce::Label labelResonance; 

    juce::Slider sliderTreble;
    juce::Label labelTreble;

    juce::Slider sliderMid;
    juce::Label labelMid;

    juce::Slider sliderBass;
    juce::Label labelBass;

    juce::Slider sliderPresence;
    juce::Label labelPresence;


    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentInput;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPostGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentResonance;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPresence;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentTreble;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentMid;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentBass;
   

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AA50AudioProcessor& audioProcessor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AA50AudioProcessorEditor)
};
