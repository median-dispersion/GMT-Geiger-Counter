// The main configuration file for the GMT-Geiger-Counter firmware

// ================================================================================================
// Geiger counter / tube settings
// ================================================================================================

// Enable pulse counting for a specific tube header
// If only one tube is connected, disable the follower tube pin header
// If two or more tubes are connected, split them evenly and enable both pin headers
#define ENABLE_MAIN_TUBE     1
#define ENABLE_FOLLOWER_TUBE 1

// The total number of tubes connected to the motherboard (both main and follower)
// This will be used to divide the equivalent dose by the number of tubes to get an accurate reading
#define TOTAL_NUMBER_OF_TUBES 1

// CURRENTLY NOT USED, PLEASE IGNORE FOR NOW!
// The average length of a pulse in microseconds
// This is not the tube's dead time, but the length of the pulse the tube driver board generates
// To get an accurate value for a specific tube type, use the Calibrate-Average-Pulse-Length firmware to measure it
// SBM-20: 115 µs
#define TUBE_PULSE_LENGTH_MICROSECONDS 115

// Noise threshold, the minimum length in microseconds a pulse must be to count as an actual pulse
// Anything shorter than that will be ignored as noise
// This should be around 30% of the pulse length in microseconds
#define TUBE_NOISE_THRESHOLD_MICROSECONDS 30

// Conversion factor to convert counts per minute to microsieverts per hour
// This is different for each type of tube and must be calculated from the datasheet
// SMB-20: 0.008095385
#define TUBE_CONVERSION_FACTOR_CPM_TO_USVH 0.008095385

// Name of the tube type
// This can be set to an arbitrary value and is only used for logging
#define TUBE_TYPE_NAME "SBM-20"

// The default integration time the Geiger counter starts at
// The integration time is the number of seconds the Geiger counter will average over to calculate the CPM value
// A shorter integration time gives more accurate values for high levels of radiation, a lower in integration time for lower levels
#define INTEGRATION_TIME_DEFAULT_SECONDS 30

// Radiation rating values in microsieverts per hour
// These values are used to classify the recorded radiation level
#define RADIATION_RATING_NORMAL_USVH   0.0
#define RADIATION_RATING_ELEVATED_USVH 2.0
#define RADIATION_RATING_HIGH_USVH     10.0
#define RADIATION_RATING_EXTREME_USVH  20.0

// ================================================================================================
// Buzzer / audio settings
// ================================================================================================

// The buzzers base frequency in Hertz
// This should be provided by the buzzer manufacture
// Most buzzers have a base frequency of around 2000 Hertz
#define BUZZER_FREQUENCY_HERTZ 2400

// The radiation level at which the buzzer starts playing the alert sounds
// By default, they are tied to the radiation rating but can be set to any value in microsieverts per hour
#define BUZZER_WARNING_LEVEL_USVH RADIATION_RATING_ELEVATED_USVH
#define BUZZER_ALARM_LEVEL_USVH   RADIATION_RATING_HIGH_USVH

// ================================================================================================
// Touchscreen / display setting
// ================================================================================================

// The touchscreen calibration matrix
#define TOUCH_CALIBRATION {0.0893660,-0.0004446,-31.8194923,-0.0002766,0.0646709,-12.4398527,320,240,3}

// The number of touch samples the touchscreen takes before returning a touch position
// For more accurate touch readings, a higher value should be used
// This value should not be changed!
// Default: 20
#define TOUCH_SAMPLE_COUNT 20

// The time the touchscreen waits before accepting a new touch event
// This can help mitigate erroneous double touch events when lifting from the touchscreen
// This value should not be changed!
// Default: 100
#define TOUCH_DEBOUNCE_TIMEOUT_MILLISECONDS 100

// The pressure threshold required before registering a touch event
// A lower value means higher pressure needs to be applied before counting as a touch event
// This value should not be changed!
// Default: 3.5
#define TOUCH_PRESSURE_THRESHOLD 3.5

// The display width and height in pixels
// These values should not be changed!
// Default: 240 x 320
#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320

// Landscape screen rotation value
// This is the rotation value for the landscape screen orientation
// This value should not be changed!
// Default: 3
#define DISPLAY_SCREEN_ROTATION_LANDSCAPE 3

// Portrait screen rotation value
// This is the rotation value for the portrait screen orientation
// This value should not be changed!
// Default: 2
#define DISPLAY_SCREEN_ROTATION_PORTRAIT 2

// The displays refresh interval
// This is the time in milliseconds the display waits before drawing a new frame
// This value should not be changed!
// Default: 500
#define DISPLAY_REFRESH_INTERVAL_MILLISECONDS 500

// ================================================================================================
// Wireless setting
// ================================================================================================

// Hotspot Name / SSID
#define HOTSPOT_NAME "GMT-Geiger-Counter"

// Hotspot password
#define HOTSPOT_PASSWORD "Changed-This-Immediately!1!1!"

// ================================================================================================
// System setting
// ================================================================================================

// Interface language
// Available options:
// LANGUAGE_ENGLISH
#define INTERFACE_LANGUAGE LANGUAGE_ENGLISH

// Baud rate for serial communication
// This value should not be changed!
// Default: 115200
#define SERIAL_BAUD_RATE 115200

// Firmware version
// This can be set to any arbitrary string
// This value should not be changed!
#define FIRMWARE_VERSION "GTMGC-2025-01-27"

// ================================================================================================
// Pin assignments
// ================================================================================================

// These values should not be changed!
#define MOSI_PIN            11
#define MISO_PIN            13
#define SCK_PIN             12
#define DISPLAY_LED_PIN     9
#define DISPLAY_RST_PIN     8
#define DISPLAY_DC_PIN      3
#define DISPLAY_CS_PIN      18
#define TOUCH_CS_PIN        10
#define TOUCH_IRQ_PIN       14
#define BUZZER_PIN          47
#define SD_CS_PIN           17
#define MAIN_TRG_PIN        42
#define FOLLOWER_TRG_PIN    41
#define COINCIDENCE_TRG_PIN 40