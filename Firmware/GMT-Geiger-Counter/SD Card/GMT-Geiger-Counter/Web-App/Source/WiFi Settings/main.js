// ================================================================================================
// Initialization once the DOM has finished loading
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

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
        toastMessageText.innerHTML = "Successfully updated WiFi credentials!";
    
    // If error message
    } else {

        // Add error classes and text
        toastMessage.classList.add("toast-message-error");
        toastMessageIcon.classList.add("icon-error");
        toastMessageText.innerHTML = "Something went wrong while updating the WiFi credentials!";

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