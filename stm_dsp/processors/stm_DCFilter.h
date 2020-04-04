#pragma once

using namespace juce;

namespace stm {

/**
 A low-pass IIR filter intended to remove dc bias from a signal
 */
class DCFilter : dsp::ProcessorBase
{
public:
    void prepare (const dsp::ProcessSpec& spec) override {
        float Q = 12.0f; // db per octave
        float freq = 20.0f; // Hz
        
        auto hiPassCoefs = FilterCoefs::makeHighPass(spec.sampleRate, freq, Q);
        
        *(filter.state) = *hiPassCoefs;
        
        filter.prepare(spec);
    }
    
    void process (const dsp::ProcessContextReplacing<float>& context) override
    {
        filter.process(context);
    }
    
    void reset() override {
        filter.reset();
    }
    
private:
    using Filter = dsp::IIR::Filter<float>;
    using FilterCoefs = dsp::IIR::Coefficients<float>;
    using DuplicatedFilter = dsp::ProcessorDuplicator<Filter, FilterCoefs>;
    
    DuplicatedFilter filter;
};

} //namespace stm

