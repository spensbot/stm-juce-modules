#pragma once
#include "stm_RecircBuffer.h"
#include <math.h>

namespace stm {

/** Similar to rolling average, but the average reacts differently to values above/below the current average. */
class RollingAverageBiased {

public: 
    void prepare(float riseSamples, float fallSamples) {
        weightUpNew = 1/riseSamples;
        weightUpOld = 1 - weightUpNew;
        weightDownNew = 1/fallSamples;
        weightDownOld = 1 - weightDownNew;
    }

    void prepare(float sampleRate, float riseSeconds, float fallSeconds){
        prepare(riseSeconds * sampleRate, fallSeconds * sampleRate);
    }

    void push(float sample) {
        if(sample > average){
            // Average moves up
            average = weightUpNew * sample + weightUpOld * average;
        } else {
            // Average moves down
            average = weightDownNew * sample + weightDownOld * average;
        }
    }

    float get() {
        return average;
    }

    void reset(){
        average = 0.0f;
    }

private: 
    float average = 0.0f;
    float weightUpNew = 1.0f;
    float weightUpOld = 1.0f;
    float weightDownNew = 0.0f;
    float weightDownOld = 0.0f;
};

class RollingAverage {
public:
    /**
     0.0 < forgetFactor < 1.0.
     0 never incorporates new samples.
     1 takes the current sample as the average.
     */
    void prepare(float forgetFactor){
        newWeight = forgetFactor;
        oldWeight = 1 - newWeight;
    }
    
    void push(float sample){
        average = newWeight * sample + oldWeight * average;
    }
    
    float get(){
        return average;
    }
    
    void reset(){
        average = 0.0f;
    }
    
private:
    float average = 0.0f;
    float newWeight = 1.0f;
    float oldWeight = 0.0f;
};

/**
 A fast, not entirely accurate way of keeping a running tally of RMS that forgets old values.
 Since RMS is the sqrt of the average of the squared samples
 We just need to maintain a rolling average of the samples squares
 Then return the square root of that average whenever RMS is needed.
 */
class RollingRMS {
public:
    
    void prepare(double sampleRate, float sustainSeconds){
        float sustainSamples = sampleRate * sustainSeconds;
        float forgetFactor = 1.0f / sustainSamples;
        meanSquare.prepare(forgetFactor);
    }
    
    void push(float sample){
        meanSquare.push(sample * sample);
    }
    
    float get(){
        return sqrt( (double)meanSquare.get() );
    }
    
    void reset(){
        meanSquare.reset();
    }
    
private:
    RollingAverage meanSquare;
};

class RollingRMSBiased {
public:
    void prepare(double sampleRate, float riseSeconds, float fallSeconds){
        meanSquare.prepare(sampleRate, riseSeconds, fallSeconds);
    }

    void push(float sample) {
        meanSquare.push(sample * sample);
    }

    float get() {
        return sqrt( (double) meanSquare.get() );
    }

    void reset(){
        meanSquare.reset();
    }

private:
    RollingAverageBiased meanSquare;
};
        
} // namespace stm
       
        
        

//-----------------------------------------------
// My original RollingRMS implementation.
// I'm pretty sure floating point rounding error could become significant over time.
//-----------------------------------------------
        
//class RollingRMS {
//public:
//
//    void prepare(double sampleRate, float sustainSeconds){
//        length = (int) (sampleRate * sustainSeconds);
//        buffer.prepare(length);
//    }
//
//    void push(float sample){
//        //First, remove the oldest weighted square from the buffer
//        meanSquare -= buffer.getSample(0);
//        //Then, add the new s
//        float newWeightedSquare = getWeightedSquare(sample);
//        meanSquare += newWeightedSquare;
//        buffer.push(newWeightedSquare);
//
//        buffer.increment();
//    }
//
//    void getRMS(){
//        return sqrt(meanSquare);
//    }
//
//    void reset(){
//        rms = 0.0f;
//        buffer.reset();
//    }
//
//private:
//
//    float getWeightedSquare(float sample){
//        return (sample * sample) / length
//    }
//
//    RecircBuffer buffer;
//
//    int length = 1;
//    float meanSquare = 0.0f;
//};

