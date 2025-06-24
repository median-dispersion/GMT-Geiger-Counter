// ================================================================================================
// Initialize web app
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Get DOM elements
    const element = {
        
        document:                     document.querySelector("html"),
        documentTitle:                document.querySelector("title"),
        headerTitle:                  document.querySelector("#header-title"),
        webInterfaces:                document.querySelector("#content-title-web-interfaces"),
        geigerCounterTitle:           document.querySelector("#content-card-info-geiger-counter").querySelector(".content-card-info-title"),
        geigerCounterDescription:     document.querySelector("#content-card-info-geiger-counter").querySelector(".content-card-info-description"),
        cosmicRayDetectorTitle:       document.querySelector("#content-card-info-cosmic-ray-detector").querySelector(".content-card-info-title"),
        cosmicRayDetectorDescription: document.querySelector("#content-card-info-cosmic-ray-detector").querySelector(".content-card-info-description"),
        wifiSettingsTitle:            document.querySelector("#content-card-info-wifi-settings").querySelector(".content-card-info-title"),
        wifiSettingsDescription:      document.querySelector("#content-card-info-wifi-settings").querySelector(".content-card-info-description"),
        restAPIEndpoints:             document.querySelector("#content-title-api-endpoints"),
        wifiCredentials:              document.querySelector("#content-endpoint-wifi-credentials"),
        restart:                      document.querySelector("#content-endpoint-restart")

    };
    
    // Selected language based on the browser language or fall back to english
    const selectedLanguage = navigator.language.slice(0, 2) in language ? navigator.language.slice(0, 2) : "en";

    // Set language strings to the selected language
    string = language[selectedLanguage];

    // Set the document language
    element.document.setAttribute("lang", selectedLanguage);

    // Translate DOM element
    element.documentTitle.innerHTML                = string.document_title;
    element.headerTitle.innerHTML                  = string.gmt_geiger_counter;
    element.webInterfaces.innerHTML                = string.web_interfaces;
    element.geigerCounterTitle.innerHTML           = string.geiger_counter;
    element.geigerCounterDescription.innerHTML     = string.geiger_counter_description;
    element.cosmicRayDetectorTitle.innerHTML       = string.cosmic_ray_detector;
    element.cosmicRayDetectorDescription.innerHTML = string.cosmic_ray_detector_description;
    element.wifiSettingsTitle.innerHTML            = string.wifi_settings;
    element.wifiSettingsDescription.innerHTML      = string.wifi_settings_message;
    element.restAPIEndpoints.innerHTML             = string.rest_api_endpoints;

    // Initialize components
    headerBar.initialize();
    dialogBox.initialize();

    // Add an event listener for displaying an info dialog about the WiFi credentials endpoint
    element.wifiCredentials.addEventListener("click", () => dialogBox.notify(

        string.wifi_credentials,
        string.wifi_credentials_message,
        [{title: string.ok, event: () => dialogBox.dismiss()}]

    ));

    // Add an event listener for displaying an info dialog about the restart endpoint
    element.restart.addEventListener("click", () => dialogBox.notify(

        // Dialog box title and message
        string.restart_system,
        string.restart_system_message,

        [
            
            // Button for restarting the system
            {title: string.yes, event: () => {

                // Restart system API endpoint url
                const url = `${window.location.origin}/system/restart`;

                // Restart the system
                fetch(url, {method: "PUT", cache: "no-store"});

                // Dismiss the dialog box
                dialogBox.dismiss();

            }},

            // Button for not restarting the system
            {title: string.no,  event: () => dialogBox.dismiss()}
        
        ]

    ));

});