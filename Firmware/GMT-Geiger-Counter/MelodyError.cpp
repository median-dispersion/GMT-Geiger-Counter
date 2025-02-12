#include "Melody.h"

const NonBlockingMelody::Note notes[2] = {
  
  {BUZZER_FREQUENCY_HERTZ, 50},
  {0, 50},
  
};

Melody MELODY_ERROR = {notes, 2, 3};