#pragma once

using namespace juce;

namespace stm {

class RecircBuffer {
public:
    AudioBuffer<float> buffer;
    
    void prepare(int numSamples, int numChannels = 1){
        buffer.setSize(numChannels, numSamples);
        reset();
    }
    
    void reset(){
        buffer.clear();
        index = 0;
    }
    
    /**
     Adds a sample to the buffer at the current index.
     Be sure to increment() after filling all channels to update the index.
    */
    void push(float sample, int channel = 0){
        buffer.setSample(channel, index, sample);
    }
    
    /**
     Linearly interpolates between samples to return non-integer sample delays.
     */
    float getInterSample(float delaySamples, int channel = 0){
        int delay0 = int (delaySamples);
        int frac = delaySamples - float (delay0);
        int rise = getSample(delay0 + 1, channel) - getSample(delay0, channel);
        return rise * frac;
    }
    
    /**
     Returns a sample at the current index.
     Be sure to get samples after push() and before increment()
     Otherwise, getSample(0) will return the oldest sample in the buffer.
     */
    float getSample(int delaySamples, int channel = 0){
        int delayIndex = index - delaySamples;
        if (delayIndex < 0){
            delayIndex = buffer.getNumSamples() + delayIndex;
        }
        return buffer.getSample(channel, delayIndex);
    }
    
    void increment(){
        index += 1;
        index %= buffer.getNumSamples();
    }
    
private:
    int index = 0;
};

} // namespace stm
