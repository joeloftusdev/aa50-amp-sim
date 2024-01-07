
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"


//==============================================================================
AA50AudioProcessorEditor::AA50AudioProcessorEditor (AA50AudioProcessor& p)
    : AudioProcessorEditor(&p),
    _meterOutput([&]() {return audioProcessor.getRMSOutputValue(0); }),
    audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1000, 300);

    addAndMakeVisible(_meterOutput);
    startTimerHz(24);

    addAndMakeVisible(_loadButton);
    _loadButton.setButtonText("Load IR");
    _loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(30, 27, 27));
    _loadButton.setColour(juce::ComboBox::outlineColourId, juce::Colour::fromRGB(144, 128, 128));
    _loadButton.setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(227, 195, 195));
    _loadButton.onClick = [this]()
        {
            fileLoader();
        };
    _irName.setText(audioProcessor.savedFile.getFileName(), juce::dontSendNotification);
    _irName.setColour(juce::Label::backgroundColourId, juce::Colour::fromRGB(30, 27, 27));
    _irName.setColour(juce::Label::outlineColourId, juce::Colour::fromRGB(144, 128, 128));
    _irName.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));
    addAndMakeVisible(_irName);

    addAndMakeVisible(_sliderInput);
    setSliderProperties(&_sliderInput);
    _sliderInput.setLookAndFeel(&_sliderLookAndFeel);
    _labelInput.attachToComponent(&_sliderInput, false);
    _labelInput.setText("VOLUME", juce::dontSendNotification);
    _labelInput.setJustificationType(juce::Justification::centred);
    _labelInput.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(_sliderPreGain);
    setSliderProperties(&_sliderPreGain);
    _sliderPreGain.setLookAndFeel(&_sliderLookAndFeel);
    _labelPreGain.attachToComponent(&_sliderPreGain, false);
    _labelPreGain.setText("PRE GAIN", juce::dontSendNotification);
    _labelPreGain.setJustificationType(juce::Justification::centred);
    _labelPreGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(_sliderPostGain);
    setSliderProperties(&_sliderPostGain);
    _sliderPostGain.setLookAndFeel(&_sliderLookAndFeel);
    _labelPostGain.attachToComponent(&_sliderPostGain, false);
    _labelPostGain.setText("POST GAIN", juce::dontSendNotification);
    _labelPostGain.setJustificationType(juce::Justification::centred);
    _labelPostGain.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible (_sliderResonance);
    setSliderProperties (&_sliderResonance);
    _sliderResonance.setLookAndFeel (&_sliderLookAndFeel);
    _labelResonance.attachToComponent (&_sliderResonance, false);
    _labelResonance.setText ("RESONANCE", juce::dontSendNotification);
    _labelResonance.setJustificationType (juce::Justification::centred);
    _labelResonance.setColour (juce::Label::textColourId, juce::Colour::fromRGB (227, 195, 195));

    addAndMakeVisible(_sliderTreble);
    setSliderProperties (&_sliderTreble);
    _sliderTreble.setLookAndFeel (&_sliderLookAndFeel);
    _labelTreble.attachToComponent (&_sliderTreble, false);
    _labelTreble.setText ("TREBLE", juce::dontSendNotification);
    _labelTreble.setJustificationType (juce::Justification::centred);
    _labelTreble.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    addAndMakeVisible(_sliderMid);
    setSliderProperties(&_sliderMid);
    _sliderMid.setLookAndFeel(&_sliderLookAndFeel);
    _labelMid.attachToComponent (&_sliderMid, false);
    _labelMid.setText ("MID", juce::dontSendNotification);
    _labelMid.setJustificationType (juce::Justification::centred);
    _labelMid.setColour (juce::Label::textColourId, juce::Colour::fromRGB (227, 195, 195));

    addAndMakeVisible(_sliderBass);
    setSliderProperties (&_sliderBass);
    _sliderBass.setLookAndFeel (&_sliderLookAndFeel);
    _labelBass.attachToComponent (&_sliderBass, false);
    _labelBass.setText ("BASS", juce::dontSendNotification);
    _labelBass.setJustificationType (juce::Justification::centred);
    _labelBass.setColour (juce::Label::textColourId, juce::Colour::fromRGB (227, 195, 195));

    addAndMakeVisible(_sliderPresence);
    setSliderProperties(&_sliderPresence);
    _sliderPresence.setLookAndFeel(&_sliderLookAndFeel);
    _labelPresence.attachToComponent(&_sliderPresence, false);
    _labelPresence.setText("PRESENCE", juce::dontSendNotification);
    _labelPresence.setJustificationType(juce::Justification::centred);
    _labelPresence.setColour(juce::Label::textColourId, juce::Colour::fromRGB(227, 195, 195));

    makeSliderAttachments();

    _backgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize);
}

AA50AudioProcessorEditor::~AA50AudioProcessorEditor()
{
}


void AA50AudioProcessorEditor::makeSliderAttachments()
{
    _sliderAttachmentInput = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUT", _sliderInput);
    _sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POSTGAIN", _sliderPostGain);
    _sliderAttachmentResonance = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "RESONANCE", _sliderResonance);
    _sliderAttachmentPreGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PREGAIN", _sliderPreGain);
    _sliderAttachmentTreble = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREBLE", _sliderTreble);
    _sliderAttachmentMid = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MID", _sliderMid);
    _sliderAttachmentBass = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BASS", _sliderBass);
    _sliderAttachmentPresence = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PRESENCE", _sliderPresence);
}

void AA50AudioProcessorEditor::timerCallback()
{
    if (audioProcessor.input)
    {
       _meterOutput.setLevel(audioProcessor.getRMSOutputValue(0));
       _meterOutput.repaint();
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
    _fileChooser = std::make_unique<juce::FileChooser>("Choose file", audioProcessor.root, "*");

    const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectDirectories;

    _fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
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
       
                _irName.setText(result.getFileName(), juce::dontSendNotification);
            }
        });
}

//==============================================================================
void AA50AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    {
        g.drawImage(_backgroundImage, getLocalBounds().toFloat());
    }
}

void AA50AudioProcessorEditor::resized()
{
    int meterWidth = 20;
    int meterHeight = 510;
    int meterX = 972;
    int meterY = 55;
    _meterOutput.setBounds(meterX, meterY, meterWidth, meterHeight);

    _loadButton.setBounds(35 - 10, 20, 100, 35);
    _irName.setBounds(35 + 100, 20, 120, 35);
    
  _sliderPresence.setBounds(proportionOfWidth(0.81f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  _sliderResonance.setBounds (proportionOfWidth (0.70f) - (proportionOfWidth (0.08f) / 2), proportionOfHeight (0.9f) - (proportionOfHeight (0.402f) / 2), proportionOfWidth (0.2f), proportionOfHeight (0.22f));
  _sliderPostGain.setBounds(proportionOfWidth(0.59f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  _sliderTreble.setBounds(proportionOfWidth(0.48f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  _sliderMid.setBounds(proportionOfWidth(0.37f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  _sliderBass.setBounds(proportionOfWidth(0.26f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  _sliderPreGain.setBounds(proportionOfWidth(0.15f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));
  _sliderInput.setBounds(proportionOfWidth(0.04f) - (proportionOfWidth(0.08f) / 2), proportionOfHeight(0.9f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2f), proportionOfHeight(0.22f));

}

