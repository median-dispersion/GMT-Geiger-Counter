class RadiationHistory {

    // --------------------------------------------------------------------------------------------
    // Private

    // Radiation history properties
    #initialized           = false;
    #verticalSteps         = 5;
    #horizontalSteps       = 5;
    #smoothingFactor       = 0.0;
    #context               = null;
    #resolutionFactor      = 1.0;
    #horizontalScale       = 1.1;
    #updateIntervalSeconds = 90;

    // Geiger counter data from log file
    #geigerCounterData;

    // Radiation history data
    #radiationHistoryData = {

        average:  0,
        maximum:  0,
        minimum:  0,
        duration: 0,
        samples:  []

    };

    // DOM elements
    #element = {

        canvas:  null,
        average: null,
        minimum: null,
        maximum: null

    };

    // CSS styles
    #style = {

        "--theme-color-background":  null,
        "--theme-color-foreground":  null,
        "--theme-color-active":      null,
        "--accent-color-background": null,
        "--accent-color-foreground": null,
        "--font-family-default":     null,
        "--font-size-description":   null,
        "--font-color-info":         null

    };

    // Update loop
    #updateLoop;

    // ============================================================================================
    // Get the latest log file data from the API endpoint
    // ============================================================================================
    async #getLogFileData() {

        // Try getting log file data
        try {
            
            // Log file data API endpoint
            const url = `${window.location.origin}/data/log`;

            // Request log file data
            const response = await fetch(url, {method: "GET", cache: "no-store"});

            // If the response is not a 200 - OK, throw an error
            if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }

            // Parse the log file data as text
            const logData = await response.text();

            // Log info message
            console.info("Updated log file data!");

            // Get the geiger counter data from the log file data
            this.#getGeigerCounterData(logData);

            // Update the radiation history
            this.#updateHistory();

        // If an error occurs requesting the latest log file data
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
            console.error(`Requesting log file data failed! Error message: '${error}'`);

        }

    }

    // ============================================================================================
    // Get the Geiger counter data from the raw log file data
    // ============================================================================================
    #getGeigerCounterData(logData) {

        // If the raw log file data ends with a trailing "," remove it
        logData = logData.replace(/,$/, "");

        // Split the raw log file data into parts delimited by "},{"
        const parts = logData.split(/(?<=})\s*,\s*(?={)/);

        // Geiger counter data array
        const geigerCounterData = [];

        // For each part in the split log file data
        parts.forEach(part => {

            // Try parsing the part as JSON
            try {

                // Parse part as JSON
                const message = JSON.parse(part);

                // Check if the message is a Geiger counter message
                if ("type" in message && message.type == "geigerCounter"){

                    // Add the message to Geiger counter data
                    geigerCounterData.push(message);

                }

            // If an error occurs parsing the part as JSON
            } catch (error) {

                // Log warning message
                console.warn(`Could not parse '${part}' as JSON! Error message: '${error}'`);

            } 
            
        });

        // If there is any Geiger counter data in the log file data
        if (geigerCounterData.length > 0) {

            // Set the Geiger counter data
            this.#geigerCounterData = geigerCounterData;
        
        // If not Geiger counter data in log file data
        } else {

            // Log warning message
            console.warn("No Geiger counter messages in log file data!");

        }

    }

    // ============================================================================================
    // Get the radiation history data from the Geiger counter data
    // ============================================================================================
    #getRadiationHistoryData() {

        // Initialize radiation history data
        const radiationHistoryData = {

            average:  0,
            maximum:  this.#geigerCounterData[0].data.countsPerMinute,
            minimum:  this.#geigerCounterData[0].data.countsPerMinute,
            duration: this.#geigerCounterData[this.#geigerCounterData.length-1].time,
            samples:  []

        };

        // Calculate the maximum number of samples based on the smoothing factor
        // Clamped to at least 2 and not more that the length of the Geiger counter data
        const maximumSamples = Math.min(Math.max(Math.round(this.#geigerCounterData.length * (1 - this.#smoothingFactor)), 2), this.#geigerCounterData.length);

        // Calculate the bin size
        const binSize = this.#geigerCounterData.length / maximumSamples;

        // For each sample up to the maximum number of samples
        for (let sample = 0; sample < maximumSamples; sample++) {
            
            // Calculate the sample start and end index
            const sampleStart = Math.floor(binSize * sample);
            const sampleEnd   = Math.floor(binSize * (sample + 1));

            // Get the sample bin
            const bin = this.#geigerCounterData.slice(sampleStart, sampleEnd);

            // Initialize bin average
            let binAverage = 0;

            // Each entry in the Geiger counter data
            bin.forEach(geigerCounterData => {
                
                // Add the cpm reading to the overall total radiation history average
                radiationHistoryData.average += geigerCounterData.data.countsPerMinute;

                // Change the minimum or maximum values of the radiation history if applicable
                if (geigerCounterData.data.countsPerMinute < radiationHistoryData.minimum) { radiationHistoryData.minimum = geigerCounterData.data.countsPerMinute; }
                if (geigerCounterData.data.countsPerMinute > radiationHistoryData.maximum) { radiationHistoryData.maximum = geigerCounterData.data.countsPerMinute; }

                // Add the cpm reading to the bin average
                binAverage += geigerCounterData.data.countsPerMinute;

            });

            // Calculate the bin average
            binAverage /= bin.length;

            // Add the bin average to the radiation history samples array
            radiationHistoryData.samples.push(binAverage);

        }

        // Calculate the radiation history average
        radiationHistoryData.average /= this.#geigerCounterData.length;

        // Set the radiation history data
        this.#radiationHistoryData = radiationHistoryData;

    }

    // ============================================================================================
    // Set the radiation history statistics values
    // ============================================================================================
    #setHistoryStatistics() {

        // Set the statistics values
        this.#element.average.innerHTML = Math.round(this.#radiationHistoryData.average);
        this.#element.maximum.innerHTML = this.#radiationHistoryData.maximum;
        this.#element.minimum.innerHTML = this.#radiationHistoryData.minimum;

    }

    // ============================================================================================
    // Draw the vertical legend to the canvas
    // ============================================================================================
    #drawVerticalLegend() {

        // Change legend styling
        const lineWidth           = Math.round(1 * this.#resolutionFactor);
        const fontSize            = parseFloat(this.#style["--font-size-description"]) * this.#resolutionFactor;
        this.#context.lineWidth   = lineWidth;
        this.#context.strokeStyle = this.#style["--theme-color-foreground"];
        this.#context.font        = `${fontSize}px ${this.#style["--font-family-default"]}`;
        this.#context.fillStyle   = this.#style["--font-color-info"];
        
        // Calculate the position step width
        const positionStepWidth = this.#element.canvas.width / (this.#verticalSteps - 1);

        // Calculate the time step duration
        const timeStepDuration = this.#radiationHistoryData.duration / (this.#verticalSteps - 1);

        // For each vertical step
        for (let step = 0; step < this.#verticalSteps; step++) {
            
            // Calculate the line position for the step, if the line width is odd offset the position by half a pixel
            const position = lineWidth % 2 === 0 ? Math.round(positionStepWidth * step): Math.round(positionStepWidth * step) + 0.5;

            // Calculate the time for the step
            const time = timeStepDuration * ((this.#verticalSteps - 1) - step);

            // If not the first or last step
            if (step > 0 && step < this.#verticalSteps - 1) {

                // Draw the line to the canvas
                this.#context.beginPath();
                this.#context.moveTo(position, 0);
                this.#context.lineTo(position, this.#element.canvas.height);
                this.#context.stroke();

            }

            // If not the last step
            if (step < this.#verticalSteps - 1) {

                // Generate a time string in minutes, or in hours if time is more than 120 minutes
                const timeString = time < 7200000 ? `${Math.round(time / 60000)}${string.minute_abbreviation}` : `${(time / 3600000).toFixed(1)}${string.hour_abbreviation}`;

                // Draw the time string to the canvas
                this.#context.fillText(timeString, position + (2 * this.#resolutionFactor), this.#element.canvas.height - (4 * this.#resolutionFactor));

            }

        }

    }

    // ============================================================================================
    // Draw the horizontal legend to the canvas
    // ============================================================================================
    #drawHorizontalLegend() {

        // Change legend styling
        const lineWidth           = Math.round(1 * this.#resolutionFactor);
        const fontSize            = parseFloat(this.#style["--font-size-description"]) * this.#resolutionFactor;
        this.#context.lineWidth   = lineWidth;
        this.#context.strokeStyle = this.#style["--theme-color-foreground"];
        this.#context.font        = `${fontSize}px ${this.#style["--font-family-default"]}`;
        this.#context.fillStyle   = this.#style["--font-color-info"];

        // Calculate the position step height
        const positionStepHeight = this.#element.canvas.height / (this.#horizontalSteps - 1);

        // Calculate value step amount
        const valueStepAmount = (this.#radiationHistoryData.maximum * this.#horizontalScale) / (this.#horizontalSteps - 1);

        // For each horizontal step
        for (let step = 0; step < this.#horizontalSteps; step++) {
            
            // Calculate the line position for the step, if the line width is odd offset the position by half a pixel
            const position = lineWidth % 2 === 0 ? Math.round(positionStepHeight * step): Math.round(positionStepHeight * step) + 0.5;

            // Calculate the step value
            const value = Math.round(valueStepAmount * ((this.#horizontalSteps - 1) - step));

            // If not the first or last step
            if (step > 0 && step < this.#horizontalSteps - 1) {

                // Draw the line to the canvas
                this.#context.beginPath();
                this.#context.moveTo(0, position);
                this.#context.lineTo(this.#element.canvas.width, position);
                this.#context.stroke();

            }

            // If not the last step
            if (step < this.#horizontalSteps - 1) {

                // Draw the value string to the canvas
                this.#context.fillText(value, 2 * this.#resolutionFactor, position + (12 * this.#resolutionFactor));

            }
            
        }

    }

    // ============================================================================================
    // The the history graph to the canvas
    // ============================================================================================
    #drawGraph(fill = false) {

        // Calculate step width and height
        const stepWidth  = this.#element.canvas.width  / (this.#radiationHistoryData.samples.length - 1);
        const stepHeight = this.#element.canvas.height / (this.#radiationHistoryData.maximum * this.#horizontalScale)

        // Get X and Y starting position
        let x = 0;
        let y = this.#element.canvas.height - (stepHeight * this.#radiationHistoryData.samples[0]);

        // Begin path
        this.#context.beginPath();
        this.#context.moveTo(x, y);

        // For each sample
        for (let sample = 1; sample < this.#radiationHistoryData.samples.length; sample++) {
            
            // Calculate the X and Y position of the sample
            x = stepWidth * sample;
            y = this.#element.canvas.height - (stepHeight * this.#radiationHistoryData.samples[sample]);

            // Draw a line to the sample position
            this.#context.lineTo(x, y);
            
        }

        // Change line and fill style
        this.#context.lineWidth   = 3 * this.#resolutionFactor;
        this.#context.lineCap     = "round";
        this.#context.lineJoin    = "round";
        this.#context.strokeStyle = this.#style["--accent-color-foreground"];
        this.#context.fillStyle   = this.#style["--accent-color-background"];

        // If the graph should be filled
        if (fill) {
            
            // Complete the graph polygon
            this.#context.lineTo(x, this.#element.canvas.height);
            this.#context.lineTo(0, this.#element.canvas.height);
            this.#context.closePath();

            // Fill the graph
            this.#context.fill();
        
        // If only the graph line should be drawn
        } else {

            // Draw the graph line
            this.#context.stroke();

        }

    }

    // ============================================================================================
    // Draw the radiation history to the canvas
    // ============================================================================================
    #drawHistory() {

        // Draw the canvas background
        this.#context.fillStyle = this.#style["--theme-color-background"];
        this.#context.fillRect(0, 0, this.#element.canvas.width, this.#element.canvas.height);

        // Fill the graph background
        this.#drawGraph(true);

        // Draw the graph legend
        this.#drawVerticalLegend();
        this.#drawHorizontalLegend();

        // Draw the graph line
        this.#drawGraph();

    }

    // ============================================================================================
    // Resize the canvas
    // ============================================================================================
    #resizeCanvas() {

        // Reset the canvas with an height
        this.#element.canvas.width  = "";
        this.#element.canvas.height = "";
        
        // Get the computed style of the canvas
        const style = getComputedStyle(this.#element.canvas);
        
        // Get the device pixel ratio and use it as a resolution factor
        this.#resolutionFactor = window.devicePixelRatio || 1;

        // Set the canvas width and height to its computed size and add the resolution factor
        this.#element.canvas.width  = parseFloat(style.width)  * this.#resolutionFactor;
        this.#element.canvas.height = parseFloat(style.height) * this.#resolutionFactor;
        
        // Redraw the canvas
        this.#drawHistory();

    }

    // ============================================================================================
    // Update the radiation history
    // ============================================================================================
    #updateHistory() {

        // If there is any Geiger counter data
        if (this.#geigerCounterData !== undefined) {

            // Get the radiation history data
            this.#getRadiationHistoryData();

            // Set the radiation history statistics
            this.#setHistoryStatistics();

            // Draw the history to the canvas
            this.#drawHistory();

        // If there is no Geiger counter data available
        } else {

            // Log a warning message
            console.warn("Could not update the radiation history, no Geiger counter data available!");

        }

    }

    // --------------------------------------------------------------------------------------------
    // Public

    // ============================================================================================
    // Initialize the radiation history
    // ============================================================================================
    initialize() {

        // If not initialized
        if(!this.#initialized) {

            // Get DOM elements
            this.#element.canvas  = document.querySelector("#content-history-graph-canvas");
            this.#element.average = document.querySelector("#content-history-statistics-info-value-average");
            this.#element.maximum = document.querySelector("#content-history-statistics-info-value-maximum");
            this.#element.minimum = document.querySelector("#content-history-statistics-info-value-minimum");

            // Get CSS root styles
            const style = getComputedStyle(document.documentElement);

            // Get custom CSS variables
            this.#style["--theme-color-background"]  = style.getPropertyValue("--theme-color-background").trim();
            this.#style["--theme-color-foreground"]  = style.getPropertyValue("--theme-color-foreground").trim();
            this.#style["--theme-color-active"]      = style.getPropertyValue("--theme-color-active").trim();
            this.#style["--accent-color-background"] = style.getPropertyValue("--accent-color-background").trim();
            this.#style["--accent-color-foreground"] = style.getPropertyValue("--accent-color-foreground").trim();
            this.#style["--font-family-default"]     = style.getPropertyValue("--font-family-default").trim();
            this.#style["--font-size-description"]   = style.getPropertyValue("--font-size-description").trim();
            this.#style["--font-color-info"]         = style.getPropertyValue("--font-color-info").trim();

            // Initialize the dialog box
            dialogBox.initialize();

            // Get the 2D render context
            this.#context = this.#element.canvas.getContext("2d");

            // Add an event listener that resizes the canvas when the windows is resized
            window.addEventListener("resize", this.#resizeCanvas.bind(this));

            // Resize the canvas immediately
            this.#resizeCanvas();

            // Create a new update loop
            this.#updateLoop = setInterval(this.#getLogFileData.bind(this), this.#updateIntervalSeconds * 1000);

            // Update the log file data immediately
            this.#getLogFileData();

            // Set the initialization flag to true
            this.#initialized = true;

        }

    }

    // ============================================================================================
    // Set the number of vertical steps on the radiation history legend
    // ============================================================================================
    setVerticalSteps(steps) {

        // Set the number of vertical steps
        this.#verticalSteps = steps;

        // Redraw the history graph
        this.#drawHistory();

    }

    // ============================================================================================
    // Set the number of horizontal steps on the radiation history legend
    // ============================================================================================
    setHorizontalSteps(steps) {

        // Set the number of horizontal steps
        this.#horizontalSteps = steps;

        // Redraw the history graph
        this.#drawHistory();

    }

    // ============================================================================================
    // Set the smoothing factor for the radiation history sample data
    // ============================================================================================
    setSmoothingFactor(smoothing) {

        // Set the smoothing factor
        this.#smoothingFactor = smoothing;

        // Update the radiation history
        this.#updateHistory();

    }

    // ============================================================================================
    // Set radiation history properties
    // ============================================================================================
    setProperties(properties) {

        // Set new properties
        this.#verticalSteps   = properties.verticalSteps;
        this.#horizontalSteps = properties.horizontalSteps;
        this.#smoothingFactor = properties.smoothingFactor;

        // Redraw the radiation history
        this.#drawHistory();

        // Update the radiation history data
        this.#updateHistory();

    }

    // ============================================================================================
    // Save the history graph as an image
    // ============================================================================================
    save(width = 1920, height = 1080) {

        // Set the new canvas resolution factor
        this.#resolutionFactor = width / this.#element.canvas.width;

        // Set the new canvas resolution
        this.#element.canvas.width  = width;
        this.#element.canvas.height = height;
    
        // Redraw the history graph at the new resolution
        this.#drawHistory();

        // Convert the canvas into an image
        const dataURL = this.#element.canvas.toDataURL("image/png");

        // Create a download element for the image and download it
        const link = document.createElement("a");
        link.download = `Radiation_History_${new Date().toISOString()}.png`;
        link.href = dataURL;
        link.click();

        // Resize the canvas to its previous size
        this.#resizeCanvas();

    }

}

// Global radiation history object
const radiationHistory = new RadiationHistory();