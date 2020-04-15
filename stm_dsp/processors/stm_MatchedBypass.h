#pragma once
#include "../utilities/stm_Rampers.h"
#include "../utilities/stm_Measuring.h"
#include "../../stm_gui/displays/stm_DebugDisplay.h"

using namespace juce;

namespace stm {


class MatchedBypass
{
public:
    void setActive (bool isActive){
        isBypassActive = isActive;
        
        justSwitched = true;
    }
    
    void prepare (const dsp::ProcessSpec& spec)
    {
        gainRamper.prepare(1.0f, 0.001f);
    }
    
    /**
     The MatchedBypass processor only works with a ProcessContextNonReplacing.
     Using this processor with ProcessContextReplacing doesn't make sense.
     Because this processor expects separate inBlock and outBlock blocks.
     */
    void process (const dsp::ProcessContextReplacing<float>& context, const dsp::AudioBlock<float>& dryBlock)
    {
        if (isBypassActive) {
            auto outBlock = context.getOutputBlock();
            int numSamples = (int) outBlock.getNumSamples();
            
            jassert (dryBlock.getNumSamples() == numSamples);
            
            float unprocessedLevel = Measuring::getRMSLevel<float>(dryBlock, 0, numSamples);
            float processedLevel = Measuring::getRMSLevel<float>(outBlock, 0, numSamples);
            
            float targetGain = 1.0f;
            
            if (unprocessedLevel > 0.0f) {
                targetGain = processedLevel / unprocessedLevel;
            }
            
            gainRamper.updateTarget(targetGain);
            
            DebugDisplay::set(5, "Bypass Target Gain: " + String(targetGain));
            
            if (justSwitched) {
                //If the matched bypass was just turned on, we need to reset the gain ramper
                //So it doesn't ramp from the previous gain value to the new target
                gainRamper.reset();
                
                justSwitched = false;
            }
            
            for (int sampleIndex=0 ; sampleIndex<numSamples ; sampleIndex++)
            {
                float gain = gainRamper.getNext();
                
                for (int channelIndex=0 ; channelIndex<outBlock.getNumChannels() ; channelIndex++)
                {
                    float sample = dryBlock.getSample(channelIndex, sampleIndex);
                    outBlock.setSample(channelIndex, sampleIndex, sample * gain);
                }
            }
        }
    }
    
    void reset()
    {
        gainRamper.reset();
    }
    
private:
    bool justSwitched = false;
    bool isBypassActive = false;
    RamperLinear gainRamper;
};


} // namespace stm
