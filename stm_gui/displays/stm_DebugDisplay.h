#pragma once

using namespace juce;

namespace stm {

/**
 A JUCE Component that prints a public, static string array.
 The string array can be added to from anywhere in a program.
 */
class DebugDisplay : public Component, private Timer
{
public:
    static void add(int line, String message){
        jassert (line < numDebugLines);
        
        debugLines[line] = message;
    }
    
    DebugDisplay()
    {
        startTimerHz(15);
        for(int i=0 ; i<numDebugLines ; i++){
            debugLines[i] = "";
        }
        debugLines[0] = "Debug Messages:";
    }

    ~DebugDisplay()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::white);
        g.setFont(12.0f);
        Rectangle<int> bounds = getLocalBounds();
        bounds.reduce(5, 5);
        
        int lineHeight = 15;
        for (int line=0 ; line<numDebugLines ; line++){
            g.drawText(debugLines[line], bounds.removeFromTop(lineHeight), Justification::left, true);
        }
    }

    void resized() override
    {
        
    }
    
    void timerCallback() override {
        repaint();
    }

private:
    static const int numDebugLines = 30;
    inline static String debugLines[30];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugDisplay)
};

} //namespace stm
