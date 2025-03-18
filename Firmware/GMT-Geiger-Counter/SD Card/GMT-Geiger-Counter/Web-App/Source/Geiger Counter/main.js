// ================================================================================================
// Initialization once the DOM has finished loading
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    updateInputRange();
    main();

});

// ================================================================================================
// Apply input range updates
// ================================================================================================
function updateInputRange() {

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

    // Prevent page scrolling
    document.body.style.overflow = "hidden"

    // Remove old animation classes
    errorElement.classList.remove("error-fade-out");
    messageElement.classList.remove("error-message-scale-up");

    // Fill error message
    titleElement.innerHTML = title;
    descriptionElement.innerHTML = message;

    // Render error element
    errorElement.style.display = "flex";
    
    // Add animation classes
    errorElement.classList.add("error-fade-in");
    messageElement.classList.add("error-message-scale-up");

}

// ================================================================================================
// Show error message
// ================================================================================================
function hideError() {

    // Get DOM elements
    const errorElement       = document.getElementById("error");
    const messageElement     = document.getElementById("error-message");

    // Remove old animation classes
    errorElement.classList.remove("error-fade-in");
    
    // Add animation classes
    errorElement.classList.add("error-fade-out");

    // Add an event listener that checks if the animation has finished playing
    errorElement.addEventListener("animationend", function callback() {

        // Hide error element
        errorElement.style.display = "none";

        // Remove old animation classes
        errorElement.classList.remove("error-fade-out");
        messageElement.classList.remove("error-message-scale-up");

        // Allow page scrolling
        document.body.style.overflow = "auto"

        // Remove the event listener
        errorElement.removeEventListener("animationend", callback);

    });

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
// Set the screen values
// ================================================================================================
function setScreen(data, readings) {

    // Get DOM elements
    const selectedRadio = document.querySelector("input[name='screen-unit']:checked");
    const screenElement = document.getElementById("main-screen");
    const doseElement   = document.getElementById("main-screen-dose");
    const unitElement   = document.getElementById("main-screen-unit");

    // Depending on the rating set the appropriate screen color
    switch (data.rating) {

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

    }

}

// ================================================================================================
// Set the readings
// ================================================================================================
function setReadings(data) {
    
    // Get DOM elements
    const cpsReadingElement     = document.getElementById("main-readings-value-cps");
    const cpmReadingElement     = document.getElementById("main-readings-value-cpm");
    const sievertReadingElement = document.getElementById("main-readings-value-sievert");
    const remReadingElement     = document.getElementById("main-readings-value-rem");
    const rontgenReadingElement = document.getElementById("main-readings-value-rontgen");
    const grayReadingElement    = document.getElementById("main-readings-value-gray");

    // Fill readings
    cpsReadingElement.innerHTML     = `<b>${data.cps.value}</b> ${data.cps.prefix}${data.cps.unit}`;
    cpmReadingElement.innerHTML     = `<b>${data.cpm.value}</b> ${data.cpm.prefix}${data.cpm.unit}`;
    sievertReadingElement.innerHTML = `<b>${data.sievert.value}</b> ${data.sievert.prefix}${data.sievert.unit}`;
    remReadingElement.innerHTML     = `<b>${data.rem.value}</b> ${data.rem.prefix}${data.rem.unit}`;
    rontgenReadingElement.innerHTML = `<b>${data.rontgen.value}</b> ${data.rontgen.prefix}${data.rontgen.unit}`;
    grayReadingElement.innerHTML    = `<b>${data.gray.value}</b> ${data.gray.prefix}${data.gray.unit}`;

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
// Set the radiation rating
// ================================================================================================
function setRating(data) {
    
    // Get DOM elements
    const ratingElement      = document.getElementById("main-radiation-rating-content-value");
    const descriptionElement = document.getElementById("main-radiation-rating-content-description");

    // Depending on the rating set the appropriate value and description
    switch (data.rating) {

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

    // Calculate relative values
    const mainRelative     = Math.round((100 / data.counts) * data.main);
    const followerRelative = Math.round((100 / data.counts) * data.follower);

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
// Calculate equivalent doses
// ================================================================================================
function calculateReadings(data) {
    
    // Get DOM elements
    const autoRangeToggleElement = document.getElementById("settings-panel-content-section-toggle-input-auto-range-units");

    // Construct and calculate reading objects
    let cps     = {value: data.cpm / 60,   prefix: "",  unit: "CPS"  };
    let cpm     = {value: data.cpm,        prefix: "",  unit: "CPM"  };
    let sievert = {value: data.usvh,       prefix: "µ", unit: "Sv/h" };
    let rem     = {value: data.usvh * 100, prefix: "µ", unit: "rem/h"};
    let rontgen = {value: data.usvh * 100, prefix: "µ", unit: "R/h"  };
    let gray    = {value: data.usvh,       prefix: "µ", unit: "Gy/h" };

    // If auto ranging units is enabled
    if (autoRangeToggleElement.checked) {

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
    url = `${window.location.host}/data/geiger-counter`;

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

    // Handle new data
    .then(data => {

        // Log Geiger counter data
        console.log(data);

        // If the Geiger counter is enabled
        if (data.enabled) {

            // Hide any previous error messages
            hideError();

            // Calculate Geiger counter readings
            const readings = calculateReadings(data);

            // Set all screen values
            setScreen(data, readings);
            setReadings(readings);
            setRating(data);
            setTubes(data);

            // Play pulse animation
            pulseScreen();

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
function main() {
    
    //getData();

    const update = document.getElementById("settings-panel-content-section-range-container-input-update-interval");
    
    setTimeout(main, update.value * 1000);

}