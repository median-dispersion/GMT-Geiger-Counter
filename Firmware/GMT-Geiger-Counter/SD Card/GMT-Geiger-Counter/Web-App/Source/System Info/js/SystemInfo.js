class SystemInfo {

    // --------------------------------------------------------------------------------------------
    // Private

    // Initialization flag
    #initialized = false;

    // DOM elements
    #element = {

        uptime:            null,
        memory:            null,
        sdCard:            null,
        geigerCounter:     null,
        cosmicRayDetector: null,
        buzzer:            null,
        touchscreen:       null,
        led:               null,
        hotspot:           null,
        wifi:              null,
        server:            null,
        firmware:          null

    };

    // ============================================================================================
    // 
    // ============================================================================================
    async #getSystemInfoData() {

        // Try getting system data
        try {

            // System data API endpoint url
            const url = `${window.location.origin}/data/system`;

            // Get the system data
            const response = await fetch(url, {method: "GET", cache: "no-store"});

            // If the response is not a 200 - OK, throw an error
            if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }

            // Parse the data as JSON
            const message = await response.json();

            // Set system info data
            this.#setSystemUptime(message.data);
            this.#setMemoryUsage(message.data);
            this.#setComponentStates(message.data);
            this.#setFirmwareVersion(message.data);

        } catch (error) {

            // Display an error dialog
            dialogBox.notify(
                string.data_request_failed,
                `${string.data_request_failed_message} (${error})`,
                [{title: string.go_back, event: () => { window.location.href = window.location.origin; }},
                 {title: string.refresh, event: () => { location.reload();                             }}]
            );

            // Log an error message
            console.error(`Requesting system data failed! Error message: '${error}'`);
            
        }

    }

    // ============================================================================================
    // Set the system uptime
    // ============================================================================================
    #setSystemUptime(data) {

        // Calculate time string components
        const totalSeconds = Math.floor(data.uptime / 1000);
        const seconds      = totalSeconds % 60;
        const minutes      = Math.floor((totalSeconds % 3600) / 60);
        const hours        = Math.floor((totalSeconds % 86400) / 3600);
        const days         = Math.floor(totalSeconds / 86400);
        
        // Time string components
        const parts = [];

        // Time string
        let timeString = "";

        // If at least 1 day has passed add it to the time string
        if (days > 0) { timeString = `${days}${string.day_abbreviation} `; }

        // If at least 1 hour has passed add it to the time string components
        if (hours > 0 || days > 0) { parts.push(hours.toString().padStart(2, "0")); }

        // Always include minutes and seconds
        parts.push(minutes.toString().padStart(2, "0"));
        parts.push(seconds.toString().padStart(2, "0"));

        // Add time components to the time string
        timeString += parts.join(":");

        // Set the time string
        this.#element.uptime.innerHTML = timeString;                

    }

    // ============================================================================================
    // Set the memory usage
    // ============================================================================================
    #setMemoryUsage(data) {

        // Format a number to a string with seperations
        const formatNumber = (number) => {
            const string = number.toString();
            if (string.length <= 3) return string;
            return string.slice(0, -3) + "," + string.slice(-3);
        };

        // Get memory info
        const freeMemory  = formatNumber(data.freeHeap);
        const totalMemory = formatNumber(data.heapSize);
        const percentage  = ((data.freeHeap / data.heapSize) * 100).toFixed(1);

        // Set the memory usage
        this.#element.memory.innerHTML = `${freeMemory} / ${totalMemory} (${percentage}%)`;

    }

    // ============================================================================================
    // Set system component states
    // ============================================================================================
    #setComponentStates(data) {

        if (data.sdCard)            { this.#element.sdCard.classList.add("content-components-entry-state-active");            }
        if (data.geigerCounter)     { this.#element.geigerCounter.classList.add("content-components-entry-state-active");     }
        if (data.cosmicRayDetector) { this.#element.cosmicRayDetector.classList.add("content-components-entry-state-active"); }
        if (data.buzzer)            { this.#element.buzzer.classList.add("content-components-entry-state-active");            }
        if (data.touchscreen)       { this.#element.touchscreen.classList.add("content-components-entry-state-active");       }
        if (data.led)               { this.#element.led.classList.add("content-components-entry-state-active");               }
        if (data.hotspot)           { this.#element.hotspot.classList.add("content-components-entry-state-active");           }
        if (data.wifi)              { this.#element.wifi.classList.add("content-components-entry-state-active");              }
        if (data.server)            { this.#element.server.classList.add("content-components-entry-state-active");            }

    }

    // ============================================================================================
    // Set system component states
    // ============================================================================================
    #setFirmwareVersion(data) {

        this.#element.firmware.innerHTML = `${string.firmware}: ${data.firmware}`;

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
            this.#element.uptime            = document.querySelector("#content-state-info-uptime").querySelector(".content-state-info-value");
            this.#element.memory            = document.querySelector("#content-state-info-memory").querySelector(".content-state-info-value");
            this.#element.sdCard            = document.querySelector("#content-components-entry-sd-card").querySelector(".content-components-entry-state");
            this.#element.geigerCounter     = document.querySelector("#content-components-entry-geiger-counter").querySelector(".content-components-entry-state");
            this.#element.cosmicRayDetector = document.querySelector("#content-components-entry-cosmic-ray-detector").querySelector(".content-components-entry-state");
            this.#element.buzzer            = document.querySelector("#content-components-entry-buzzer").querySelector(".content-components-entry-state");
            this.#element.touchscreen       = document.querySelector("#content-components-entry-touchscreen").querySelector(".content-components-entry-state");
            this.#element.led               = document.querySelector("#content-components-entry-led").querySelector(".content-components-entry-state");
            this.#element.hotspot           = document.querySelector("#content-components-entry-hotspot").querySelector(".content-components-entry-state");
            this.#element.wifi              = document.querySelector("#content-components-entry-wifi").querySelector(".content-components-entry-state");
            this.#element.server            = document.querySelector("#content-components-entry-server").querySelector(".content-components-entry-state");
            this.#element.firmware          = document.querySelector("#content-title-firmware");

            // Initialize dialog box
            dialogBox.initialize();

            // Get system data
            this.#getSystemInfoData();

        }

    }

}

// Global system info object
const systemInfo = new SystemInfo();