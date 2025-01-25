#ifndef _STRINGS_H
#define _STRINGS_H

#include "Arduino.h"
#include "Configuration.h"

#define LANGUAGE_ENGLISH 1

#if INTERFACE_LANGUAGE == LANGUAGE_ENGLISH
  #include "StringsEnglish.h"
#endif

#endif