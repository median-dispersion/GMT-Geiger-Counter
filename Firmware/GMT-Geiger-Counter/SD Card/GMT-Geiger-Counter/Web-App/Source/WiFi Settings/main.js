// Global variables
let GLOBAL_LANGUAGE;

// ================================================================================================
// Initialization once the DOM has finished loading
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Apply language
    applyLanguage();

    // Add an event listener for submits on the main input area
    document.getElementById("wifi-credentials").addEventListener("submit", (event) => {

        // Prevent the default event action
        event.preventDefault();
        
        // Get WiFi name and password elements
        const wifiName     = document.getElementById("wifi-credentials-name");
        const wifiPassword = document.getElementById("wifi-credentials-password");

        // Send the WiFi name and password values to the Geiger counter
        sendWiFiCredentials(wifiName.value, wifiPassword.value);

        // Clear WiFi name and password values
        wifiName.value = "";
        wifiPassword.value = "";

    });

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
    document.getElementById("toast-message-encryption-info").innerHTML = GLOBAL_LANGUAGE.toast_message_encryption_info;
    document.getElementById("toast-mesaage-update-info").innerHTML = GLOBAL_LANGUAGE.toast_message_update_info;
    document.getElementById("wifi-credentials-title").innerHTML = GLOBAL_LANGUAGE.wifi_credentials_title;
    document.getElementById("wifi-credentials-name").placeholder = GLOBAL_LANGUAGE.wifi_credentials_name_placeholder;
    document.getElementById("wifi-credentials-password").placeholder = GLOBAL_LANGUAGE.wifi_credentials_password_placeholder;
    document.getElementById("wifi-credentials-button").innerHTML = GLOBAL_LANGUAGE.wifi_credentials_button;
    document.getElementById("update-info-title").innerHTML = GLOBAL_LANGUAGE.update_info_title;

}

// ================================================================================================
// Send WiFi credentials to the Geiger counter
// ================================================================================================
function sendWiFiCredentials(wifiName, wifiPassword) {

    // Construct URL
    const url = window.location.origin + "/wifi-credentials";
    
    // Send the WiFi credentials to the Geiger counter
    fetch(url, {

        // PUT request
        method: "PUT",

        // Indicates the request body contains JSON
        headers: {

            "Content-Type": "application/json"

        },

        // JSON data containing wifi name and password
        body: JSON.stringify({ 

            wifiName:     wifiName, 
            wifiPassword: wifiPassword 

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
            throw new Error("Request failed with status: " + response.status);

        }

    })

    // If response was 200 OK handle JSON data
    .then(update => {
        
        // If JSON contains success status
        if (update.success) {

            // Display success message
            displayUpdateStatus(true);
        
        // If JSON data contains no success status 
        } else {

            // Throw an error
            throw new Error("Something went wrong updating the WiFi credentials!");

        }

    })

    // If an error occurred during the update
    .catch(error => {

        // Log the error to the console
        console.error(error);

        // Display error message
        displayUpdateStatus(false)

    });
    

}

// ================================================================================================
// Display update status message
// ================================================================================================
function displayUpdateStatus(success) {

    // Create HTML elements
    const toastMessage     = document.createElement("div");
    const toastMessageIcon = document.createElement("div");
    const toastMessageText = document.createElement("div");

    // Add class names to the elements
    toastMessage.className = "toast-message";
    toastMessageIcon.className = "toast-message-icon";
    toastMessageText.className = "toast-message-text";

    // If success message
    if (success) {

        // Add success classes and text
        toastMessage.classList.add("toast-message-success");
        toastMessageIcon.classList.add("icon-success");
        toastMessageText.innerHTML = GLOBAL_LANGUAGE.toast_message_success_info;
    
    // If error message
    } else {

        // Add error classes and text
        toastMessage.classList.add("toast-message-error");
        toastMessageIcon.classList.add("icon-error");
        toastMessageText.innerHTML = GLOBAL_LANGUAGE.toast_message_failure_info;

    }

    // Apply the fade in animation
    toastMessage.classList.add("fade-in");
    document.getElementById("update-info-title").classList.add("fade-in");

    // Append elements to message
    toastMessage.appendChild(toastMessageIcon);
    toastMessage.appendChild(toastMessageText);

    // Append message to the DOM
    document.getElementById("update-info").appendChild(toastMessage);

}