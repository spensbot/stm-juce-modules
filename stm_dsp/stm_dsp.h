/** 
     BEGIN_JUCE_MODULE_DECLARATION

      ID:               stm_dsp
      vendor:           Spenser Saling
      version:          1.0.0
      name:             Sailing To Mars DSP Module
      description:      A Digital Signal Processing Module for JUCE
      website:          https://sailingtomars.com/
      license:          MIT

      dependencies:     juce_core, juce_audio_basics, juce_dsp

     END_JUCE_MODULE_DECLARATION
*/

#pragma once

#include "rampers/stm_Rampers.h"
#include "buffers/stm_RecircBuffer.h"
#include "processors/stm_Delay.h"
#include "processors/stm_DCFilter.h"
#include "processors/stm_Saturation.h"
