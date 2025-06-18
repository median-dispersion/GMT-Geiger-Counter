// Strings in different languages
const language = {

    // English
    en: {

        document_title:                   "WiFi settings | GMT Geiger Counter",
        wifi_settings:                    "WiFi settings",
        warning_message:                  "The WiFi credentials will be transmitted over an <b>unencrypted</b> connection! Only update them in a <b>trusted</b> network or via the wireless hotspot feature.",
        info_message:                     "After updating the credentials, the WiFi has to be toggled off and on again to make them take effect. To fix an incorrectly updated WiFi name or password, use the wireless hotspot feature to access this page again.",
        update_wifi_credentials:          "Update WiFi credentials",
        wifi_name:                        "WiFi name",
        wifi_password:                    "WiFi password",
        update:                           "Update",
        wifi_credentials_updated:         "WiFi credentials updated!",
        wifi_credentials_updated_message: "The WiFi credentials were updated successfully! To make them take effect the WiFi has to be manually toggled off and on again or the system has to be restarted. Restart the system now? Keep in mind that restarting the system will reset all data that has been recorded so far.",
        ok:                               "OK",
        restart_now:                      "Restart now",
        an_error_occurred:                "An error occurred!",
        an_error_occurred_message:        "An error occurred while updating the WiFi credentials! Please refresh the page and try again.",
        go_back:                          "Go back",
        refresh:                          "Refresh"

    },

    // German
    de: {

        document_title:                   "WLAN Einstellungen | GMT Geiger Counter",
        wifi_settings:                    "WLAN Einstellungen",
        warning_message:                  "Die WLAN Zugangsdaten werden über eine <b>unverschlüsselte</b> Verbindung übertragen! Sie sollten nur in einem <b>vertrauenswürdigen</b> Netzwerk oder über das Hotspot-Feature geändert werden.",
        info_message:                     "Nach der Aktualisierung der Zugangsdaten muss das WLAN aus- und wieder eingeschaltet werden, damit die Änderungen wirksam werden. Um einen falsch aktualisierten WLAN-Namen oder ein falsch aktualisiertes Passwort zu korrigieren, verwenden Sie das Hotspot-Feature, um diese Seite erneut aufzurufen.",
        update_wifi_credentials:          "WLAN aktualisieren",
        wifi_name:                        "WLAN Name",
        wifi_password:                    "WLAN Passwort",
        update:                           "Ändern",
        wifi_credentials_updated:         "WLAN Zugangsdaten aktualisiert!",
        wifi_credentials_updated_message: "Die WLAN Zugangsdaten wurden erfolgreich aktualisiert! Damit die Aktualisierung wirksam wird, muss das WLAN manuell aus- und wieder eingeschaltet oder das System neu gestartet werden. Soll das System jetzt neu gestartet werden? Beachten Sie, dass beim Neustart alle bisher aufgezeichneten Daten zurückgesetzt werden.",
        ok:                               "OK",
        restart_now:                      "Jetzt neu starten",
        an_error_occurred:                "Ein  ist aufgetreten!",
        an_error_occurred_message:        "Beim Aktualisieren der WLAN Zugangsdaten ist ein Fehler aufgetreten! Bitte aktualisieren Sie die Seite und versuchen Sie es erneut.",
        go_back:                          "Zurück",
        refresh:                          "Neuladen"

    }

};

// Set strings based on the browser language or fall back to english
let string = language[navigator.language.slice(0, 2)] || language["en"];