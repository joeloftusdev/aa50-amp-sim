
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
        VerticalGradientMeter(std::function<float()>&& valueFunction) : _valueSupplier(std::move(valueFunction))
        {
            startTimerHz(24);
        }

        void paint(juce::Graphics& g) override
        {
            const auto meterLevel = _valueSupplier();

            auto bounds = getLocalBounds().toFloat();


            g.setColour(juce::Colours::black);
            g.fillRect(bounds);

            g.setGradientFill(_gradient);
            const auto scaledY = juce::jmap(meterLevel, -60.0f, 6.0f, 0.0f, static_cast<float>(getHeight()));
            g.fillRect(bounds.removeFromBottom(scaledY));

            auto currLevelBounds = getLocalBounds().toFloat();
            currLevelBounds.setX(currLevelBounds.getX() + 100.0f);
        }

        void resized() override
        {
            const auto bounds = getLocalBounds().toFloat();
            _gradient = juce::ColourGradient{
                juce::Colours::green,
                bounds.getBottomLeft(),
                juce::Colours::red,
                bounds.getTopLeft(),
                false
            };
            _gradient.addColour(0.5f, juce::Colours::yellow);
        }


        void setLevel(const float value)
        {
            level = value;
        }

        void timerCallback() override
        {
            repaint();
        }

    private:
        std::function<float()> _valueSupplier;
        juce::ColourGradient _gradient{};
        float level = -60.0f;

    };
}
