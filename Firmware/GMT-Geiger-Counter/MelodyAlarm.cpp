#include "Melody.h"

const NonBlockingMelody::Note notes[2] = {
  
  {BUZZER_FREQUENCY_HERTZ, 200},
  {0, 50}
  
};

Melody MELODY_ALARM = {notes, 2, 1};