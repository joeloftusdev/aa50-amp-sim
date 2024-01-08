
#pragma once

#include "PluginProcessor.h"

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
