
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

namespace Gui {

    class VerticalGradientMeter : public juce::Component, public juce::Timer
    {
    public:
        VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
        {
            startTimerHz(24);
           // grill = juce::ImageCache::getFromMemory(BinaryData::MeterGrill_png, BinaryData::MeterGrill_pngSize);
        }

        void paint(juce::Graphics& g) override
        {
            const auto level = valueSupplier();

            auto bounds = getLocalBounds().toFloat();


            g.setColour(juce::Colours::black);
            g.fillRect(bounds);

            g.setGradientFill(gradient);
            const auto scaledY = juce::jmap(level, -60.0f, 6.0f, 0.0f, static_cast<float>(getHeight()));
            g.fillRect(bounds.removeFromBottom(scaledY));

            auto currLevelBounds = getLocalBounds().toFloat();
            currLevelBounds.setX(currLevelBounds.getX() + 100);
        }

        void resized() override
        {
            const auto bounds = getLocalBounds().toFloat();
            gradient = juce::ColourGradient{
                juce::Colours::green,
                bounds.getBottomLeft(),
                juce::Colours::red,
                bounds.getTopLeft(),
                false
            };
            gradient.addColour(0.5f, juce::Colours::yellow);
        }

       // void paintOverChildren(::juce::Graphics& g) override
       // {
       //     g.drawImage(grill, getLocalBounds().toFloat());
       // }

        void setLevel(const float value)
        {
            level = value;
        }

        void timerCallback() override
        {
            repaint();
        }

    private:
        std::function<float()> valueSupplier;
        juce::ColourGradient gradient{};
        //juce::Image grill;
        float level = -60.0f;

    };
}
