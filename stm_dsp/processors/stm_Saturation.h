#pragma once

namespace stm {

/**
 An abstract base class for saturators
 */
class SaturatorBase : dsp::ProcessorBase
{
    virtual float saturateSample (float sample, float amount)=0;
    
    void setAmount (float _amount){
        amount = _amount;
    }
    
    void prepare (const dsp::ProcessSpec& spec) override {}
    
    void process (const dsp::ProcessContextReplacing<float>& context) override
    {
        auto input = context.getInputBlock();
        auto output = context.getOutputBlock();
        
        for (auto sample = 0 ; sample < input.getNumSamples() ; sample++)
        {
            for (auto channel = 0 ; channel < input.getNumChannels() ; channel++)
            {
                float saturatedSample = saturateSample(input.getSample(channel, sample), amount);
                
                output.setSample(channel, sample, saturatedSample);
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
    float saturateSample (float sample, float amount) override
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
        float wet = sample * sample;
        wet -= 0.5f;
        wet *= 2.0f;
        return wet * mix + sample * (1.0f - mix);
    }
};

} //namespace stm



//class CrispySaturator : dsp::ProcessorBase
//{
//public:
//
//    void setParams(float _oddPower, float _evenMix){
//        oddPower = _oddPower;
//        evenMix = _evenMix;
//    }
//
//    void prepare (const dsp::ProcessSpec& spec) override
//    {
//        processors.prepare(spec);
//    }
//
//    void process (const dsp::ProcessContextReplacing<float>& context) override
//    {
//        processors.process(context);
//    }
//
//    void reset() override
//    {
//        processors.reset();
//    }
//
//    static float saturateSample(float sample, float oddPower, float evenMix){
//        sample = CrispySaturatorOdd::saturate(sample, oddPower);
//
//        sample = CrispySaturatorEven::saturate(sample, evenMix);
//
//        return sample;
//    }
//
//    static void saturate(AudioSampleBuffer& buffer, int startSample, int numSamples, float oddPower, float evenMix){
//        for (int sample = startSample ; sample<startSample+numSamples ; sample++)
//        {
//            for (int channel = 0 ; channel<buffer.getNumChannels() ; channel++)
//            {
//                float currentSample = buffer.getSample(channel, sample);
//                float saturatedSample = saturateSample(currentSample, oddPower, evenMix);
//                buffer.setSample(channel, sample, saturatedSample);
//            }
//        }
//    }
//
//private:
//    float oddPower = 1.0f;
//    float evenMix = 0.0f;
//
//    enum {
//        oddIndex,
//        evenIndex
//    };
//
//    dsp::ProcessorChain<CrispySaturatorOdd, CrispySaturatorEven> processors;
//};
