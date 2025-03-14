// ================================================================================================
// Pulse the main screen
// ================================================================================================
function pulseScreen() {

    // Get the pulse element
    const pulse = document.getElementById("main-content-shelf-screen-pulse");

    // Add the animation class to start the animation
    pulse.classList.add("main-content-shelf-screen-pulse-scale");

    // Add an event listener that checks if the animation has fished playing
    pulse.addEventListener("animationend", function callback(event) {

        // Remove the animation class
        pulse.classList.remove("main-content-shelf-screen-pulse-scale");

        // Remove the event listener
        pulse.removeEventListener("animationend", callback);

    });

}

// ================================================================================================
// Show the settings panel
// ================================================================================================
function showSettings() {

    // Get DOM elements
    const settings   = document.getElementById("settings");
    const background = document.getElementById("setting-background");
    const panel      = document.getElementById("settings-panel");

    // Change the display mode of the settings window
    settings.style.display = "block";

    // Remove old animations classes if necessary
    background.classList.remove("setting-background-fade-out")
    panel.classList.remove("settings-panel-slide-out")

    // Add CSS animation classes
    background.classList.add("setting-background-fade-in");
    panel.classList.add("settings-panel-slide-in");

}

// ================================================================================================
// Hide settings panel
// ================================================================================================
function hideSettings() {

    // Get DOM elements
    const settings   = document.getElementById("settings");
    const background = document.getElementById("setting-background");
    const panel      = document.getElementById("settings-panel");

    // Remove old animations classes if necessary
    background.classList.remove("setting-background-fade-in")
    panel.classList.remove("settings-panel-slide-in")

    // Add CSS animation classes
    background.classList.add("setting-background-fade-out");
    panel.classList.add("settings-panel-slide-out");

    // Add an event listener that checks if the animation has fished playing
    settings.addEventListener("animationend", function callback(event) {

        // Hide the settings window completely
        settings.style.display = "none";

        // Remove the event listener
        settings.removeEventListener("animationend", callback);

    });

}