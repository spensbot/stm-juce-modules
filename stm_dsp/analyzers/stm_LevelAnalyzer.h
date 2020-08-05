#pragma once
#include "../utilities/stm_RollingAverage.h"

namespace stm {


class LevelAnalyzer
{
public:
    void prepare (const dsp::ProcessSpec& spec) {
        levels.reserve(spec.numChannels);
        
        for (int iChannel = 0; iChannel < spec.numChannels ; iChannel++){
//            auto level = stm::RollingAverageBiased();
//            level.prepare(spec.sampleRate, 0.01f, 0.01f);
            auto level = stm::RollingAverage();
            level.prepare(100.0f);
            levels.push_back(level);
        }
    }
    
    void process (const juce::dsp::AudioBlock<float>& processBlock)
    {
        for (int iChannel = 0; iChannel < processBlock.getNumChannels() ; iChannel++){
            for (int iSample = 0; iSample < processBlock.getNumSamples() ; iSample++){\
                auto sample = processBlock.getSample(iChannel, iSample);
                levels[iChannel].push(sample);
            }
        }
        
    }
    
    void reset() {
        
    }
    
    std::vector<stm::RollingAverage> levels;
    
private:
};

} //namespace stm

