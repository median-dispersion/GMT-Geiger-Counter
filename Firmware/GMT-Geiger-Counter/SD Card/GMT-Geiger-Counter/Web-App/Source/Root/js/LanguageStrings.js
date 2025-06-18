// Strings in different languages
const language = {

    // English
    en: {

        document_title:                  "GMT Geiger Counter",
        gmt_geiger_counter:              "GMT Geiger Counter",
        web_interfaces:                  "Web interfaces",
        geiger_counter:                  "Geiger counter",
        geiger_counter_description:      "Get and visualize the Geiger counter data via the main web interface.",
        cosmic_ray_detector:             "Cosmic ray detector",
        cosmic_ray_detector_description: "Get and visualize the cosmic ray detector data via the web interface.",
        wifi_settings:                   "WiFi settings",
        wifi_settings_message:           "Update the WiFi access point the Geiger counter tries to connect to by changing its credentials.",
        rest_api_endpoints:              "REST API endpoints",
        wifi_credentials:                "WiFi Credentials",
        wifi_credentials_message:        "A PUT API endpoint for updating the WiFi credentials.",
        ok:                              "OK",
        restart_system:                  "Restart system?",
        restart_system_message:          "A PUT API endpoint for restarting the system. Restart the system now? Keep in mind that restarting the system will reset all data that has been recorded so far.",
        yes:                             "Yes",
        no:                              "No"

    },

    // German
    de: {

        document_title:                  "GMT Geiger Counter",
        gmt_geiger_counter:              "GMT Geiger Counter",
        web_interfaces:                  "Weboberflächen",
        geiger_counter:                  "Geigerzähler",
        geiger_counter_description:      "Weboberfläche zum Erhalten und Darstellen der Geigerzählerdaten.",
        cosmic_ray_detector:             "Kosmischer Strahlungsdetektor",
        cosmic_ray_detector_description: "Weboberfläche zum Erhalten und Darstellen der kosmischen Strahlungsdaten.",
        wifi_settings:                   "WLAN-Einstellungen",
        wifi_settings_message:           "Ändern des WLAN-Zugangspunktes, mit dem sich der Geigerzähler verbindet.",
        rest_api_endpoints:              "REST API Endpunkte",
        wifi_credentials:                "WLAN-Anmeldeinformationen",
        wifi_credentials_message:        "Ein PUT API Endpunkt zum Aktualisieren der WLAN Anmeldeinformationen.",
        ok:                              "OK",
        restart_system:                  "System neu starten?",
        restart_system_message:          "Ein PUT-API-Endpunkt zum Neustart des Systems. Soll das System jetzt neu gestartet werden? Beachten Sie, dass beim Neustart des Systems alle bisher aufgezeichneten Daten zurückgesetzt werden.",
        yes:                             "Ja",
        no:                              "Nein"

    }

};

// Set strings based on the browser language or fall back to english
let string = language[navigator.language.slice(0, 2)] || language["en"];