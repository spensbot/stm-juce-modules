#pragma once

using namespace juce;

namespace stm {

/**
 Single-producer (audio thread)
 Single-consumer (gui thread)
 */
class RingBuffer {
public:
    RingBuffer() {
        
    }
    
    prepare(int numSamples, numChannels){
        
    }
    
    /**
     Adds a value to the buffer. Don't forget to call increment afterwards.
     Also
     */
    push(float val) {
        int pos = writePosition.load();
        
        if (++pos == buffer.getNumSamples()){
            writePosition.store(0);
        } else {
            writePosition.store(pos);
        }
    }
    
    reset(){
        
    }
    
    
    
    
private:
    std::atomic<int> writePosition = 0;
    
    AudioSampleBuffer buffer;
};


} //namespace stm
