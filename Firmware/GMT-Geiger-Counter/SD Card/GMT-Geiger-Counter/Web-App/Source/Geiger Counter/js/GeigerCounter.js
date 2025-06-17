class GeigerCounter {

    // --------------------------------------------------------------------------------------------
    // Private

    // Geiger counter properties
    #initialized           = false;
    #autoRange             = true;
    #screenUnit            = "sieverts";
    #updateIntervalSeconds = 10;

    // Geiger counter data
    #geigerCounterData;
    #equivalentDoses;
    #countReadings;
    #radiationRating;
    #tubeStatistics;

    // DOM elements
    #element = {

        screen:               null,
        screenPulse:          null,
        screenValue:          null,
        screenUnit:           null,
        countsPerMinute:      null,
        countsPerMinuteUnit:  null,
        countsPerSecond:      null,
        countsPerSecondUnit:  null,
        sievertsValue:        null,
        sievertsUnit:         null,
        remValue:             null,
        remUnit:              null,
        rontgenValue:         null,
        rontgenUnit:          null,
        grayValue:            null,
        grayUnit:             null,
        ratingTitle:          null,
        ratingDescription:    null,
        totalCounts:          null,
        numberOfTubes:        null,
        tubeType:             null,
        mainTubeAbsolute:     null,
        mainTubeRelative:     null,
        followerTubeAbsolute: null,
        followerTubeRelative: null

    };

    // Pulse animation
    #pulseAnimation;

    // Update loop
    #updateLoop;

    // ============================================================================================
    // Get the latest Geiger counter data from the API endpoint
    // ============================================================================================
    async #getGeigerCounterData() {

        // Try getting Geiger counter data
        try {

            // Geiger counter API endpoint URL
            const url = `${window.location.origin}/data/geiger-counter`;

            // Fetch the Geiger counter data
            const response = await fetch(url, {method: "GET", cache: "no-store"});

            // If the response is not a 200 - OK, throw an error
            if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }

            // Parse the response as JSON
            const message = await response.json();

            // If there is no "enabled" field in the Geiger counter message throw an error
            if (!("enabled" in (message.data || {}))) { throw new Error("Malformed Geiger counter data!"); }

            // If the Geiger counter is enabled
            if (message.data.enabled) {

                // Set the Geiger counter data
                this.#geigerCounterData = message;

                // Update the Geiger counter values
                this.#updateValues();

                // Log info message
                console.info("Updated Geiger counter data!");
            
            // If the Geiger counter is disabled
            } else {

                // Display an error dialog
                dialogBox.notify(
                    string.geiger_counter_disabled,
                    string.geiger_counter_disabled_message,
                    [{title: string.go_back, event: () => { window.location.href = window.location.origin; }},
                     {title: string.refresh, event: () => { location.reload();                             }}]
                );

                // Clear the update loop
                clearInterval(this.#updateLoop);

                // Log a warning message
                console.warn("Geiger counter is disabled!");

            }

        // If an error occurs requesting the Geiger counter data
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
            console.error(`Requesting Geiger counter data failed! Error message: '${error}'`);

        }

    }

    // ============================================================================================
    // Get the equivalent doses from the Geiger counter data
    // ============================================================================================
    #getEquivalentDoses() {

        // Equivalent dose data
        const equivalentDoses = {

            sieverts: { value: null, prefix: string.micro_prefix, unit: string.sieverts_per_hour_abbreviation },
            rem:      { value: null, prefix: string.micro_prefix, unit: string.rem_per_hour_abbreviation      },
            rontgen:  { value: null, prefix: string.micro_prefix, unit: string.rontgen_per_hour_abbreviation  },
            gray:     { value: null, prefix: string.micro_prefix, unit: string.gray_per_hour_abbreviation     },

        };

        // Calculate the equivalent dose for each unit
        equivalentDoses.sieverts.value = this.#geigerCounterData.data.microsievertsPerHour;
        equivalentDoses.rem.value      = this.#geigerCounterData.data.microsievertsPerHour * 100;
        equivalentDoses.rontgen.value  = this.#geigerCounterData.data.microsievertsPerHour * 100;
        equivalentDoses.gray.value     = this.#geigerCounterData.data.microsievertsPerHour;

        // If auto range is enabled
        if (this.#autoRange) {

            // For each unit
            Object.values(equivalentDoses).forEach(equivalentDoses => {

                // If equivalent dose is >= 500 milli
                // Divide value to convert to base unit and set metric prefix
                if (equivalentDoses.value >= 500000) {

                    equivalentDoses.value /= 1000000;
                    equivalentDoses.prefix = "";
                
                // If equivalent dose is >= 500 micro
                // Divide value to convert to milli unit and set metric prefix
                } else if (equivalentDoses.value >= 500) {

                    equivalentDoses.value /= 1000;
                    equivalentDoses.prefix = string.milli_prefix;

                }

            });

        }

        // Set the equivalent dose
        this.#equivalentDoses = equivalentDoses;

    }

    // ============================================================================================
    // Get the count readings from the Geiger counter data
    // ============================================================================================
    #getCountReadings() {

        // Count readings data
        const countReadings = {

            perMinute: null,
            perSecond: null

        };

        // Get the count readings
        countReadings.perMinute = this.#geigerCounterData.data.countsPerMinute;
        countReadings.perSecond = countReadings.perMinute / 60;

        // Set the count readings
        this.#countReadings = countReadings;

    }

    // ============================================================================================
    // Get the radiation rating from the Geiger counter data
    // ============================================================================================
    #getRadiationRating() {

        // Radiation rating data
        const radiationRating = {

            value:       null,
            title:       null,
            description: null

        };

        // Get the radiation rating value
        radiationRating.value = this.#geigerCounterData.data.rating;

        // Depending on the rating select a title and description
        switch (radiationRating.value) {

            case 0:
                radiationRating.title       = string.radiation_rating_normal_title;
                radiationRating.description = string.radiation_rating_normal_description;
            break;

            case 1:
                radiationRating.title       = string.radiation_rating_elevated_title;
                radiationRating.description = string.radiation_rating_elevated_description;
            break;

            case 2:
                radiationRating.title       = string.radiation_rating_medium_title;
                radiationRating.description = string.radiation_rating_medium_description;
            break;

            case 3:
                radiationRating.title       = string.radiation_rating_high_title;
                radiationRating.description = string.radiation_rating_high_description;
            break;

            case 4:
                radiationRating.title       = string.radiation_rating_extreme_title;
                radiationRating.description = string.radiation_rating_extreme_description;
            break;
        
            default:
                radiationRating.title       = string.radiation_rating_unknown_title;
                radiationRating.description = string.radiation_rating_unknown_description;
            break;

        }

        // Set the radiation rating
        this.#radiationRating = radiationRating;

    }

    // ============================================================================================
    // Get the tube statistics from the Geiger counter data
    // ============================================================================================
    #getTubeStatistics() {

        // Tube statistics data
        const tubeStatistics = {

            type:   null,
            number: null,

            counts: {

                total:  null,
                
                main:     { absolute: null, relative: null },
                follower: { absolute: null, relative: null }

            }

        };

        // Get tube statistics
        tubeStatistics.type                     = this.#geigerCounterData.data.tubeType;
        tubeStatistics.number                   = this.#geigerCounterData.data.tubes;
        tubeStatistics.counts.total             = this.#geigerCounterData.data.counts;
        tubeStatistics.counts.main.absolute     = this.#geigerCounterData.data.mainCounts;
        tubeStatistics.counts.main.relative     = (tubeStatistics.counts.main.absolute / tubeStatistics.counts.total) * 100;
        tubeStatistics.counts.follower.absolute = this.#geigerCounterData.data.followerCounts;
        tubeStatistics.counts.follower.relative = (tubeStatistics.counts.follower.absolute / tubeStatistics.counts.total) * 100;

        // Set the tube statistics
        this.#tubeStatistics = tubeStatistics;

    }

    // ============================================================================================
    // Set the screen values
    // ============================================================================================
    #setScreenValues() {

        // Depending on the radiation rating set the screen color value
        switch (this.#radiationRating.value) {

            case 0:  this.#element.screen.className = "content-screen-normal";   break;
            case 1:  this.#element.screen.className = "content-screen-elevated"; break;
            case 2:  this.#element.screen.className = "content-screen-medium";   break;
            case 3:  this.#element.screen.className = "content-screen-high";     break;
            case 4:  this.#element.screen.className = "content-screen-extreme";  break;
            default: this.#element.screen.className = "content-screen-normal";   break;

        }

        // Depending on the screen unit set the equivalent dose and unit values
        switch (this.#screenUnit) {

            case "sieverts":
                this.#element.screenValue.innerHTML = this.#equivalentDoses.sieverts.value.toFixed(2);
                this.#element.screenUnit.innerHTML  = `${this.#equivalentDoses.sieverts.prefix}${this.#equivalentDoses.sieverts.unit}`;
            break;

            case "rem":
                this.#element.screenValue.innerHTML = this.#equivalentDoses.rem.value.toFixed(2);
                this.#element.screenUnit.innerHTML  = `${this.#equivalentDoses.rem.prefix}${this.#equivalentDoses.rem.unit}`;
            break;

            case "rontgen":
                this.#element.screenValue.innerHTML = this.#equivalentDoses.rontgen.value.toFixed(2);
                this.#element.screenUnit.innerHTML  = `${this.#equivalentDoses.rontgen.prefix}${this.#equivalentDoses.rontgen.unit}`;
            break;

            case "gray":
                this.#element.screenValue.innerHTML = this.#equivalentDoses.gray.value.toFixed(2);
                this.#element.screenUnit.innerHTML  = `${this.#equivalentDoses.gray.prefix}${this.#equivalentDoses.gray.unit}`;
            break;

            default:
                this.#element.screenValue.innerHTML = "?";
                this.#element.screenUnit.innerHTML  = "?";
            break;

        }

    }

    // ============================================================================================
    // Set the count reading values
    // ============================================================================================
    #setCountReadings() {

        // Set the count reading values
        this.#element.countsPerMinute.innerHTML     = this.#countReadings.perMinute;
        this.#element.countsPerMinuteUnit.innerHTML = string.counts_per_minute_abbreviation;
        this.#element.countsPerSecond.innerHTML     = this.#countReadings.perSecond.toFixed(2);
        this.#element.countsPerSecondUnit.innerHTML = string.counts_per_second_abbreviation;

    }

    // ============================================================================================
    // Set the equivalent doses values 
    // ============================================================================================
    #setEquivalentDoses() {

        // Set the equivalent dose values 
        this.#element.sievertsValue.innerHTML = this.#equivalentDoses.sieverts.value.toFixed(2);
        this.#element.sievertsUnit.innerHTML  = `${this.#equivalentDoses.sieverts.prefix}${this.#equivalentDoses.sieverts.unit}`;
        this.#element.remValue.innerHTML      = this.#equivalentDoses.rem.value.toFixed(2);
        this.#element.remUnit.innerHTML       = `${this.#equivalentDoses.rem.prefix}${this.#equivalentDoses.rem.unit}`;
        this.#element.rontgenValue.innerHTML  = this.#equivalentDoses.rontgen.value.toFixed(2);
        this.#element.rontgenUnit.innerHTML   = `${this.#equivalentDoses.rontgen.prefix}${this.#equivalentDoses.rontgen.unit}`;
        this.#element.grayValue.innerHTML     = this.#equivalentDoses.gray.value.toFixed(2);
        this.#element.grayUnit.innerHTML      = `${this.#equivalentDoses.gray.prefix}${this.#equivalentDoses.gray.unit}`;

    }

    // ============================================================================================
    // Set the radiation rating values
    // ============================================================================================
    #setRadiationRating() {

        // Set the radiation rating values
        this.#element.ratingTitle.innerHTML       = this.#radiationRating.title;
        this.#element.ratingDescription.innerHTML = this.#radiationRating.description;

    }

    // ============================================================================================
    // Set tube statistics values
    // ============================================================================================
    #setTubeStatistics() {

        // Set tube statistics values
        this.#element.totalCounts.innerHTML          = this.#tubeStatistics.counts.total;
        this.#element.numberOfTubes.innerHTML        = this.#tubeStatistics.number;
        this.#element.tubeType.innerHTML             = this.#tubeStatistics.type;
        this.#element.mainTubeAbsolute.innerHTML     = this.#tubeStatistics.counts.main.absolute;
        this.#element.mainTubeRelative.innerHTML     = `${this.#tubeStatistics.counts.main.relative.toFixed(1)} %`;
        this.#element.followerTubeAbsolute.innerHTML = this.#tubeStatistics.counts.follower.absolute;
        this.#element.followerTubeRelative.innerHTML = `${this.#tubeStatistics.counts.follower.relative.toFixed(1)} %`;

    }

    // ============================================================================================
    // Pulse the Geiger counter screen
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
    // Update the Geiger counter values
    // ============================================================================================
    #updateValues() {

        // If there is any Geiger counter data
        if (this.#geigerCounterData !== undefined) {

            // Parse the Geiger counter data
            this.#getEquivalentDoses();
            this.#getCountReadings();
            this.#getRadiationRating();
            this.#getTubeStatistics();

            // Update the Geiger counter values
            this.#setScreenValues();
            this.#setCountReadings();
            this.#setEquivalentDoses();
            this.#setRadiationRating();
            this.#setTubeStatistics();

            // Play the pulse animation
            this.#pulseScreen();
        
        // If there is no Geiger counter data available
        } else {

            // Log warn message
            console.warn("Could not update Geiger counter values, no data available!");

        }

    }

    // --------------------------------------------------------------------------------------------
    // Public

    // ============================================================================================
    // Initialize the Geiger counter
    // ============================================================================================
    initialize() {

        // If not initialized
        if(!this.#initialized) {

            // Set initialization flag to true
            this.#initialized = true;

            // Get DOM elements
            this.#element.screen               = document.querySelector("#content-screen");
            this.#element.screenPulse          = document.querySelector("#content-screen-pulse");
            this.#element.screenValue          = document.querySelector("#content-screen-value");
            this.#element.screenUnit           = document.querySelector("#content-screen-unit");
            this.#element.countsPerMinute      = document.querySelector("#content-readings-reading-counts-per-minute").querySelector(".content-readings-reading-value");
            this.#element.countsPerMinuteUnit  = document.querySelector("#content-readings-reading-counts-per-minute").querySelector(".content-readings-reading-unit");
            this.#element.countsPerSecond      = document.querySelector("#content-readings-reading-counts-per-second").querySelector(".content-readings-reading-value");
            this.#element.countsPerSecondUnit  = document.querySelector("#content-readings-reading-counts-per-second").querySelector(".content-readings-reading-unit");
            this.#element.sievertsValue        = document.querySelector("#content-readings-reading-sieverts").querySelector(".content-readings-reading-value");
            this.#element.sievertsUnit         = document.querySelector("#content-readings-reading-sieverts").querySelector(".content-readings-reading-unit");
            this.#element.remValue             = document.querySelector("#content-readings-reading-rem").querySelector(".content-readings-reading-value");
            this.#element.remUnit              = document.querySelector("#content-readings-reading-rem").querySelector(".content-readings-reading-unit");
            this.#element.rontgenValue         = document.querySelector("#content-readings-reading-rontgen").querySelector(".content-readings-reading-value");
            this.#element.rontgenUnit          = document.querySelector("#content-readings-reading-rontgen").querySelector(".content-readings-reading-unit");
            this.#element.grayValue            = document.querySelector("#content-readings-reading-gray").querySelector(".content-readings-reading-value");
            this.#element.grayUnit             = document.querySelector("#content-readings-reading-gray").querySelector(".content-readings-reading-unit");
            this.#element.ratingTitle          = document.querySelector("#content-rating-value-title");
            this.#element.ratingDescription    = document.querySelector("#content-rating-value-description");
            this.#element.totalCounts          = document.querySelector("#content-tube-info-row-total-counts").querySelector(".content-tube-info-value");
            this.#element.numberOfTubes        = document.querySelector("#content-tube-info-row-number-of-tubes").querySelector(".content-tube-info-value");
            this.#element.tubeType             = document.querySelector("#content-tube-info-row-tube-types").querySelector(".content-tube-info-value");
            this.#element.mainTubeAbsolute     = document.querySelector("#content-tube-info-row-main-absolute").querySelector(".content-tube-info-value");
            this.#element.mainTubeRelative     = document.querySelector("#content-tube-info-row-main-relative").querySelector(".content-tube-info-value");
            this.#element.followerTubeAbsolute = document.querySelector("#content-tube-info-row-follower-absolute").querySelector(".content-tube-info-value");
            this.#element.followerTubeRelative = document.querySelector("#content-tube-info-row-follower-relative").querySelector(".content-tube-info-value");

            // Initialize the dialog box
            dialogBox.initialize();

            // Create a new update loop
            this.#updateLoop = setInterval(this.#getGeigerCounterData.bind(this), this.#updateIntervalSeconds * 1000);

            // Update the Geiger counter data immediately
            this.#getGeigerCounterData();

        }

    }

    // ============================================================================================
    // Set the auto range state
    // ============================================================================================
    setAutoRange(state) {

        // Set the auto range state
        this.#autoRange = state;

        // Update the Geiger counter values
        this.#updateValues();

    }

    // ============================================================================================
    // Set the screen unit
    // ============================================================================================
    setScreenUnit(unit) {

        // Set the screen unit
        this.#screenUnit = unit;

        // Update the Geiger counter values
        this.#updateValues();

    }

    // ============================================================================================
    // Set the update interval
    // ============================================================================================
    setUpdateInterval(timeSeconds) {

        // Clear the previous update loop
        clearInterval(this.#updateLoop);

        // Set the new update interval time
        this.#updateIntervalSeconds = timeSeconds;

        // Get the newest Geiger counter data
        this.#getGeigerCounterData();

        // Create a new update loop
        this.#updateLoop = setInterval(this.#getGeigerCounterData.bind(this), this.#updateIntervalSeconds * 1000);

    }

    // ============================================================================================
    // Set Geiger counter properties
    // ============================================================================================
    setProperties(properties) {

        // Clear the previous update loop
        clearInterval(this.#updateLoop);

        // Set new properties
        this.#autoRange             = properties.autoRange;
        this.#screenUnit            = properties.screenUnit;
        this.#updateIntervalSeconds = properties.updateIntervalSeconds;

        // Create a new update loop
        this.#updateLoop = setInterval(this.#getGeigerCounterData.bind(this), this.#updateIntervalSeconds * 1000);

        // Update the Geiger counter
        this.#updateValues();

    }

}

// Global Geiger counter object
const geigerCounter = new GeigerCounter();