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

    },

    // German
    de: {

        document_title:              "Systeminformationen | GMT Geiger Counter",
        system_info:                 "Systeminformationen",
        system_uptime:               "Systemlaufzeit",
        day_abbreviation:            "t",
        since_power_on:              "seit dem Einschalten",
        memory_usage:                "Speichernutzung",
        bytes_of_ram_free:           "bytes Arbeitsspeicher ungenutzt",
        system_components:           "Systemkomponenten",
        sd_card_mounted:             "SD-Karte eingehängt",
        geiger_counter_enabled:      "Geigerzähler aktiviert",
        cosmic_ray_detector_enabled: "Kosmischer Strahlungsdetektor aktiviert",
        buzzer_active:               "Lautsprecher aktiv",
        touchscreen_active:          "Touchscreen aktiv",
        rgb_led_active:              "RGB LED aktiv",
        hotspot_enabled:             "Hotspot aktiviert",
        wifi_enabled:                "WLAN aktiviert",
        http_server_running:         "HTTP-Server aktiv",
        firmware:                    "Firmware",
        data_request_failed:         "Datenübertragung fehlgeschlagen!",
        data_request_failed_message: "Die Anfrage der benötigten Daten ist fehlgeschlagen! Bitte überprüfen Sie die WLAN-Verbindung.",
        go_back:                     "Zurück",
        refresh:                     "Neuladen"

    }

};

// Set strings based on the browser language or fall back to english
let string = language[navigator.language.slice(0, 2)] || language["en"];