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

namespace Gui

{
	class Bulb : public juce::Component
	{
	public:
		Bulb(const juce::Colour& c) : colour(c) {}
		void paint(juce::Graphics& g) override
		{
			if (isOn)
			{
				g.setColour(colour);

			}
			else
			{
				g.setColour(juce::Colours::black);
			}
			const auto delta = 4.f;
			const auto bounds = getLocalBounds().toFloat().reduced(delta);
			const auto side = juce::jmin(bounds.getWidth(), bounds.getHeight());
			const auto bulbFillBounds = juce::Rectangle<float>{ bounds.getX(), bounds.getY(), side, side };
			g.fillEllipse(bulbFillBounds);
			g.setColour(juce::Colours::grey);
			g.drawEllipse(bulbFillBounds, 1.f);
			if (isOn)
			{
				g.setGradientFill(juce::ColourGradient{ colour.withAlpha(0.3f), bulbFillBounds.getCentre(), colour.withLightness(1.5f).withAlpha(0.f), {}, true });
				g.fillEllipse(bulbFillBounds.expanded(delta));
			}

		}
		void setState(const bool state) { isOn = state; }
	private:
		bool isOn = false;
		juce::Colour colour{};
	};

	class VerticalDiscreteMeter : public juce::Component, public juce::Timer
	{
	public:
		VerticalDiscreteMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
		{
			startTimerHz(24);
		}

		void paint(juce::Graphics& g) override
		{
			const auto value = juce::jmap(valueSupplier(), -60.f, 6.f, 0.f, 1.f);
			for (auto i = 0; i < totalNumberOfBulbs; i++)
			{
				if (value >= static_cast<float>(i + 1) / totalNumberOfBulbs)
					bulbs[i]->setState(true);
				else
					bulbs[i]->setState(false);
			}
		}

		void resized() override
		{
			const auto bounds = getLocalBounds().toFloat();
			juce::ColourGradient gradient{ juce::Colours::green, bounds.getBottomLeft(), juce::Colours::red, bounds.getTopLeft(), false };
			gradient.addColour(0.5, juce::Colours::yellow);

			const auto bulbHeight = getLocalBounds().getHeight() / totalNumberOfBulbs;
			auto bulbBounds = getLocalBounds();
			bulbs.clear();
			for (auto i = 0; i < totalNumberOfBulbs; i++)
			{
				auto bulb = std::make_unique<Bulb>(gradient.getColourAtPosition(static_cast<double>(i) / totalNumberOfBulbs));
				addAndMakeVisible(bulb.get());
				bulb->setBounds(bulbBounds.removeFromBottom(bulbHeight));
				bulbs.push_back(std::move(bulb));
			}
		}

		void timerCallback() override
		{
			repaint();
		}

	private:
		std::function<float()> valueSupplier;
		std::vector<std::unique_ptr<Bulb>> bulbs;
		const int totalNumberOfBulbs = 10;
	};
}