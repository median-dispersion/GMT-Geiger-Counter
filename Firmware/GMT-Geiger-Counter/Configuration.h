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
// This can be set to an arbitrary string and is only used for logging
#define TUBE_TYPE_NAME "SBM-20"

// The default integration time the Geiger counter starts at
// The integration time is the number of seconds the Geiger counter will average over to calculate the CPM value
// A shorter integration time gives more accurate values for high levels of radiation, a lower in integration time for lower levels
// This can be adjusted during runtime and is just the default value
#define INTEGRATION_TIME_DEFAULT_SECONDS 30

// The number of minutes the radiation history screen displays
// For a longer history graph increase the number of minutes
// Range: 5 - 250 minutes
#define RADIATION_HISTORY_LENGTH_MINUTES 20

// The minimum scale of the counts per minutes axis on the radiation history screen
// If a value larger than this is recorded, the axis will scale automatically to that new value
// This is just for setting the minimum scale it can go down to
#define RADIATION_HISTORY_MINIMUM_SCALE_CPM 32

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

// Time until the display automatically turns off in seconds
// If the auto timeout is enabled, the display will turn off after the specified time
// For longer screen on time, increase this value
// Default: 120 (2 min)
#define DISPLAY_AUTO_TIMEOUT_SECONDS 120

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

// Hotspot name / SSID
// The name of the wireless hotspot the Geiger counter will create for wireless communication
#define HOTSPOT_NAME "GMT-Geiger-Counter"

// Hotspot password
// The password of the wireless hotspot the Geiger counter will create for wireless communication
// This must be at least 8 characters long!
#define HOTSPOT_PASSWORD "Change-This-Immediately!1!1!"

// WiFi name / SSID
// The name of the default WiFi that the Geiger counter will try to connect to
// This value is only used for the initial flash and can later be changed during runtime
#define WIFI_NAME "Your-WiFi-Name"

// WiFi password
// The password of the default WiFi that the Geiger counter will try to connect to
// This value is only used for the initial flash and can later be changed during runtime
#define WIFI_PASSWORD "Your-WiFi-Password"

// ================================================================================================
// System setting
// ================================================================================================

// Interface language
// Available options:
// LANGUAGE_ENGLISH
#define INTERFACE_LANGUAGE LANGUAGE_ENGLISH

// Log interval in seconds
// How often the data should be logged to the serial console and SD card (if available)
// This should be treated as long-term data acquisition and therefore should have a relatively high value < 1 min
// Default: 60
#define LOG_INTERVAL_SECONDS 60

// The maximum log file size in bytes
// If the log file reaches a size larger than this, it will be split up into parts
// This value should not be changed!
// Default: 8388608 (8 MiB)
#define LOG_FILE_MAXIMUM_SIZE_BYTES 8388608

// The root directories on the SD card the Geiger counter will look for
// This is used for all applications that require storage, like logging or the web interface
// The root directory will be used to detect if an SD card is present
// The specific subdirectories are based on the root directory
// These values should not be changed!
#define SD_CARD_ROOT_DIRECTORY     "/GMT-Geiger-Counter"
#define SD_CARD_LOG_DIRECTORY      SD_CARD_ROOT_DIRECTORY"/Logs"
#define SD_CARD_WEB_APP_DIRECTORY  SD_CARD_ROOT_DIRECTORY"/Web-App"

// Baud rate for serial communication
// This value should not be changed!
// Default: 115200
#define SERIAL_BAUD_RATE 115200

// Firmware version
// This can be set to any arbitrary string
// This value should not be changed!
#define FIRMWARE_VERSION "GMTGC-2025-02-27"

// The minimum threshold of free heap the system is allowed to have
// If the free heap falls below this value, the system will reboot to prevent software instability or lock-ups
// This value should not be changed!
// Default: 16384 (16 KiB)
#define WATCHDOG_MINIMUM_HEAP_BYTES 16384

// The minimum threshold of the largest allocatable block of heap the system is allowed to have
// If the block size falls below this value, the system will reboot to prevent software instability or lock-ups
// This value should not be changed!
// Default: 4096 (4 KiB)
#define WATCHDOG_MINIMUM_HEAP_BLOCK_BYTES 4096

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