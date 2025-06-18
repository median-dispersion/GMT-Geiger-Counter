// ================================================================================================
// Initialize web app
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Get DOM elements
    const element = {

        document:              document.querySelector("html"),
        documentTitle:         document.querySelector("title"),
        headerTitle:           document.querySelector("#header-title"),
        warningMessage:        document.querySelector("#content-notification-message-warning"),
        infoMessage:           document.querySelector("#content-notification-message-info"),
        updateWiFiCredentials: document.querySelector("#content-title-wifi-credentials"),
        credentialsForm:       document.querySelector("#content-credentials"),
        wifiName:              document.querySelector("#content-credentials-name"),
        wifiPassword:          document.querySelector("#content-credentials-password"),
        updateButton:          document.querySelector("#content-credentials-button")

    };

    // Selected language based on the browser language or fall back to english
    const selectedLanguage = navigator.language.slice(0, 2) in language ? navigator.language.slice(0, 2) : "en";

    // Set language strings to the selected language
    string = language[selectedLanguage];

    // Set the document language
    element.document.setAttribute("lang", selectedLanguage);

    // Translate DOM element
    element.documentTitle.innerHTML         = string.document_title;
    element.headerTitle.innerHTML           = string.wifi_settings;
    element.warningMessage.innerHTML        = string.warning_message;
    element.infoMessage.innerHTML           = string.info_message;
    element.updateWiFiCredentials.innerHTML = string.update_wifi_credentials;
    element.wifiName.placeholder            = string.wifi_name;
    element.wifiPassword.placeholder        = string.wifi_password;
    element.updateButton.innerHTML          = string.update;

    // Initialize components
    headerBar.initialize();
    dialogBox.initialize();

    // Add an event listener for updating the WiFi credentials
    element.credentialsForm.addEventListener("submit", (event) => {

        // Prevent the default event action
        event.preventDefault();
        
        // API endpoint url for updating the WiFi credentials 
        const url = `${window.location.origin}/system/wifi-credentials`;

        // Send the WiFi credentials to the Geiger counter
        fetch(url, {

            // PUT request
            method: "PUT",

            // No cache
            cache: "no-store",

            // Indicates the request body contains JSON
            headers: {

                "Content-Type": "application/json"

            },

            // JSON data containing wifi name and password
            body: JSON.stringify({ 

                wifiName:     element.wifiName.value,
                wifiPassword: element.wifiPassword.value

            })

        })

        // Wait for a response
        .then(response => {
            
            // Check if the response is 200 OK
            if (response.ok) {

                // Return response data
                return response.json();
            
            // If not OK
            } else {

                // Throw an error
                throw new Error(`HTTP error! Status: ${response.status}`);

            }

        })

        // If response was 200 OK handle JSON data
        .then(update => {
            
            // If JSON contains success status
            if (update.success) {

                // Display a dialog box
                dialogBox.notify(

                    // Dialog box title and message
                    string.wifi_credentials_updated,
                    string.wifi_credentials_updated_message,

                    [
                        
                        // OK button
                        {title: string.ok, event: () => dialogBox.dismiss()},

                        // Button for restarting the system
                        {title: string.restart_now, event: () => {
                            
                            // Restart system API endpoint url
                            const url = `${window.location.origin}/system/restart`;

                            // Restart the system
                            fetch(url, {method: "PUT", cache: "no-store"});

                            // Dismiss the dialog box
                            dialogBox.dismiss();

                        }}

                    ]

                );
            
            // If JSON data contains no success status 
            } else {

                // Throw an error
                throw new Error("Internal error occurred while updating the WiFi credentials!");

            }

        })

        // If an error occurred during the update
        .catch(error => {

            dialogBox.notify(

                string.an_error_occurred,
                `${string.an_error_occurred_message} (${error})`,
                [{title: string.go_back, event: () => { window.location.href = window.location.origin; }},
                 {title: string.refresh, event: () => location.reload()}]

            );

            // Log the error to the console
            console.error(error);

        });

        // Clear WiFi name and password values
        element.wifiName.value = "";
        element.wifiPassword.value = "";

    });

});