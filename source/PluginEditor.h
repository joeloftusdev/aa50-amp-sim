
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
/**
*/
class GuitarAmpBasicAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    GuitarAmpBasicAudioProcessorEditor(GuitarAmpBasicAudioProcessor&);
    ~GuitarAmpBasicAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void modeMenuChanged();
    void fileLoader();
    void setSliderProperties(juce::Slider* sliderToSet);
    void makeSliderAttachments();
    

private:
    juce::LookAndFeel_V4 lookAndFeel;
    MXRLookAndFeel sliderLookAndFeel;

    juce::Image levelMeterInputLines;
    juce::Image levelMeterInputNums;
    Gui::VerticalGradientMeter verticalGradientMeterInput;
    Gui::VerticalGradientMeter verticalGradientMeterOutput;

    juce::TextButton loadButton;
    juce::Label irName;
    juce::Image backgroundImage;
    juce::Image cachedImage_BinaryData_bg_png;
    

    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Label labelWaveshapeType1;//Label for menu
    juce::ComboBox waveshapeType1; //Menu selecting waveshaping function

    juce::Slider sliderInputGain;
    juce::Label labelInputGain;



    juce::Slider sliderPreGain2;//Resonance slider
    juce::Label labelPreGain2; //Resonance label

 

    juce::Slider sliderPostGain;//Postgain slider
    juce::Label labelPostGain; //Postgain label

    juce::Slider sliderPreEQ;//PreEQ slider
    juce::Label labelPreEQ; //PreEQ label

   
    //Filters
    juce::Slider sliderFilterHighGain;
    juce::Label labelFilterHighGain;

    juce::Slider sliderFilterMidGain;
    juce::Label labelFilterMidGain;

    juce::Slider sliderFilterLowGain;
    juce::Label labelFilterLowGain;

    juce::Slider sliderPresence;
    juce::Label labelPresence;


    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentInputGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreGain2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPostGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreEQ;
  //  std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachmentWaveshapeType1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPresence;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentFilterHighGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentFilterMidGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentFilterLowGain;
   

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarAmpBasicAudioProcessor& audioProcessor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuitarAmpBasicAudioProcessorEditor)
};
