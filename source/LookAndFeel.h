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
#define M_PI		3.14159265358979323846f
#define jcolor  juce::Colour::fromRGB(144,128,128)

class MXRLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        auto radius = (float)juce::jmin(width / 2, height / 2) ;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;

        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;

        g.setColour(juce::Colour::fromRGB(30,27,27));
        g.fillEllipse(rx, ry, rw, rw);

        g.setColour(jcolor);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.47f;
        auto pointerThickness = radius / 10;

        p.addRoundedRectangle(-pointerThickness, -pointerLength, pointerThickness, pointerLength, pointerThickness / 2, pointerThickness / 2);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX + pointerThickness / 2, centreY));
        g.setColour(juce::Colour::fromRGB(144, 128, 128));
        g.fillPath(p);

        juce::Path p2;
        auto dotsize = radius / 10;
        p2.addEllipse(-dotsize / 2, -radius * 0.95f, dotsize, dotsize); 
        p2.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        g.fillPath(p2);

        float jangle_step = 2.f * M_PI / 7.f;
        float jangle = angle + (M_PI + jangle_step) / 2;
        float jr = radius * 0.8f;
        float jx, jx1, jy, jy1;
        g.setColour(jcolor);
        jx = jr * cosf(jangle) + centreX; jy = jr * sinf(jangle) + centreY;

        for (int i = 0; i < 7; i++) {
            jangle += jangle_step;
            jx1 = jr * cosf(jangle) + centreX;
            jy1 = jr * sinf(jangle) + centreY;
            g.drawLine(jx, jy, jx1, jy1);
            jx = jx1;
            jy = jy1;
        }

        g.setColour(juce::Colours::red);
        jr = radius * 2;
        jangle = angle + (M_PI + jangle_step) / 2;
        jx = jr * cosf(jangle) + centreX; jy = jr * sinf(jangle) + centreY;
        juce::Path p3;
        for (int i = 0; i < 7; i++) {
            jangle += jangle_step;
            jx1 = -jr * cosf(jangle) + centreX;
            jy1 = -jr * sinf(jangle) + centreY;
            p3.addCentredArc(jx1, jy1, jr * 0.666f, jr * 0.666f, jangle + jangle_step + M_PI / 2, -jangle_step * 0.707f, -jangle_step - M_PI * 0.08f, true);
            jx = jx1;
            jy = jy1;
        }
        g.setColour(jcolor);
        g.strokePath(p3, juce::PathStrokeType(1.0));
    }
};



