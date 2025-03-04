// Global variables
let GLOBAL_LANGUAGE;

// ================================================================================================
// Initialization once the DOM has finished loading
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Apply language
    applyLanguage();

});

// ================================================================================================
// Apply interface language based on the browser settings
// ================================================================================================
function applyLanguage(selectedLanguage) {

    // If no language was provided
    if (selectedLanguage === undefined) {

        // Get the first two letters of the browser language code
        selectedLanguage = navigator.language.slice(0, 2);

    }

    // Check if language is available
    if (GLOBAL_STRINGS[selectedLanguage]) {

        // Set language based on browser language
        GLOBAL_LANGUAGE = GLOBAL_STRINGS[selectedLanguage];

        // Change the html tag language value
        document.documentElement.lang = selectedLanguage;
    
    // If language is not available
    } else {

        // Default to English
        GLOBAL_LANGUAGE = GLOBAL_STRINGS["en"];

    }
    
    // Apply strings for the selected language
    document.title = GLOBAL_LANGUAGE.title;
    document.getElementById("header-title").innerHTML = GLOBAL_LANGUAGE.header_title;
    document.getElementById("interface-cards-title").innerHTML = GLOBAL_LANGUAGE.interface_cards_title;
    document.getElementById("interface-card-geiger-counter-title").innerHTML = GLOBAL_LANGUAGE.interface_card_geiger_counter_title;
    document.getElementById("interface-card-geiger-counter-description").innerHTML = GLOBAL_LANGUAGE.interface_card_geiger_counter_description;
    document.getElementById("interface-card-cosmic-ray-detector-title").innerHTML = GLOBAL_LANGUAGE.interface_card_cosmic_ray_detector_title;
    document.getElementById("interface-card-cosmic-ray-detector-description").innerHTML = GLOBAL_LANGUAGE.interface_card_cosmic_ray_detector_description;
    document.getElementById("interface-card-wifi-title").innerHTML = GLOBAL_LANGUAGE.interface_card_wifi_title;
    document.getElementById("interface-card-wifi-description").innerHTML = GLOBAL_LANGUAGE.interface_card_wifi_description;
    document.getElementById("api-endpoints-title").innerHTML = GLOBAL_LANGUAGE.api_endpoints_title;

}