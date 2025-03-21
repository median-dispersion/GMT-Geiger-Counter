// ------------------------------------------------------------------------------------------------
// Global variables

let GEIGER_COUNTER_DATA      = {latest: {}, history: []};
let LAST_UPDATE_MILLISECONDS = 0;

// ================================================================================================
// Initialization once the DOM has finished loading
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Get stored settings
    getSettings();

    // Initialize all inputs
    initializeInputs();
    initializeRangeSliders();

    // Get latest data
    getData();

    // Loop forever
    loop();

});

// ================================================================================================
// Initialize all inputs
// ================================================================================================
function initializeInputs() {
    
    // For all inputs
    document.querySelectorAll("input").forEach(input => {

        // Add an input event listener
        input.addEventListener("input", () => {

            // Store currently applied settings
            setSettings();

            // Whenever a user input is registered update all values on screen
            setDOMElements();

        });

    });

}

// ================================================================================================
// Initialize all range sliders
// ================================================================================================
function initializeRangeSliders() {

    // For all input ranges
    document.querySelectorAll("input[type='range']").forEach(range => {

        // Get DOM elements
        const thumb = range.parentNode.getElementsByClassName("settings-panel-content-section-range-container-slider-thumb")[0];
        const value = range.parentNode.parentNode.getElementsByClassName("settings-panel-content-section-range-info-value")[0];

        // Calculate with of range thumb
        const relative = (100 / (range.max - range.min)) * (range.value - range.min);

        // Calculate range thumb offset
        const offset   = 1 - (relative / 100);

        // Apply width and offset to range thumb
        thumb.style.width = `calc(${relative}% + (24px * ${offset}))`;

        // Set the range value text
        value.innerHTML = range.value;
        
        // Add event listener to the input range so it dynamically updates
        range.addEventListener("input", () => {

            // Calculate width and offset of the range thumb
            const relative = (100 / (range.max - range.min)) * (range.value - range.min);
            const offset   = 1 - (relative / 100);

            // Apply width and offset to range thumb
            thumb.style.width = `calc(${relative}% + (24px * ${offset}))`;

            // Set the range value text
            value.innerHTML = range.value;
            
        });

    });

}

// ================================================================================================
// Show error message
// ================================================================================================
function showError(message, title = "Error") {

    // Get DOM elements
    const errorElement       = document.getElementById("error");
    const messageElement     = document.getElementById("error-message");
    const titleElement       = document.getElementById("error-message-title");
    const descriptionElement = document.getElementById("error-message-description");

    // Fill error message
    titleElement.innerHTML = title;
    descriptionElement.innerHTML = message;

    // If the error message is not already being rendered
    if (window.getComputedStyle(errorElement).display != "flex") {
        
        // Remove old animation classes
        errorElement.classList.remove("error-fade-out");
        messageElement.classList.remove("error-message-scale-up");

        // Add animation classes
        errorElement.classList.add("error-fade-in");
        messageElement.classList.add("error-message-scale-up");

        // Prevent page scrolling
        document.body.style.overflow = "hidden"

        // Render error element
        errorElement.style.display = "flex";

    }

}

// ================================================================================================
// Show error message
// ================================================================================================
function hideError() {

    // Get DOM elements
    const errorElement   = document.getElementById("error");
    const messageElement = document.getElementById("error-message");

    // If the error message is visible
    if (window.getComputedStyle(errorElement).display != "none") {

        // Remove old animation classes
        errorElement.classList.remove("error-fade-in");
        
        // Add animation classes
        errorElement.classList.add("error-fade-out");

        // Add an event listener that checks if the animation has finished playing
        errorElement.addEventListener("animationend", function callback() {

            // Hide error element
            errorElement.style.display = "none";

            // Allow page scrolling
            document.body.style.overflow = "auto"

            // Remove old animation classes
            errorElement.classList.remove("error-fade-out");
            messageElement.classList.remove("error-message-scale-up");

            // Remove the event listener
            errorElement.removeEventListener("animationend", callback);

        });

    }

}

// ================================================================================================
// Show settings
// ================================================================================================
function showSettings() {
    
    // Get DOM elements
    const settingElement    = document.getElementById("settings");
    const backgroundElement = document.getElementById("settings-background");
    const panelElement      = document.getElementById("settings-panel");

    // Prevent page scroll
    document.body.style.overflow = "hidden";

    // Render settings
    settingElement.style.display = "block";

    // Remove old animation classes
    backgroundElement.classList.remove("settings-background-fade-out");
    panelElement.classList.remove("settings-panel-slide-out");

    // Add animation classes
    backgroundElement.classList.add("settings-background-fade-in");
    panelElement.classList.add("settings-panel-slide-in");

}

// ================================================================================================
// Hide settings
// ================================================================================================
function hideSettings() {
    
    // Get DOM elements
    const settingElement    = document.getElementById("settings");
    const backgroundElement = document.getElementById("settings-background");
    const panelElement      = document.getElementById("settings-panel");

    // Remove old animation classes
    backgroundElement.classList.remove("settings-background-fade-in");
    panelElement.classList.remove("settings-panel-slide-in");

    // Add animation classes
    backgroundElement.classList.add("settings-background-fade-out");
    panelElement.classList.add("settings-panel-slide-out");

    // Add an event listener that checks if the animation has finished playing
    settingElement.addEventListener("animationend", function callback() {

        // Hide settings
        settingElement.style.display = "none";

        // Allow page scroll
        document.body.style.overflow = "auto";

        // Remove the event listener
        settingElement.removeEventListener("animationend", callback);

    });

}

// ================================================================================================
// Pulse the main screen
// ================================================================================================
function pulseScreen() {

    // Get the pulse element
    const pulseElement = document.getElementById("main-screen-pulse");

    // Render the pulse element
    pulseElement.style.display = "block";

    // Add the animation class to start the animation
    pulseElement.classList.add("main-screen-pulse-scale");

    // Add an event listener that checks if the animation has finished playing
    pulseElement.addEventListener("animationend", function callback(event) {

        // Hide the pulse element
        pulseElement.style.display = "none";

        // Remove the animation class
        pulseElement.classList.remove("main-screen-pulse-scale");

        // Remove the event listener
        pulseElement.removeEventListener("animationend", callback);

    });

}

// ================================================================================================
// Set the screen values
// ================================================================================================
function setDoseScreen(rating, readings) {

    // Get DOM elements
    const selectedRadio = document.querySelector("input[name='screen-unit']:checked");
    const screenElement = document.getElementById("main-screen");
    const doseElement   = document.getElementById("main-screen-dose");
    const unitElement   = document.getElementById("main-screen-unit");

    // Depending on the rating set the appropriate screen color
    switch (rating) {

        case 0:  screenElement.className = "main-screen-normal";   break;
        case 1:  screenElement.className = "main-screen-elevated"; break;
        case 2:  screenElement.className = "main-screen-medium";   break;
        case 3:  screenElement.className = "main-screen-high";     break;
        case 4:  screenElement.className = "main-screen-extreme";  break;
        default: screenElement.className = "main-screen-normal";   break;

    }

    // Depending on the selected screen unit set the appropriate value and unit
    switch (selectedRadio.value) {

        case "sievert":
            doseElement.innerHTML = readings.sievert.value;
            unitElement.innerHTML = readings.sievert.prefix + readings.sievert.unit;
        break;

        case "rem":
            doseElement.innerHTML = readings.rem.value;
            unitElement.innerHTML = readings.rem.prefix + readings.rem.unit;
        break;

        case "rontgen":
            doseElement.innerHTML = readings.rontgen.value;
            unitElement.innerHTML = readings.rontgen.prefix + readings.rontgen.unit;
        break;

        case "gray":
            doseElement.innerHTML = readings.gray.value;
            unitElement.innerHTML = readings.gray.prefix + readings.gray.unit;
        break;

        default:
            doseElement.innerHTML = readings.sievert.value;
            unitElement.innerHTML = readings.sievert.prefix + readings.sievert.unit;
        break;

    }

}

// ================================================================================================
// Set the readings
// ================================================================================================
function setReadings(readings) {
    
    // Get DOM elements
    const cpsReadingElement     = document.getElementById("main-readings-value-cps");
    const cpmReadingElement     = document.getElementById("main-readings-value-cpm");
    const sievertReadingElement = document.getElementById("main-readings-value-sievert");
    const remReadingElement     = document.getElementById("main-readings-value-rem");
    const rontgenReadingElement = document.getElementById("main-readings-value-rontgen");
    const grayReadingElement    = document.getElementById("main-readings-value-gray");

    // Fill readings
    cpsReadingElement.innerHTML     = `<b>${readings.cps.value}</b> ${readings.cps.prefix}${readings.cps.unit}`;
    cpmReadingElement.innerHTML     = `<b>${readings.cpm.value}</b> ${readings.cpm.prefix}${readings.cpm.unit}`;
    sievertReadingElement.innerHTML = `<b>${readings.sievert.value}</b> ${readings.sievert.prefix}${readings.sievert.unit}`;
    remReadingElement.innerHTML     = `<b>${readings.rem.value}</b> ${readings.rem.prefix}${readings.rem.unit}`;
    rontgenReadingElement.innerHTML = `<b>${readings.rontgen.value}</b> ${readings.rontgen.prefix}${readings.rontgen.unit}`;
    grayReadingElement.innerHTML    = `<b>${readings.gray.value}</b> ${readings.gray.prefix}${readings.gray.unit}`;

}

// ================================================================================================
// Set the radiation rating
// ================================================================================================
function setRating(rating) {
    
    // Get DOM elements
    const ratingElement      = document.getElementById("main-radiation-rating-content-value");
    const descriptionElement = document.getElementById("main-radiation-rating-content-description");

    // Depending on the rating set the appropriate value and description
    switch (rating) {

        case 0:
            ratingElement.innerHTML = "NORMAL";
            descriptionElement.innerHTML = "Normal background radiation, no action is needed.";
        break;

        case 1:
            ratingElement.innerHTML = "ELEVATED";
            descriptionElement.innerHTML = "Elevated radiation readings, no immediate danger.";
        break;

        case 2:
            ratingElement.innerHTML = "MEDIUM";
            descriptionElement.innerHTML = "Medium level of radiation, closely monitor readings and prepare to leave.";
        break;

        case 3:
            ratingElement.innerHTML = "HIGH";
            descriptionElement.innerHTML = "High radiation exposure! Leave the area as fast as possible.";
        break;

        case 4:
            ratingElement.innerHTML = "EXTREME";
            descriptionElement.innerHTML = "EXTREME DANGER! Evacuate immediately! Radiation readings are extremely high.";
        break;
    
        default:
            ratingElement.innerHTML = "UNKNOWN";
            descriptionElement.innerHTML = "Unknown radiation rating, which is indicative of a communication error.";
        break;
    }

}

// ================================================================================================
// Set the tube data
// ================================================================================================
function setTubes(data) {
    
    // Get DOM elements
    const totalCountsElement      = document.getElementById("main-tube-content-value-total-counts");
    const numberOfTubesElement    = document.getElementById("main-tube-content-value-number-of-tubes");
    const tubeTypeElement         = document.getElementById("main-tube-content-value-tube-type");
    const mainCountsElement       = document.getElementById("main-tube-content-value-main-counts");
    const mainRelativeElement     = document.getElementById("main-tube-content-value-main-relative");
    const followerCountsElement   = document.getElementById("main-tube-content-value-follower-counts");
    const followerRelativeElement = document.getElementById("main-tube-content-value-follower-relative");

    // Relative values
    let mainRelative     = 0;
    let followerRelative = 0;

    // Only do this if there are any counts (prevent division by 0)
    if (data.counts > 0) {

        // Calculate relative values
        mainRelative     = Math.round((100 / data.counts) * data.main);
        followerRelative = Math.round((100 / data.counts) * data.follower);

    }

    // Fill elements
    totalCountsElement.innerHTML = data.counts;
    numberOfTubesElement.innerHTML = data.tubes;
    tubeTypeElement.innerHTML = data.type;
    mainCountsElement.innerHTML = data.main;
    mainRelativeElement.innerHTML = `${mainRelative} %`;
    followerCountsElement.innerHTML = data.follower;
    followerRelativeElement.innerHTML = `${followerRelative} %`;

}

// ================================================================================================
// Update all DOM elements
// ================================================================================================
function setDOMElements() {

    // If Geiger counter data is initialized
    if (Object.keys(GEIGER_COUNTER_DATA.latest).length) {

        // Get the latest Geiger counter message
        const message = GEIGER_COUNTER_DATA.latest;

        // Get the latest Geiger counter data
        const data = message.data;

        // Get the Geiger counter readings
        const readings = getReadings(data);

        // Update all window elements
        setDoseScreen(data.rating, readings);
        setReadings(readings);
        setRating(data.rating);
        setTubes(data);

    }

}

// ================================================================================================
// Store the applied settings
// ================================================================================================
function setSettings() {

    // Get DOM elements
    const autoRangeElement      = document.getElementById("settings-panel-content-section-toggle-input-auto-range-units");
    const updateIntervalElement = document.getElementById("settings-panel-content-section-range-container-input-update-interval");
    const selectedUnitElement   = document.querySelector("input[name='screen-unit']:checked");

    // Construct JSON settings object
    const settings = {

        autoRange:      autoRangeElement.checked,
        screenUnit:     selectedUnitElement.value,
        updateInterval: updateIntervalElement.value

    };

    // Convert to JSON string and store in localStorage
    localStorage.setItem("geigerCounterSettings", JSON.stringify(settings));

}

// ================================================================================================
// Apply stored settings
// ================================================================================================
function getSettings() {

    // Get DOM elements
    const autoRangeElement      = document.getElementById("settings-panel-content-section-toggle-input-auto-range-units");
    const updateIntervalElement = document.getElementById("settings-panel-content-section-range-container-input-update-interval");
    const unitSievertElement    = document.getElementById("settings-panel-content-section-radio-input-sievert");
    const unitRemElement        = document.getElementById("settings-panel-content-section-radio-input-rem");
    const unitRontgenElement    = document.getElementById("settings-panel-content-section-radio-input-rontgen");
    const unitGrayElement       = document.getElementById("settings-panel-content-section-radio-input-gray");

    // Try to retrieve the stored settings
    let settings = localStorage.getItem("geigerCounterSettings");

    // Check if the settings exists
    if (settings) {

        // Convert settings string to JSON
        settings = JSON.parse(settings);

        // Apply settings
        autoRangeElement.checked    = settings.autoRange;
        updateIntervalElement.value = settings.updateInterval;

        // Depending on the selected unit apply selected unit
        switch (settings.screenUnit) {

            case "sievert": unitSievertElement.checked = true; break;
            case "rem":     unitRemElement.checked     = true; break;
            case "rontgen": unitRontgenElement.checked = true; break;
            case "gray":    unitGrayElement.checked    = true; break;

        }

    }
    
}

// ================================================================================================
// Calculate equivalent doses
// ================================================================================================
function getReadings(data) {
    
    // Get DOM elements
    const autoRangeElement = document.getElementById("settings-panel-content-section-toggle-input-auto-range-units");

    // Construct and calculate reading objects
    let cps     = {value: data.cpm / 60,   prefix: "",  unit: "CPS"  };
    let cpm     = {value: data.cpm,        prefix: "",  unit: "CPM"  };
    let sievert = {value: data.usvh,       prefix: "µ", unit: "Sv/h" };
    let rem     = {value: data.usvh * 100, prefix: "µ", unit: "rem/h"};
    let rontgen = {value: data.usvh * 100, prefix: "µ", unit: "R/h"  };
    let gray    = {value: data.usvh,       prefix: "µ", unit: "Gy/h" };

    // If auto ranging units is enabled
    if (autoRangeElement.checked) {

        // Function for auto ranging
        const autoRange = (reading) => {

            // If equivalent dose is >= 500 milli
            // Divide value to convert to base unit and remove the prefix
            if (reading.value >= 500000) {

                reading.value /= 1000000;
                reading.prefix = "";
            
            // If equivalent dose is >= 500 micro
            // Divide value to convert to milli unit and set milli prefix
            } else if (reading.value >= 500) {

                reading.value /= 1000;
                reading.prefix = "m";

            }

        };

        // Auto range equivalent doses
        autoRange(sievert);
        autoRange(rem);
        autoRange(rontgen);
        autoRange(gray);

    }

    // Clip decimal places
    cps.value     = parseFloat(cps.value.toFixed(2));
    cpm.value     = parseFloat(cpm.value.toFixed(2));
    sievert.value = parseFloat(sievert.value.toFixed(2));
    rem.value     = parseFloat(rem.value.toFixed(2));
    rontgen.value = parseFloat(rontgen.value.toFixed(2));
    gray.value    = parseFloat(gray.value.toFixed(2));

    return {cps, cpm, sievert, rem, rontgen, gray};

}

// ================================================================================================
// Get data
// ================================================================================================
function getData() {
    
    // Geiger counter data URL
    url = `${window.location.protocol}//${window.location.host}/data/geiger-counter`;

    // Request data
    fetch(url)

    // Handle response
    .then(response => {

        // If response OK
        if (response.ok) {

            // Returns parsed JSON
            return response.json();

        // If response not OK
        } else {

            // Throw error
            throw new Error(response.status);

        }

    })

    // Handle new message
    .then(message => {

        // Log Geiger counter message
        console.log(message);

        // If the Geiger counter is enabled
        if (message.data.enabled) {

            // If the message doesn't contain a date
            if (!("date" in message)) {

                // Append the current date to the Geiger counter message
                message.date = new Date();

            }

            // Update the global Geiger counter data variable with the latest message
            GEIGER_COUNTER_DATA.latest = message;

            // Append the new geiger counter message to the history
            GEIGER_COUNTER_DATA.history.push(message);

            // Hide any previous error messages
            hideError();

            // Play pulse animation
            pulseScreen();

            // Update all values
            setDOMElements();

        // If the Geiger counter is disabled
        } else {

            // Display error message
            showError("The Geiger counter is disabled! Please enable it before using this feature.", "Geiger counter disabled");

        }

    })

    // Catch network errors
    .catch(error => {

        // Log error
        console.error(error);

        // Display error message
        showError(`A network error occurred! Please check the wireless connection. (${error})`, "Network error");

    });

}

// ================================================================================================
// Main loop
// ================================================================================================
function loop() {

    // Get DOM elements
    const update = document.getElementById("settings-panel-content-section-range-container-input-update-interval");

    // If the last update time is greater or equal to the set update interval
    if (performance.now() - LAST_UPDATE_MILLISECONDS >= update.value * 1000) {

        // Get Geiger counter data
        getData();

        // Set the last update time
        LAST_UPDATE_MILLISECONDS = performance.now();

    }

    // Loop forever
    requestAnimationFrame(loop);

}