// Strings in different languages
const language = {

    // English
    en: {

        document_title:                       "Cosmic ray detector | GMT Geiger Counter",
        cosmic_ray_detector:                  "Cosmic ray detector",
        coincidence_events:                   "Coincidence events",
        counts_per_hour_abbreviation:         "CPH",
        coincidence_event_history:            "Coincidence event history",
        tube_statistics:                      "Tube statistics",
        coincidence_tube:                     "Coincidence tube",
        total_events:                         "Total events",
        main_tube:                            "Main tube",
        counts:                               "Counts",
        events:                               "Events",
        event_percentage:                     "Event percentage",
        follower_tube:                        "Follower tube",
        settings:                             "Settings",
        update_interval:                      "Update interval",
        refresh_rate:                         "Refresh rate",
        second_abbreviation:                  "s",
        event_history:                        "Event history",
        ignore_first_hour:                    "Ignore first hour",
        vertical_steps:                       "Vertical steps",
        horizontal_steps:                     "Horizontal steps",
        smoothing_factor:                     "Smoothing factor",
        save_history:                         "Save history",
        actions:                              "Actions",
        download_log:                         "Download log",
        restart_system_title:                 "Restart system",
        restart_system:                       "Restart system?",
        restart_system_message:               "Restart the system? Keep in mind that restarting the system will reset all data that has been recorded so far.",
        yes:                                  "Yes",
        no:                                   "No",
        data_request_failed:                  "Data request failed!",
        data_request_failed_message:          "Requesting the required data failed! Please check the wireless connection.",
        go_back:                              "Go back",
        refresh:                              "Refresh",
        minute_abbreviation:                  "m",
        hour_abbreviation:                    "h",
        cosmic_ray_detector_disabled:         "Cosmic ray detector is disabled!",
        cosmic_ray_detector_disabled_message: "The cosmic ray detector is disabled! Please enable it before using this feature."

    },

    // German
    de: {

        document_title:                       "Kosmischer Strahlungsdetektor | GMT Geiger Counter",
        cosmic_ray_detector:                  "Kosmischer Strahlungsdetektor",
        coincidence_events:                   "Zufallsereignisse",
        counts_per_hour_abbreviation:         "CPH",
        coincidence_event_history:            "Zufallsereignisverlauf",
        tube_statistics:                      "Röhrenstatistik",
        coincidence_tube:                     "Koinzidenzröhre",
        total_events:                         "Alle Ereignisse",
        main_tube:                            "Hauptröhre",
        counts:                               "Zählungen",
        events:                               "Ereignisse",
        event_percentage:                     "Ereignisprozentsatz",
        follower_tube:                        "Nebenröhre",
        settings:                             "Einstellungen",
        update_interval:                      "Updateintervall",
        refresh_rate:                         "Aktualisierungsrate",
        second_abbreviation:                  "s",
        event_history:                        "Ereignisverlauf",
        ignore_first_hour:                    "Erste Stunde ignorieren",
        vertical_steps:                       "Vertikale Schritte",
        horizontal_steps:                     "Horizontale Schritte",
        smoothing_factor:                     "Glättungsfaktor",
        save_history:                         "Verlauf speichern",
        actions:                              "Aktionen",
        download_log:                         "Logdatei downloaden",
        restart_system_title:                 "System neu starten",
        restart_system:                       "System neu starten?",
        restart_system_message:               "System neu starten? Beachten Sie, dass durch einen Neustart des Systems alle bisher aufgezeichneten Daten zurückgesetzt werden.",
        yes:                                  "Ja",
        no:                                   "Nein",
        data_request_failed:                  "Datenübertragung fehlgeschlagen!",
        data_request_failed_message:          "Die Anfrage der benötigten Daten ist fehlgeschlagen! Bitte überprüfen Sie die WLAN-Verbindung.",
        go_back:                              "Zurück",
        refresh:                              "Neuladen",
        minute_abbreviation:                  "m",
        hour_abbreviation:                    "h",
        cosmic_ray_detector_disabled:         "Kosmischer Strahlungsdetektor ist deaktiviert!",
        cosmic_ray_detector_disabled_message: "Der Kosmischer Strahlungsdetektor ist deaktiviert! Bitte aktivieren Sie ihn, bevor Sie diese Funktion verwenden."

    }

};

// Set strings based on the browser language or fall back to english
let string = language[navigator.language.slice(0, 2)] || language["en"];