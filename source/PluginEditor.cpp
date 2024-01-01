
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"


//==============================================================================
GuitarAmpBasicAudioProcessorEditor::GuitarAmpBasicAudioProcessorEditor(GuitarAmpBasicAudioProcessor& p)
    : AudioProcessorEditor(&p),
    verticalGradientMeterInput([&]() {return audioProcessor.getRMSValueInput(0); }),
    verticalGradientMeterOutput([&]() {return audioProcessor.getRMSValueOutput(0); }),
    audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1000, 300);


   // addAndMakeVisible(verticalGradientMeterInput);
    addAndMakeVisible(verticalGradientMeterOutput);
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

    addAndMakeVisible(sliderInputGain);
    setSliderProperties(&sliderInputGain);
    sliderInputGain.setLookAndFeel(&sliderLookAndFeel);
    labelInputGain.attachToComponent(&sliderInputGain, false);
    labelInputGain.setText("VOLUME", juce::dontSendNotification);
    labelInputGain.setJustificationType(juce::Justification::centred);
    labelInputGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderPreGain2);
    setSliderProperties(&sliderPreGain2);
    sliderPreGain2.setLookAndFeel(&sliderLookAndFeel);
    labelPreGain2.attachToComponent(&sliderPreGain2, false);
    labelPreGain2.setText("PRE GAIN", juce::dontSendNotification);
    labelPreGain2.setJustificationType(juce::Justification::centred);
    labelPreGain2.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderPostGain);
    setSliderProperties(&sliderPostGain);
    sliderPostGain.setLookAndFeel(&sliderLookAndFeel);
    labelPostGain.attachToComponent(&sliderPostGain, false);
    labelPostGain.setText("POST GAIN", juce::dontSendNotification);
    labelPostGain.setJustificationType(juce::Justification::centred);
    labelPostGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderPreEQ);
    setSliderProperties(&sliderPreEQ);
    sliderPreEQ.setLookAndFeel(&sliderLookAndFeel);
    labelPreEQ.attachToComponent(&sliderPreEQ, false);
    labelPreEQ.setText("RESONANCE", juce::dontSendNotification);
    labelPreEQ.setJustificationType(juce::Justification::centred);
    labelPreEQ.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderFilterHighGain);
    setSliderProperties(&sliderFilterHighGain);
    sliderFilterHighGain.setLookAndFeel(&sliderLookAndFeel);
    labelFilterHighGain.attachToComponent(&sliderFilterHighGain, false);
    labelFilterHighGain.setText("TREBLE", juce::dontSendNotification);
    labelFilterHighGain.setJustificationType(juce::Justification::centred);
    labelFilterHighGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderFilterMidGain);
    setSliderProperties(&sliderFilterMidGain);
    sliderFilterMidGain.setLookAndFeel(&sliderLookAndFeel);
    labelFilterMidGain.attachToComponent(&sliderFilterMidGain, false);
    labelFilterMidGain.setText("MID", juce::dontSendNotification);
    labelFilterMidGain.setJustificationType(juce::Justification::centred);
    labelFilterMidGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderFilterLowGain);
    setSliderProperties(&sliderFilterLowGain);
    sliderFilterLowGain.setLookAndFeel(&sliderLookAndFeel);
    labelFilterLowGain.attachToComponent(&sliderFilterLowGain, false);
    labelFilterLowGain.setText("BASS", juce::dontSendNotification);
    labelFilterLowGain.setJustificationType(juce::Justification::centred);
    labelFilterLowGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(sliderPresence);
    setSliderProperties(&sliderPresence);
    sliderPresence.setLookAndFeel(&sliderLookAndFeel);
    labelPresence.attachToComponent(&sliderPresence, false);
    labelPresence.setText("PRESENCE", juce::dontSendNotification);
    labelPresence.setJustificationType(juce::Justification::centred);
    labelPresence.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));


    makeSliderAttachments();

   // cachedImage_BinaryData_bg_png = juce::ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize);
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize);
}

GuitarAmpBasicAudioProcessorEditor::~GuitarAmpBasicAudioProcessorEditor()
{
}


void GuitarAmpBasicAudioProcessorEditor::makeSliderAttachments()
{
    sliderAttachmentInputGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUTGAIN", sliderInputGain);
    sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POSTGAIN", sliderPostGain);
    sliderAttachmentPreEQ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PREEQ", sliderPreEQ);
    sliderAttachmentPreGain2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PREGAIN2", sliderPreGain2);
    sliderAttachmentFilterHighGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HIGH", sliderFilterHighGain);
    sliderAttachmentFilterMidGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MID", sliderFilterMidGain);
    sliderAttachmentFilterLowGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LOW", sliderFilterLowGain);
    sliderAttachmentPresence = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PRESENCE", sliderPresence);

    

   
}

void GuitarAmpBasicAudioProcessorEditor::timerCallback()
{

    if (audioProcessor.isInput)
    {
        verticalGradientMeterInput.setLevel(audioProcessor.getRMSValueInput(0));
        verticalGradientMeterInput.repaint();
    }
    else
    {
        verticalGradientMeterOutput.setLevel(audioProcessor.getRMSValueOutput(0));
        verticalGradientMeterOutput.repaint();
    }

}

void GuitarAmpBasicAudioProcessorEditor::setSliderProperties(juce::Slider* sliderToSet)
{
    sliderToSet->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderToSet->setTextBoxStyle(juce::Slider::NoTextBox, false, 76, 38);
    sliderToSet->setDoubleClickReturnValue(true, 0.0f);
}

void GuitarAmpBasicAudioProcessorEditor::fileLoader()
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

                audioProcessor.variableTree.setProperty("file1", result.getFullPathName(), nullptr);
                audioProcessor.variableTree.setProperty("root", result.getParentDirectory().getFullPathName(), nullptr);


                audioProcessor.irLoader.reset();
                audioProcessor.irLoader.loadImpulseResponse(audioProcessor.savedFile, juce::dsp::Convolution::Stereo::yes,
                    juce::dsp::Convolution::Trim::yes, 0);
               // irName.setColour(juce::Label::backgroundColourId, juce::Colour::fromRGB(30, 27, 27));
                //irName.setColour(juce::Label::outlineColourId, juce::Colour::fromRGB(144, 128, 128));
                irName.setText(result.getFileName(), juce::dontSendNotification);
                

            }
        });
}

//==============================================================================
void GuitarAmpBasicAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);

    {
        //int x = 0, y = 0, width = proportionOfWidth(1.0f), height = proportionOfHeight(0.65f);

        g.drawImage(backgroundImage, getLocalBounds().toFloat());

    }

    
   
   
    
}

void GuitarAmpBasicAudioProcessorEditor::resized()
{
    int meterWidth = 15;
    int meterHeight = 510;
    int meterX = 5;
    int meterY = 5;
    verticalGradientMeterInput.setBounds(meterX, meterY, meterWidth, meterHeight);
    verticalGradientMeterOutput.setBounds(getWidth() - (verticalGradientMeterInput.getWidth() + verticalGradientMeterInput.getX()), meterY, meterWidth, meterHeight);

   ;
   

    //IR load button/label
    loadButton.setBounds(35 - 10, 20, 100, 35);
    irName.setBounds(35 + 100, 20, 120, 35);
    

    //Waveshape menu
   // waveshapeType1.setBounds(getWidth() - (loadButton.getWidth() + leftOffest), topOffset, 100, 25);

  sliderPresence.setBounds(proportionOfWidth(0.81f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderPreEQ.setBounds(proportionOfWidth(0.70f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderPostGain.setBounds(proportionOfWidth(0.59f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderFilterHighGain.setBounds(proportionOfWidth(0.48f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderFilterMidGain.setBounds(proportionOfWidth(0.37f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderFilterLowGain.setBounds(proportionOfWidth(0.26f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderPreGain2.setBounds(proportionOfWidth(0.15f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  sliderInputGain.setBounds(proportionOfWidth(0.04f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  
  
   
}






void GuitarAmpBasicAudioProcessorEditor::modeMenuChanged()
{
    switch (waveshapeType1.getSelectedId())
    {
    case 1:
        audioProcessor.waveshapeFunction = "Tanh";
        break;
    case 2:
        audioProcessor.waveshapeFunction = "Amp2";
        break;
    default:
        audioProcessor.waveshapeFunction = "Tanh";
        break;
    }
}

