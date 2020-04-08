#pragma once

namespace stm {

/**
 An abstract base class for saturators
 */
class SaturatorBase : dsp::ProcessorBase
{
public:
    virtual float saturateSample (float sample, float amount)=0;
    
    void setAmount (float _amount){
        amount = _amount;
    }
    
    void prepare (const dsp::ProcessSpec& spec) override {}
    
    void process (const dsp::ProcessContextReplacing<float>& context) override
    {
        auto& inBlock = context.getInputBlock();
        auto& outBlock = context.getOutputBlock();
        
        for (auto sample = 0 ; sample < inBlock.getNumSamples() ; sample++)
        {
            for (auto channel = 0 ; channel < inBlock.getNumChannels() ; channel++)
            {
                float saturatedSample = saturateSample(inBlock.getSample(channel, sample), amount);
                
                outBlock.setSample(channel, sample, saturatedSample);
            }
        }
    }
    
    void reset() override {}
    
private:
    float amount = 1.0f;
};

/**
 A simple hyperbolic tangent saturator
 */
class SaturatorTanH : public SaturatorBase
{
public:
    float saturateSample (float sample, float amount = 1.0f) override
    {
        return tanh(sample);
    };
};

/**
 Adds odd harmonics to a signal.
 Input must be between -1.0 and 1.0.
 */
class SaturatorCrispyOdd : public SaturatorBase
{
public:
    float saturateSample (float sample, float power) override
    {
        jlimit(-1.0f, 1.0f, sample);
        if (sample > 0) {
            return 1.0f - powf(1.0f-sample, power);
        } else {
            return -(1.0f - powf(1.0f+sample, power));
        }
    }
};


/**
Adds even harmonics to a signal.
Input must be between -1.0 and 1.0.
Note: Adding even harmonics introduces DC bias, which should be removed with a High Pass filter.
*/
class SaturatorCrispyEven : public SaturatorBase
{
public:
    float saturateSample(float sample, float mix)
    {
        jlimit(-1.0f, 1.0f, sample);
        float wet = sample * sample;
        wet -= 0.5f;
        wet *= 2.0f;
        return wet * mix + sample * (1.0f - mix);
    }
};

} //namespace stm
