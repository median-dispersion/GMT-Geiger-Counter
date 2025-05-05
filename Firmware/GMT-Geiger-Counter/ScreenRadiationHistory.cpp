#include "ScreenRadiationHistory.h"

//-------------------------------------------------------------------------------------------------
// Public

// ================================================================================================
// Constructor
// ================================================================================================
ScreenRadiationHistory::ScreenRadiationHistory():

  // Initialize members
  ScreenBasicLandscape(STRING_RADIATION_HISTORY_CPM),
  _historyIndex(0),
  _timeSteps(round(RADIATION_HISTORY_LENGTH_MINUTES / 4.0)),
  _countSteps(round(RADIATION_HISTORY_MINIMUM_SCALE_CPM / 4.0)),
  _averageCountsPerMinuteString("0"),
  _maximumCountsPerMinuteString("0"),
  _minimumCountsPerMinuteString("0"),
  _averageCountsPerMinute(  2, 213, 105, IMAGE_AVERAGE, _averageCountsPerMinuteString.c_str()),
  _maximumCountsPerMinute(108, 213, 105, IMAGE_MAXIMUM, _maximumCountsPerMinuteString.c_str()),
  _minimumCountsPerMinute(214, 213, 104, IMAGE_MINIMUM, _minimumCountsPerMinuteString.c_str())

{}

// ================================================================================================
// Update
// ================================================================================================
void ScreenRadiationHistory::update(const XPT2046::Point &position) {

  // Update the basic screen
  ScreenBasicLandscape::update(position);

}

// ================================================================================================
// Draw
// ================================================================================================
void ScreenRadiationHistory::draw(GFXcanvas16 &canvas) {

  // Draw the basic screen
  ScreenBasicLandscape::draw(canvas);

  // Draw background
  canvas.fillRect(2, 31, 316, 181, COLOR_GRAY_DARK);

  // Draw grid
  canvas.drawFastVLine( 86,  42, 159, COLOR_GRAY_LIGHT);
  canvas.drawFastVLine(160,  42, 159, COLOR_GRAY_LIGHT);
  canvas.drawFastVLine(234,  42, 159, COLOR_GRAY_LIGHT);
  canvas.drawFastVLine(307,  42, 159, COLOR_GRAY_LIGHT);
  canvas.drawFastHLine( 13,  41, 295, COLOR_GRAY_LIGHT);
  canvas.drawFastHLine( 13,  81, 294, COLOR_GRAY_LIGHT);
  canvas.drawFastHLine( 13, 121, 294, COLOR_GRAY_LIGHT);
  canvas.drawFastHLine( 13, 161, 294, COLOR_GRAY_LIGHT);

  // Set the font family, size and color
  canvas.setFont(&FreeSans9pt7b);
  canvas.setTextColor(COLOR_GRAY_LIGHT);

  // Draw time steps
  canvas.setCursor(236, 199);
  canvas.print(_timeSteps * 1);
  canvas.setCursor(162, 199);
  canvas.print(_timeSteps * 2);
  canvas.setCursor(88, 199);
  canvas.print(_timeSteps * 3);
  canvas.setCursor(14, 199);
  canvas.print(_timeSteps * 4);

  // Draw counts steps
  canvas.setCursor(14, 177);
  canvas.print(_countSteps * 1);
  canvas.setCursor(14, 137);
  canvas.print(_countSteps * 2);
  canvas.setCursor(14, 97);
  canvas.print(_countSteps * 3);
  canvas.setCursor(14, 57);
  canvas.print(_countSteps * 4);

  // Calculate with and height of line elements
  float width  = 294.0 / (RADIATION_HISTORY_LENGTH_MINUTES - 1);
  float height = 159.0 / (_countSteps * 4); 

  // Get starting position of the first line element
  uint16_t x = 306;
  uint16_t y = 200 - round(height * _history[(_historyIndex + RADIATION_HISTORY_LENGTH_MINUTES - 1) % RADIATION_HISTORY_LENGTH_MINUTES]); 

  // For all samples in the history array
  for (uint8_t sample = 0; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

    // Calculate wrapped index from the current history index offset by the sample + 1
    uint8_t wrappedIndex = (_historyIndex + RADIATION_HISTORY_LENGTH_MINUTES - (sample + 1)) % RADIATION_HISTORY_LENGTH_MINUTES;

    // If the sample is not invalid
    if (_history[wrappedIndex] != UINT32_MAX) {

      // Calculate end position of line element
      uint16_t dx = 306 - round(width  * sample);
      uint16_t dy = 200 - round(height * _history[wrappedIndex]);

      // Draw line between the two positions
      canvas.drawLine(x, y, dx, dy, COLOR_NEON);

      // Set the next starting position to the current end position
      x = dx;
      y = dy;

    }

  }

  // Draw axis
  canvas.drawFastVLine(12,  41, 161, COLOR_WHITE);
  canvas.drawFastHLine(12, 201, 296, COLOR_WHITE);
  canvas.fillTriangle(  7,  49,  12,  41,  17,  49, COLOR_WHITE);
  canvas.fillTriangle(299, 196, 307, 201, 299, 206, COLOR_WHITE);

  // Draw screen elements
  _averageCountsPerMinute.draw(canvas);
  _maximumCountsPerMinute.draw(canvas);
  _minimumCountsPerMinute.draw(canvas);

}

// ================================================================================================
// Update the radiation history
// ================================================================================================
void ScreenRadiationHistory::setRadiationHistory(uint32_t *history, const uint8_t index) {

  // Set the radiation history
  _history = history;

  // If the radiation history index has updated
  if (_historyIndex != index){

    // Update the history index
    _historyIndex = index;

    // Reset count variables
    double   averageCountsPerMinute = 0.0;
    uint32_t maximumCountsPerMinute = 0;
    uint32_t minimumCountsPerMinute = UINT32_MAX;
    uint8_t  validSamples           = 0;

    // For all samples in the history array
    for (uint8_t sample = 0; sample < RADIATION_HISTORY_LENGTH_MINUTES; sample++) {

      // Only update count variables if not an invalid count value
      if (_history[sample] != UINT32_MAX) {

        // Add sample to the sum of a counts per minute values
        averageCountsPerMinute += _history[sample];

        // If this sample is larger than the previous maximum set the maximum to it
        if (_history[sample] > maximumCountsPerMinute) { maximumCountsPerMinute = _history[sample]; }
        
        // If this sample is smaller than the previous minimum set the minimum to it
        if (_history[sample] < minimumCountsPerMinute) { minimumCountsPerMinute = _history[sample]; }

        // Increase the number of valid samples
        validSamples++;

      }

    }

    // If there were any valid samples
    if (validSamples) {

      // Calculate average
      averageCountsPerMinute = round(averageCountsPerMinute / validSamples);

      // If the maximum counts per minute value is less than 90% of the minimum history scale
      if (maximumCountsPerMinute * 1.1 < RADIATION_HISTORY_MINIMUM_SCALE_CPM) {
        
        // Set the count step size to the minimum history scale
        _countSteps = round(RADIATION_HISTORY_MINIMUM_SCALE_CPM / 4.0);

      // If If the maximum counts per minute value is more then the minimum history scale
      } else {

        // Scale the history scale accordingly
        _countSteps = round((maximumCountsPerMinute * 1.1) / 4.0);

      }

      // Update strings
      _averageCountsPerMinuteString = (uint32_t)(averageCountsPerMinute);
      _maximumCountsPerMinuteString = maximumCountsPerMinute;
      _minimumCountsPerMinuteString = minimumCountsPerMinute;

    }

  }

}