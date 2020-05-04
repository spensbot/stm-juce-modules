#pragma once
#include "../utilities/stm_Rampers.h"

using namespace juce;

namespace stm {

class Panner {
    
public:
    void prepare() {
        panRamper.prepare(0.5f, 0.01f);
    }
    
    void process (const dsp::AudioBlock<float>& processBlock)
    {
        panRamper.getNext();
    }
    
    void reset() {
        panRamper.reset();
    }
    
    void setPan(float pan)
    {
        panRamper.updateTarget(pan);
    }

private:
    
    
    RamperLinear panRamper;
};

} // namespace stm
