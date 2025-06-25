// ================================================================================================
// Initialize web app
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // DOM elements
    const element = {

        document:           document.querySelector("html"),
        documentTitle:      document.querySelector("title"),
        headerTitle:        document.querySelector("#header-title"),
        systemUptime:       document.querySelector("#content-title-system-uptime"),
        powerOnDescription: document.querySelector("#content-state-info-uptime").querySelector(".content-state-info-description"),
        memoryUsage:        document.querySelector("#content-title-memory-usage"),
        ramFreeDescription: document.querySelector("#content-state-info-memory").querySelector(".content-state-info-description"),
        systemComponents:   document.querySelector("#content-title-system-components"),
        sdCard:             document.querySelector("#content-components-entry-sd-card").querySelector(".content-components-entry-title"),
        geigerCounter:      document.querySelector("#content-components-entry-geiger-counter").querySelector(".content-components-entry-title"),
        cosmicRayDetector:  document.querySelector("#content-components-entry-cosmic-ray-detector").querySelector(".content-components-entry-title"),
        buzzer:             document.querySelector("#content-components-entry-buzzer").querySelector(".content-components-entry-title"),
        touchscreen:        document.querySelector("#content-components-entry-touchscreen").querySelector(".content-components-entry-title"),
        led:                document.querySelector("#content-components-entry-led").querySelector(".content-components-entry-title"),
        hotspot:            document.querySelector("#content-components-entry-hotspot").querySelector(".content-components-entry-title"),
        wifi:               document.querySelector("#content-components-entry-wifi").querySelector(".content-components-entry-title"),
        server:             document.querySelector("#content-components-entry-server").querySelector(".content-components-entry-title")

    };

    // Selected language based on the browser language or fall back to english
    const selectedLanguage = navigator.language.slice(0, 2) in language ? navigator.language.slice(0, 2) : "en";

    // Set language strings to the selected language
    string = language[selectedLanguage];

    // Set the document language
    element.document.setAttribute("lang", selectedLanguage);

    // Translate DOM elements
    element.documentTitle.innerHTML      = string.document_title;
    element.headerTitle.innerHTML        = string.system_info;
    element.systemUptime.innerHTML       = string.system_uptime;
    element.powerOnDescription.innerHTML = string.since_power_on;
    element.memoryUsage.innerHTML        = string.memory_usage;
    element.ramFreeDescription.innerHTML = string.bytes_of_ram_free;
    element.systemComponents.innerHTML   = string.system_components;
    element.sdCard.innerHTML             = string.sd_card_mounted;
    element.geigerCounter.innerHTML      = string.geiger_counter_enabled;
    element.cosmicRayDetector.innerHTML  = string.cosmic_ray_detector_enabled;
    element.buzzer.innerHTML             = string.buzzer_active;
    element.touchscreen.innerHTML        = string.touchscreen_active;
    element.led.innerHTML                = string.rgb_led_active;
    element.hotspot.innerHTML            = string.hotspot_enabled;
    element.wifi.innerHTML               = string.wifi_enabled;
    element.server.innerHTML             = string.http_server_running;

    // Initialize components
    systemInfo.initialize();
    headerBar.initialize();
    dialogBox.initialize();

});