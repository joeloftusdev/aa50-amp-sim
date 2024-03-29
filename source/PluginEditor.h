
#pragma once
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
    juce::LookAndFeel_V4 _lookAndFeel;
    MXRLookAndFeel _sliderLookAndFeel;

    Gui::VerticalGradientMeter _meterOutput;

    juce::TextButton _loadButton;
    juce::Label _irName;
    juce::Image _backgroundImage;
    
    std::unique_ptr<juce::FileChooser> _fileChooser;

    juce::Slider _sliderInput;
    juce::Label _labelInput;

    juce::Slider _sliderPreGain;
    juce::Label _labelPreGain; 

    juce::Slider _sliderPostGain;
    juce::Label _labelPostGain;  

    juce::Slider _sliderTreble;
    juce::Label _labelTreble;

    juce::Slider _sliderMid;
    juce::Label _labelMid;

    juce::Slider _sliderBass;
    juce::Label _labelBass;

    juce::Slider _sliderResonance;
    juce::Label _labelResonance;

    juce::Slider _sliderPresence;
    juce::Label _labelPresence;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentInput;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentPreGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentPostGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentResonance;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentPresence;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentTreble;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentMid;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _sliderAttachmentBass;
   
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AA50AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AA50AudioProcessorEditor)
};
