#include "Watchdog.h"

// ------------------------------------------------------------------------------------------------
// Public

// Initialize global reference
Watchdog& watchdog = Watchdog::getInstance();

// ================================================================================================
// Get the single instance of the class
// ================================================================================================
Watchdog& Watchdog::getInstance() {

  // Get the single instance
  static Watchdog instance;

  // Return the instance
  return instance;

}

// ================================================================================================
// Initialize everything
// ================================================================================================
void Watchdog::begin() {

  // If not initialized
  if (!_initialized) {

    // Set the initialization flag
    _initialized = true;

    // Check if PSRAM is enabled
    _psram = psramFound();

  }

}

// ================================================================================================
// Update the watchdog
// ================================================================================================
void Watchdog::update() {

  // If the free heap falls below the minimum
  // Or the largest block size that can be allocated falls below the minumum
  // Or if the system has PSRAM and the free amount of PSRAM falls below the minimum
  // Restart the system to prevent software instability or lock ups
  if (

    ESP.getFreeHeap() < WATCHDOG_MINIMUM_HEAP_BYTES || 
    ESP.getMaxAllocHeap() < WATCHDOG_MINIMUM_HEAP_BLOCK_BYTES ||
    (_psram && ESP.getPsramSize() < WATCHDOG_MINIMUM_PSRAM_BYTES)

  ) {

    // OH SHIT, OH FUCK, OH SHIT, OH FUCK, NO MORE MEMORY REBOOT NOW!!!!
    ESP.restart();

  }

}

// ------------------------------------------------------------------------------------------------
// Private

Watchdog::Watchdog():

  // Initialize members
  _initialized(false),
  _psram(false)

{}