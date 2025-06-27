class SettingsPanel {

    // --------------------------------------------------------------------------------------------
    // Private

    // Initialization flag
    #initialized = false;

    // Settings values
    #value = {

        version:               2,
        autoRange:             true,
        screenUnit:            "sieverts",
        updateIntervalSeconds: 10,
        verticalSteps:         5,
        horizontalSteps:       5,
        smoothingFactor:       0,
        enableAudio:           false,
        simulateDetections:    true,
        playWarning:           true,
        playAlarm:             true,
        volume:                50

    };

    // DOM elements
    #element = {

        settings:        null,
        background:      null,
        panel:           null,
        close:           null,
        autoRange:       null,
        sieverts:        null,
        rem:             null,
        rontgen:         null,
        gray:            null,
        updateInterval:  null,
        verticalSteps:   null,
        horizontalSteps: null,
        smoothingFactor: null,
        saveHistory:     null,
        downloadLog:     null,
        restartSystem:   null,
        enableAudio:     null,
        detections:      null,
        warning:         null,
        alarm:           null,
        volume:          null

    };

    // Settings animation handler
    #settingsAnimation;

    // ============================================================================================
    // Apply current settings
    // ============================================================================================
    #apply() {

        // Apply values to settings elements
        this.#element.autoRange.checked     = this.#value.autoRange;
        this.#element.updateInterval.value  = this.#value.updateIntervalSeconds;
        this.#element.verticalSteps.value   = this.#value.verticalSteps;
        this.#element.horizontalSteps.value = this.#value.horizontalSteps;
        this.#element.smoothingFactor.value = this.#value.smoothingFactor;
        this.#element.enableAudio.checked   = this.#value.enableAudio;
        this.#element.detections.checked    = this.#value.simulateDetections;
        this.#element.warning.checked       = this.#value.playWarning;
        this.#element.alarm.checked         = this.#value.playAlarm;
        this.#element.volume.value          = this.#value.volume;

        // Depending on the selected screen unit apply it to the appropriate radio button
        switch (this.#value.screenUnit) {

            case "sieverts": this.#element.sieverts.checked = true; break;
            case "rem":      this.#element.rem.checked      = true; break;
            case "rontgen":  this.#element.rontgen.checked  = true; break;
            case "gray":     this.#element.gray.checked     = true; break;

            default: this.#element.screenUnit.sieverts.checked = true; break;

        }

        // Convert values to JSON string and store in localStorage
        localStorage.setItem("geigerCounterSettings", JSON.stringify(this.#value));

    }

    // ============================================================================================
    // Load settings from local storage
    // ============================================================================================
    #load() {

        // Try loading values
        try {

            // Load values from local storage
            let value = localStorage.getItem("geigerCounterSettings");

            // Parse values string as JSON
            value = JSON.parse(value);

            // If the loaded version of the values is not the current settings version throw an error
            if (value.version !== this.#value.version) { throw Error("Settings version mismatch!"); }

            // Reset important values
            value.smoothingFactor = 0;
            value.enableAudio     = false;

            // Set current values to loaded values
            this.#value = value;

            // Log info message
            console.info("Loaded settings");

        // If an error occurs loading the values
        } catch (error) {

            // Log warning message
            console.warn(`Could not load stored settings using defaults instead (${error})`);
            
        }

    }

    // ============================================================================================
    // Update the auto ranging value
    // ============================================================================================
    #changeAutoRange(event) {

        // Set new value
        this.#value.autoRange = event.currentTarget.checked;

        // Update the Geiger counter with the new value
        geigerCounter.setAutoRange(this.#value.autoRange);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the screen unit
    // ============================================================================================
    #changeScreenUnit(event) {

        // Set new value
        this.#value.screenUnit = event.currentTarget.value;

        // Update the Geiger counter with the new value
        geigerCounter.setScreenUnit(this.#value.screenUnit);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the update interval
    // ============================================================================================
    #changeUpdateInterval(event) {

        // Set new value
        this.#value.updateIntervalSeconds = event.currentTarget.value;

        // Update the Geiger counter with the new value
        geigerCounter.setUpdateInterval(this.#value.updateIntervalSeconds);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the vertical steps
    // ============================================================================================
    #changeVerticalSteps(event) {

        // Set new value
        this.#value.verticalSteps = event.currentTarget.value;

        // Update the radiation history with the new value
        radiationHistory.setVerticalSteps(this.#value.verticalSteps);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the horizontal steps
    // ============================================================================================
    #changeHorizontalSteps(event) {

        // Set new value
        this.#value.horizontalSteps = event.currentTarget.value;

        // Update the radiation history with the new value
        radiationHistory.setHorizontalSteps(this.#value.horizontalSteps);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the smoothing factor
    // ============================================================================================
    #changeSmoothingFactor(event) {

        // Set new value
        this.#value.smoothingFactor = event.currentTarget.value;

        // Update the radiation history with the new value
        radiationHistory.setSmoothingFactor(this.#value.smoothingFactor / 100);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Save the history as an image
    // ============================================================================================
    #saveHistory() {

        // Save the radiation history as an image
        radiationHistory.save();

    }

    // ============================================================================================
    // Download the log file
    // ============================================================================================
    #downloadLog() {

        // Create a link element
        const link = document.createElement("a");

        // Set the download name
        link.download = `Log_${new Date().toISOString()}.json`;

        // Set the download URL to the log file API endpoint
        link.href = `${window.location.origin}/data/log`;

        // Download the log file
        link.click();

    }

    // ============================================================================================
    // Restart the system
    // ============================================================================================
    #restartSystem() {

        // Display a dialog box
        dialogBox.notify(

            // Dialog box title and message
            string.restart_system,
            string.restart_system_message,

            [
                
                // Button for restarting the system
                {title: string.yes, event: () => {

                    // Restart system API endpoint url
                    const url = `${window.location.origin}/system/restart`;

                    // Restart the system
                    fetch(url, {method: "PUT", cache: "no-store"});

                    // Dismiss the dialog box
                    dialogBox.dismiss();

                }},

                // Button for not restarting the system
                {title: string.no,  event: () => { dialogBox.dismiss(); }}
            
            ]

        );        

    }

    // ============================================================================================
    // Change the enable audio state
    // ============================================================================================
    #changeEnableAudio(event) {

        // Set new value
        this.#value.enableAudio = event.currentTarget.checked;

        // Set the state
        audioFeedback.setEnableState(this.#value.enableAudio);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the simulate detections state
    // ============================================================================================
    #changeSimulateDetections(event) {

        // Set new value
        this.#value.simulateDetections = event.currentTarget.checked;

        // Set the state
        audioFeedback.setSimulateDetectionsState(this.#value.simulateDetections);
        
        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the warning state
    // ============================================================================================
    #changePlayWarning(event) {

        // Set new value
        this.#value.playWarning = event.currentTarget.checked;

        // Set the state
        audioFeedback.setWarningState(this.#value.playWarning);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the alarm state
    // ============================================================================================
    #changePlayAlarm(event) {

        // Set new value
        this.#value.playAlarm = event.currentTarget.checked;

        // Set the state
        audioFeedback.setAlarmState(this.#value.playAlarm);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Change the audio volume
    // ============================================================================================
    #changeVolume(event) {

        // Set new value
        this.#value.volume = event.currentTarget.value;

        // Set the volume
        audioFeedback.setVolume(this.#value.volume / 100);

        // Apply current settings
        this.#apply();

    }

    // ============================================================================================
    // Update a settings input range
    // ============================================================================================
    #updateInputRange(event) {

        // Get DOM elements
        const range = event.currentTarget;
        const thumb = range.parentNode.querySelector(".settings-panel-content-section-range-slider-tack-thumb");
        const value = range.parentNode.parentNode.querySelector(".settings-panel-content-section-range-state-value");

        // Calculate width of range thumb
        const width = (100 / (range.max - range.min)) * (range.value - range.min);

        // Calculate range thumb offset
        const offset = 1 - (width / 100);

        // Apply width and offset to range thumb
        thumb.style.width = `calc(${width}% + (24px * ${offset}))`;

        // Set the range value text
        value.innerHTML = range.value;

    }

    // --------------------------------------------------------------------------------------------
    // Public

    // ============================================================================================
    // Initialize the header
    // ============================================================================================
    initialize() {

        // If not already initialized
        if(!this.#initialized) {

            // Set the initialization flag to true
            this.#initialized = true;

            // Get DOM elements
            this.#element.settings        = document.querySelector("#settings");
            this.#element.background      = document.querySelector("#settings-background");
            this.#element.panel           = document.querySelector("#settings-panel");
            this.#element.close           = document.querySelector("#settings-panel-header-button-close");
            this.#element.autoRange       = document.querySelector("#settings-panel-content-section-toggle-auto-range").querySelector(".settings-panel-content-section-toggle-input");
            this.#element.sieverts        = document.querySelector("#settings-panel-content-section-radio-sieverts").querySelector(".settings-panel-content-section-radio-input");
            this.#element.rem             = document.querySelector("#settings-panel-content-section-radio-rem").querySelector(".settings-panel-content-section-radio-input");
            this.#element.rontgen         = document.querySelector("#settings-panel-content-section-radio-rontgen").querySelector(".settings-panel-content-section-radio-input");
            this.#element.gray            = document.querySelector("#settings-panel-content-section-radio-gray").querySelector(".settings-panel-content-section-radio-input");
            this.#element.updateInterval  = document.querySelector("#settings-panel-content-section-range-update-interval").querySelector(".settings-panel-content-section-range-slider-input");
            this.#element.verticalSteps   = document.querySelector("#settings-panel-content-section-range-vertical-steps").querySelector(".settings-panel-content-section-range-slider-input");
            this.#element.horizontalSteps = document.querySelector("#settings-panel-content-section-range-horizontal-steps").querySelector(".settings-panel-content-section-range-slider-input");
            this.#element.smoothingFactor = document.querySelector("#settings-panel-content-section-range-smoothing-factor").querySelector(".settings-panel-content-section-range-slider-input");
            this.#element.saveHistory     = document.querySelector("#settings-panel-content-section-action-save-history");
            this.#element.downloadLog     = document.querySelector("#settings-panel-content-section-action-download-log");
            this.#element.restartSystem   = document.querySelector("#settings-panel-content-section-action-restart-system");
            this.#element.enableAudio     = document.querySelector("#settings-panel-content-section-toggle-enable-audio").querySelector(".settings-panel-content-section-toggle-input");
            this.#element.detections      = document.querySelector("#settings-panel-content-section-toggle-simulate-detection").querySelector(".settings-panel-content-section-toggle-input");
            this.#element.warning         = document.querySelector("#settings-panel-content-section-toggle-warning").querySelector(".settings-panel-content-section-toggle-input");
            this.#element.alarm           = document.querySelector("#settings-panel-content-section-toggle-alarm").querySelector(".settings-panel-content-section-toggle-input");
            this.#element.volume          = document.querySelector("#settings-panel-content-section-range-volume").querySelector(".settings-panel-content-section-range-slider-input");

            // Load settings
            this.#load();

            // Apply loaded settings
            this.#apply();

            // Initialize dialog box
            dialogBox.initialize();

            // Initialize Geiger counter and update its settings
            geigerCounter.initialize();
            geigerCounter.setProperties({

                autoRange:             this.#value.autoRange,
                screenUnit:            this.#value.screenUnit,
                updateIntervalSeconds: this.#value.updateIntervalSeconds

            });
            
            // Initialize radiation history and update its settings
            radiationHistory.initialize();
            radiationHistory.setProperties({

                verticalSteps:   this.#value.verticalSteps,
                horizontalSteps: this.#value.horizontalSteps,
                smoothingFactor: this.#value.smoothingFactor / 100

            });

            // Initialize audio feedback and update its settings
            audioFeedback.initialize();
            audioFeedback.setProperties({

                enableAudio:        this.#value.enableAudio,
                simulateDetections: this.#value.simulateDetections,
                playWarning:        this.#value.playWarning,
                playAlarm:          this.#value.playAlarm,
                volume:             this.#value.volume / 100,

            });
            
            // Add an event listener for closing the settings panel
            this.#element.background.addEventListener("click", this.close.bind(this));
            this.#element.close.addEventListener("click", this.close.bind(this));

            // Add event listeners for changing values
            this.#element.autoRange.addEventListener("change", this.#changeAutoRange.bind(this));
            this.#element.updateInterval.addEventListener("change", this.#changeUpdateInterval.bind(this));
            this.#element.verticalSteps.addEventListener("change", this.#changeVerticalSteps.bind(this));
            this.#element.horizontalSteps.addEventListener("change", this.#changeHorizontalSteps.bind(this));
            this.#element.smoothingFactor.addEventListener("change", this.#changeSmoothingFactor.bind(this));
            this.#element.enableAudio.addEventListener("change", this.#changeEnableAudio.bind(this));
            this.#element.detections.addEventListener("change", this.#changeSimulateDetections.bind(this));
            this.#element.warning.addEventListener("change", this.#changePlayWarning.bind(this));
            this.#element.alarm.addEventListener("change", this.#changePlayAlarm.bind(this));
            this.#element.volume.addEventListener("change", this.#changeVolume.bind(this));

            // For every screen unit radio button add an event listener for the selected screen unit
            document.querySelectorAll("[name='screen-unit']").forEach(radio => {radio.addEventListener("change", this.#changeScreenUnit.bind(this));});
            
            // Add event listeners for action elements
            this.#element.saveHistory.addEventListener("click", this.#saveHistory.bind(this));
            this.#element.downloadLog.addEventListener("click", this.#downloadLog.bind(this));
            this.#element.restartSystem.addEventListener("click", this.#restartSystem.bind(this));

            // For each settings range input
            this.#element.settings.querySelectorAll("input[type='range']").forEach(range => {
                
                // Update the range input
                this.#updateInputRange({currentTarget: range});

                // Add an event listener to update the range input automatically
                range.addEventListener("input", this.#updateInputRange);
            
            });

        }

    }

    // ============================================================================================
    // Open the settings panel
    // ============================================================================================
    open() {

        // Remove the old event listener
        this.#element.settings.removeEventListener("animationend", this.#settingsAnimation);

        // Remove old animation classes
        this.#element.background.classList.remove("settings-background-fade-out");
        this.#element.panel.classList.remove("settings-panel-slide-out");

        // Prevent page scroll
        document.body.style.overflow = "hidden";

        // Add the animation classes
        this.#element.background.classList.add("settings-background-fade-in");
        this.#element.panel.classList.add("settings-panel-slide-in");

        // Render the settings element
        this.#element.settings.style.display = "block";

    }

    // ============================================================================================
    // Close the settings panel
    // ============================================================================================
    close() {

        // Remove the old event listener
        this.#element.settings.removeEventListener("animationend", this.#settingsAnimation);

        // Remove old animation classes
        this.#element.background.classList.remove("settings-background-fade-in");
        this.#element.panel.classList.remove("settings-panel-slide-in");

        // Settings animation event handler
        this.#settingsAnimation = () => {
            
            // Stop rendering the settings element
            this.#element.settings.style.display = "none";

            // Remove old animation classes
            this.#element.background.classList.remove("settings-background-fade-out");
            this.#element.panel.classList.remove("settings-panel-slide-out");

            // Remove the event listener
            this.#element.settings.removeEventListener("animationend", this.#settingsAnimation);

        };

        // Add the event listener for stopping the animation
        this.#element.settings.addEventListener("animationend", this.#settingsAnimation);

        // Add new animation classes
        this.#element.background.classList.add("settings-background-fade-out");
        this.#element.panel.classList.add("settings-panel-slide-out");

        // Allow page scroll
        document.body.style.overflow = "auto";

    }
    
}

// Global settings panel object
const settingsPanel = new SettingsPanel();