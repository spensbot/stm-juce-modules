#pragma once

namespace stm {

class RecircBuffer {
public:
    juce::AudioBuffer<float> buffer;
    
    void reset(int newSize){
        buffer.setSize(1, newSize);
        buffer.clear();
        writeIndex = 0;
    }
    
    int getSize(){
        return buffer.getNumSamples();
    }
    
    const float* getReadPointer(){
        return buffer.getReadPointer(0);
    }
    
    //Read before pushing.
    //Otherwise getSample(0) will return the last value in the buffer.
    void push(float sample){
        buffer.setSample(0, writeIndex, sample);
        
        increment();
    }
    
    float getInterSample(float delaySamples){
        int delay0 = int (delaySamples);
        int frac = delaySamples - float (delay0);
        int rise = getSample(delay0 + 1) - getSample(delay0);
        return rise * frac;
    }
    
    float getSample(int delaySamples){
        int readIndex = writeIndex - delaySamples;
        if (readIndex < 0){
            readIndex = buffer.getNumSamples() + readIndex;
        }
        return buffer.getSample(0, readIndex);
    }
    
private:
    int writeIndex = 0;
    
    void increment(){
        writeIndex += 1;
        writeIndex %= buffer.getNumSamples();
    }
};

} // namespace stm
