
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"


//==============================================================================
AA50AudioProcessorEditor::AA50AudioProcessorEditor (AA50AudioProcessor& p)
    : AudioProcessorEditor(&p),
    meterOutput([&]() {return audioProcessor.getRMSOutputValue(0); }),
    audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1000, 300);


   // addAndMakeVisible(verticalGradientMeterInput);
    addAndMakeVisible(meterOutput);
    startTimerHz(24);

    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load IR");
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(30, 27, 27));
    loadButton.setColour(juce::ComboBox::outlineColourId, juce::Colour::fromRGB(144, 128, 128));
    loadButton.setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(227, 195, 195));
    loadButton.onClick = [this]()
        {
            fileLoader();
        };
    irName.setText(audioProcessor.savedFile.getFileName(), juce::dontSendNotification);
    irName.setColour(juce::Label::backgroundColourId, juce::Colour::fromRGB(30, 27, 27));
    irName.setColour(juce::Label::outlineColourId, juce::Colour::fromRGB(144, 128, 128));
    irName.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));
    addAndMakeVisible(irName);

    addAndMakeVisible(sliderInput);
    setSliderProperties(&sliderInput);
    sliderInput.setLookAndFeel(&sliderLookAndFeel);
    labelInput.attachToComponent(&sliderInput, false);
    labelInput.setText("VOLUME", juce::dontSendNotification);
    labelInput.setJustificationType(juce::Justification::centred);
    labelInput.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderPreGain);
    setSliderProperties(&sliderPreGain);
    sliderPreGain.setLookAndFeel(&sliderLookAndFeel);
    labelPreGain.attachToComponent(&sliderPreGain, false);
    labelPreGain.setText("PRE GAIN", juce::dontSendNotification);
    labelPreGain.setJustificationType(juce::Justification::centred);
    labelPreGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderPostGain);
    setSliderProperties(&sliderPostGain);
    sliderPostGain.setLookAndFeel(&sliderLookAndFeel);
    labelPostGain.attachToComponent(&sliderPostGain, false);
    labelPostGain.setText("POST GAIN", juce::dontSendNotification);
    labelPostGain.setJustificationType(juce::Justification::centred);
    labelPostGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible (sliderResonance);
    setSliderProperties (&sliderResonance);
    sliderResonance.setLookAndFeel (&sliderLookAndFeel);
    labelResonance.attachToComponent (&sliderResonance, false);
    labelResonance.setText ("RESONANCE", juce::dontSendNotification);
    labelResonance.setJustificationType (juce::Justification::centred);
    labelResonance.setColour (juce::Label::textColourId, juce::Colour::fromRGB (227, 195, 195));

    addAndMakeVisible(sliderTreble);
    setSliderProperties (&sliderTreble);
    sliderTreble.setLookAndFeel (&sliderLookAndFeel);
    labelTreble.attachToComponent (&sliderTreble, false);
    labelTreble.setText ("TREBLE", juce::dontSendNotification);
    labelTreble.setJustificationType (juce::Justification::centred);
    labelTreble.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderMid);
    setSliderProperties(&sliderMid);
    sliderMid.setLookAndFeel(&sliderLookAndFeel);
    labelMid.attachToComponent (&sliderMid, false);
    labelMid.setText ("MID", juce::dontSendNotification);
    labelMid.setJustificationType (juce::Justification::centred);
    labelMid.setColour (juce::Label::textColourId, juce::Colour::fromRGB (227, 195, 195));

    addAndMakeVisible(sliderBass);
    setSliderProperties (&sliderBass);
    sliderBass.setLookAndFeel (&sliderLookAndFeel);
    labelBass.attachToComponent (&sliderBass, false);
    labelBass.setText ("BASS", juce::dontSendNotification);
    labelBass.setJustificationType (juce::Justification::centred);
    labelBass.setColour (juce::Label::textColourId, juce::Colour::fromRGB (227, 195, 195));

    addAndMakeVisible(sliderPresence);
    setSliderProperties(&sliderPresence);
    sliderPresence.setLookAndFeel(&sliderLookAndFeel);
    labelPresence.attachToComponent(&sliderPresence, false);
    labelPresence.setText("PRESENCE", juce::dontSendNotification);
    labelPresence.setJustificationType(juce::Justification::centred);
    labelPresence.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));


    makeSliderAttachments();

   
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize);
}

AA50AudioProcessorEditor::~AA50AudioProcessorEditor()
{
}


void AA50AudioProcessorEditor::makeSliderAttachments()
{
    sliderAttachmentInput = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUT", sliderInput);
    sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POSTGAIN", sliderPostGain);
    sliderAttachmentResonance = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "RESONANCE", sliderResonance);
    sliderAttachmentPreGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PREGAIN", sliderPreGain);
    sliderAttachmentTreble = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREBLE", sliderTreble);
    sliderAttachmentMid = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MID", sliderMid);
    sliderAttachmentBass = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BASS", sliderBass);
    sliderAttachmentPresence = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PRESENCE", sliderPresence);

    

   
}

void AA50AudioProcessorEditor::timerCallback()
{
    if (audioProcessor.input)
    {
       meterOutput.setLevel(audioProcessor.getRMSOutputValue(0));
        meterOutput.repaint();
    }

}

void AA50AudioProcessorEditor::setSliderProperties (juce::Slider* sliderToSet)
{
    sliderToSet->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderToSet->setTextBoxStyle(juce::Slider::NoTextBox, false, 76, 38);
    sliderToSet->setDoubleClickReturnValue(true, 0.0f);
}

void AA50AudioProcessorEditor::fileLoader()
{
    fileChooser = std::make_unique<juce::FileChooser>("Choose file", audioProcessor.root, "*");



    const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectDirectories;

    fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            
            juce::File result(chooser.getResult());

            if (result.getFileExtension() == ".wav")
            {
                audioProcessor.savedFile = result;
                audioProcessor.root = result.getParentDirectory().getFullPathName();

                audioProcessor.valueTree.setProperty("file1", result.getFullPathName(), nullptr);
                audioProcessor.valueTree.setProperty("root", result.getParentDirectory().getFullPathName(), nullptr);


                audioProcessor.irLoader.reset();
                audioProcessor.irLoader.loadImpulseResponse(audioProcessor.savedFile, juce::dsp::Convolution::Stereo::yes,
                    juce::dsp::Convolution::Trim::yes, 0);
       
                irName.setText(result.getFileName(), juce::dontSendNotification);
                

            }
        });
}

//==============================================================================
void AA50AudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);

    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }

    
   
   
    
}

void AA50AudioProcessorEditor::resized()
{
    int meterWidth = 20;
    int meterHeight = 510;
    int meterX = 972;
    int meterY = 55;
    meterOutput.setBounds(meterX, meterY, meterWidth, meterHeight);


    loadButton.setBounds(35 - 10, 20, 100, 35);
    irName.setBounds(35 + 100, 20, 120, 35);
    
  sliderPresence.setBounds(proportionOfWidth(0.81f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderResonance.setBounds (proportionOfWidth (0.70f) - (proportionOfWidth (0.08f) / 2), proportionOfHeight (0.9f) - (proportionOfHeight (0.402f) / 2), proportionOfWidth (0.2f), proportionOfHeight (0.22f));
  sliderPostGain.setBounds(proportionOfWidth(0.59f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderTreble.setBounds(proportionOfWidth(0.48f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderMid.setBounds(proportionOfWidth(0.37f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderBass.setBounds(proportionOfWidth(0.26f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderPreGain.setBounds(proportionOfWidth(0.15f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderInput.setBounds(proportionOfWidth(0.04f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  
  
   
}



void AA50AudioProcessorEditor::channelSelect()
{
    switch (ampChannel.getSelectedId())
    {
    case 1:
        audioProcessor.waveshaperFunction = "Lead";
        break;
    case 2:
        audioProcessor.waveshaperFunction = "Crunch";
        break;
    default:
        audioProcessor.waveshaperFunction = "Lead";
        break;
    }
}

