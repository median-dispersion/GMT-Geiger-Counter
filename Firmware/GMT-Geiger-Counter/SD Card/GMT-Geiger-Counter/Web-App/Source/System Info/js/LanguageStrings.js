// Strings in different languages
const language = {

    // English
    en: {

        document_title:              "System info | GMT Geiger Counter",
        system_info:                 "System info",
        system_uptime:               "System uptime",
        day_abbreviation:            "d",
        since_power_on:              "since power on",
        memory_usage:                "Memory usage",
        bytes_of_ram_free:           "bytes of RAM free",
        system_components:           "System components",
        sd_card_mounted:             "SD card mounted",
        geiger_counter_enabled:      "Geiger counter enabled",
        cosmic_ray_detector_enabled: "Cosmic ray detector enabled",
        buzzer_active:               "Buzzer active",
        touchscreen_active:          "Touchscreen active",
        rgb_led_active:              "RGB LED active",
        hotspot_enabled:             "Hotspot enabled",
        wifi_enabled:                "WiFi enabled",
        http_server_running:         "HTTP server running",
        firmware:                    "Firmware",
        data_request_failed:         "Data request failed!",
        data_request_failed_message: "Requesting the required data failed! Please check the wireless connection.",
        go_back:                     "Go back",
        refresh:                     "Refresh",

    }

};

// Set strings based on the browser language or fall back to english
let string = language[navigator.language.slice(0, 2)] || language["en"];