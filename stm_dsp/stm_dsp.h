/** 
     BEGIN_JUCE_MODULE_DECLARATION

      ID:               stm_dsp
      vendor:           Spenser Saling
      version:          1.0.0
      name:             Sailing To Mars DSP Module
      description:      A Digital Signal Processing Module for JUCE
      website:          https://sailingtomars.com/
      license:          MIT

      dependencies:     juce_core, juce_audio_basics, juce_dsp, stm_gui

     END_JUCE_MODULE_DECLARATION
*/

#pragma once

#include "processors/stm_DelaySimple.h"
#include "processors/stm_DCFilter.h"
#include "processors/stm_Saturation.h"
#include "processors/stm_AutoGain.h"
#include "processors/stm_MatchedBypass.h"
#include "processors/stm_DryWetMix.h"

#include "utilities/stm_Measuring.h"
#include "utilities/stm_Rampers.h"
#include "utilities/stm_RecircBuffer.h"
#include "utilities/stm_RollingRMS.h"
