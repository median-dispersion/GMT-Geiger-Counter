class CosmicRayDetector {

    // --------------------------------------------------------------------------------------------
    // Private

    // Cosmic ray detector properties
    #initialized           = false;
    #updateIntervalSeconds = 30;

    // Cosmic ray detector data
    #cosmicRayDetectorData;

    // DOM elements
    #element = {

        screen:                 null,
        screenPulse:            null,
        screenValue:            null,
        doseValue:              null,
        coincidenceTubeTotal:   null,
        mainTubeCounts:         null,
        mainTubeEvents:         null,
        mainTubePercentage:     null,
        followerTubeCounts:     null,
        followerTubeEvents:     null,
        followerTubePercentage: null

    };

    // Pulse animation
    #pulseAnimation;

    // Update loop
    #updateLoop;

    // ============================================================================================
    // Get the cosmic ray detector data
    // ============================================================================================
    async #getCosmicRayDetectorData() {

        // Try getting the cosmic ray detector data
        try {

            // API endpoint URL
            const url = `${window.location.origin}/data/cosmic-ray-detector`;

            // Fetch the cosmic ray detector data
            const response = await fetch(url, {method: "GET", cache: "no-store"});

            // if the response is not a 200 - OK, throw an error
            if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }

            // Parse the response as JSON
            const message = await response.json();

            // If there is no "enabled" field in the cosmic ray detector message throw an error
            if (!("enabled" in (message.data || {}))) { throw new Error("Malformed cosmic ray detector data!"); }

            // If the cosmic ray detector is enabled
            if (message.data.enabled) {

                // Set the cosmic ray detector data
                this.#cosmicRayDetectorData = message;

                // Update the cosmic ray detector values
                this.#updateValues();

                // Log info message
                console.info("Updated cosmic ray detector data!");
            
            // If the cosmic ray detector is disabled
            } else {

                // Display an error dialog
                dialogBox.notify(
                    string.cosmic_ray_detector_disabled,
                    string.cosmic_ray_detector_disabled_message,
                    [{title: string.go_back, event: () => { window.location.href = window.location.origin; }},
                     {title: string.refresh, event: () => { location.reload();                             }}]
                );

                // Clear the update loop
                clearInterval(this.#updateLoop);

                // Log a warning message
                console.warn("Cosmic ray detector is disabled!");

            }

        } catch (error) {

            // Display an error dialog
            dialogBox.notify(
                string.data_request_failed,
                `${string.data_request_failed_message} (${error})`,
                [{title: string.go_back, event: () => { window.location.href = window.location.origin; }},
                 {title: string.refresh, event: () => { location.reload();                             }}]
            );

            // Clear the update loop
            clearInterval(this.#updateLoop);

            // Log an error message
            console.error(`Requesting cosmic ray detector data failed! Error message: '${error}'`);

        }

    }

    // ============================================================================================
    // Set the cosmic ray detector values
    // ============================================================================================
    #setCosmicRayDetectorValues() {

        // Set the screen value
        this.#element.screenValue.innerHTML = this.#cosmicRayDetectorData.data.coincidenceEvents;

        // Set the dose value
        this.#element.doseValue.innerHTML = this.#cosmicRayDetectorData.data.eventsPerHour.toFixed(2);

        // Set the coincidence tube stats
        this.#element.coincidenceTubeTotal.innerHTML = this.#cosmicRayDetectorData.data.eventsTotal;

        // Set the main tube stats
        this.#element.mainTubeCounts.innerHTML = this.#cosmicRayDetectorData.data.mainCounts;
        this.#element.mainTubeEvents.innerHTML = this.#cosmicRayDetectorData.data.coincidenceEvents;
        this.#element.mainTubePercentage.innerHTML = this.#cosmicRayDetectorData.data.mainCounts > 0 ? `${((this.#cosmicRayDetectorData.data.coincidenceEvents/this.#cosmicRayDetectorData.data.mainCounts) * 100).toFixed(2)} %` : "0.00 %";

        // Set the follower tube stats
        this.#element.followerTubeCounts.innerHTML = this.#cosmicRayDetectorData.data.followerCounts;
        this.#element.followerTubeEvents.innerHTML = this.#cosmicRayDetectorData.data.coincidenceEvents;
        this.#element.followerTubePercentage.innerHTML = this.#cosmicRayDetectorData.data.mainCounts > 0 ?  `${((this.#cosmicRayDetectorData.data.coincidenceEvents/this.#cosmicRayDetectorData.data.followerCounts) * 100).toFixed(2)} %` : "0.00 %";

    }

    // ============================================================================================
    // Pulse the screen
    // ============================================================================================
    #pulseScreen() {

        // Remove the old event listener
        this.#element.screenPulse.removeEventListener("animationend", this.#pulseAnimation);

        // Remove the old animation class
        this.#element.screenPulse.classList.remove("content-screen-pulse-scale-up");

        // Pulse animation event handler
        this.#pulseAnimation = () => {

            // Stop rendering pulse element
            this.#element.screenPulse.style.display = "none";

            // Remove the old animation class
            this.#element.screenPulse.classList.remove("content-screen-pulse-scale-up");
            
            // Remove the event listener
            this.#element.screenPulse.removeEventListener("animationend", this.#pulseAnimation);

        };

        // Add the event listener for stopping the animation
        this.#element.screenPulse.addEventListener("animationend", this.#pulseAnimation);

        // Add the animation class
        this.#element.screenPulse.classList.add("content-screen-pulse-scale-up");

        // Render the pulse element
        this.#element.screenPulse.style.display = "block";

    }

    // ============================================================================================
    // Update the cosmic ray detector values
    // ============================================================================================
    #updateValues() {

        // If there is any cosmic ray detector data
        if (this.#cosmicRayDetectorData !== undefined) {

            // Play the pulse animation
            this.#pulseScreen();

            // Set the cosmic ray detector values
            this.#setCosmicRayDetectorValues();
        
        // If there is no cosmic ray detector data available
        } else {

            // Log warn message
            console.warn("Could not update cosmic ray detector values, no data available!");

        }

    }

    // --------------------------------------------------------------------------------------------
    // Public

    // ============================================================================================
    // Initialize the cosmic ray detector
    // ============================================================================================
    initialize() {

        // If not initialized
        if(!this.#initialized) {

            // Set initialization flag to true
            this.#initialized = true;

            // Get DOM elements
            this.#element.screen                 = document.querySelector("#content-screen");
            this.#element.screenPulse            = document.querySelector("#content-screen-pulse");
            this.#element.screenValue            = document.querySelector("#content-screen-value");
            this.#element.doseValue              = document.querySelector("#content-dose-reading-value");
            this.#element.coincidenceTubeTotal   = document.querySelector("#content-tube-info-row-coincidence-total").querySelector(".content-tube-info-value");
            this.#element.mainTubeCounts         = document.querySelector("#content-tube-info-row-main-counts").querySelector(".content-tube-info-value");
            this.#element.mainTubeEvents         = document.querySelector("#content-tube-info-row-main-events").querySelector(".content-tube-info-value");
            this.#element.mainTubePercentage     = document.querySelector("#content-tube-info-row-main-event-percentage").querySelector(".content-tube-info-value");
            this.#element.followerTubeCounts     = document.querySelector("#content-tube-info-row-follower-counts").querySelector(".content-tube-info-value");
            this.#element.followerTubeEvents     = document.querySelector("#content-tube-info-row-follower-events").querySelector(".content-tube-info-value");
            this.#element.followerTubePercentage = document.querySelector("#content-tube-info-row-follower-event-percentage").querySelector(".content-tube-info-value");

            // Initialize the dialog box
            dialogBox.initialize();

            // Create a new update loop
            this.#updateLoop = setInterval(this.#getCosmicRayDetectorData.bind(this), this.#updateIntervalSeconds * 1000);

            // Update the cosmic ray detector data immediately
            this.#getCosmicRayDetectorData();

        }

    }

    // ============================================================================================
    // Set the update interval
    // ============================================================================================
    setUpdateInterval(timeSeconds) {

        // Clear the previous update loop
        clearInterval(this.#updateLoop);

        // Set the new update interval time
        this.#updateIntervalSeconds = timeSeconds;

        // Get the newest cosmic ray detector data
        this.#getCosmicRayDetectorData();

        // Create a new update loop
        this.#updateLoop = setInterval(this.#getCosmicRayDetectorData.bind(this), this.#updateIntervalSeconds * 1000);

    }

}

const cosmicRayDetector = new CosmicRayDetector();