#ifndef _MELODY_H
#define _MELODY_H

#include "Arduino.h"
#include "Configuration.h"
#include "NonBlockingMelody.h"

// Melody structure
struct Melody {

  const NonBlockingMelody::Note *notes;
  const uint16_t length;
  const uint16_t repeats;

};

// Melody data
extern Melody MELODY_ALARM;
extern Melody MELODY_BACK;
extern Melody MELODY_COINCIDENCE_EVENT;
extern Melody MELODY_DETECTION;
extern Melody MELODY_JINGLE;
extern Melody MELODY_NEXT;
extern Melody MELODY_TAP;
extern Melody MELODY_WARNING;

#endif