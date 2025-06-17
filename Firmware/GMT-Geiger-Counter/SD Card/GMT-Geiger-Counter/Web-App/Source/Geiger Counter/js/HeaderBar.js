class HeaderBar {

    // --------------------------------------------------------------------------------------------
    // Private

    // Initialization flag
    #initialized = false;

    #element = {

        header:   null,
        back:     null,
        settings: null

    };

    // Scroll properties
    #scroll = {

        amount:    0,
        direction: null,
        threshold: 50

    };

    // Hide animation handler
    #hideAnimation;

    // ============================================================================================
    // Handle header behavior for mobile devices
    // ============================================================================================
    #handleMobileDevices() {

        // Check if the device is a mobile device
        if (/android|webos|iphone|ipad|ipod|blackberry|iemobile|opera mini/i.test(navigator.userAgent)) {

            // Add a scroll event listener
            window.addEventListener("scroll", () => {

                // Calculate the scroll amount
                const scroll = window.pageYOffset || document.documentElement.scrollTop;
                const delta  = scroll - this.#scroll.amount;

                // If scrolled more than the threshold
                if (Math.abs(delta) > this.#scroll.threshold) {

                    // If scrolling down
                    if (delta > 0 && this.#scroll.direction !== "down") {

                        // Hide the header
                        this.hide();

                        // Set the scroll direction to down
                        this.#scroll.direction = "down";

                    // If scrolling up
                    } else if (delta < 0 && this.#scroll.direction !== "up") {

                        // Show the header
                        this.show();

                        // Set the scroll direction to up
                        this.#scroll.direction = "up";

                    }

                    // Update the scrolled amount
                    this.#scroll.amount = scroll <= 0 ? 0 : scroll;

                }

            });

        }

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
            this.#element.header   = document.querySelector("#header");
            this.#element.back     = document.querySelector("#header-button-back");
            this.#element.settings = document.querySelector("#header-button-settings");

            // Initialize the settings panel
            settingsPanel.initialize();

            // Handle mobile devices
            this.#handleMobileDevices();

            // Add an event listener to go back
            this.#element.back.addEventListener("click", () => { window.location = window.location.origin; });

            // Add an event listener to open the settings panel
            this.#element.settings.addEventListener("click", () => { settingsPanel.open(); });

        }

    }

    // ============================================================================================
    // Show header
    // ============================================================================================
    show() {

        // Remove old event listener
        this.#element.header.removeEventListener("animationend", this.#hideAnimation);

        // Remove old animation classes
        this.#element.header.classList.remove("header-slide-up");

        // Render header
        this.#element.header.style.display = "flex";

        // Add new animation classes
        this.#element.header.classList.add("header-slide-down");

    }

    // ============================================================================================
    // Hide header
    // ============================================================================================
    hide() {

        // Remove old event listener
        this.#element.header.removeEventListener("animationend", this.#hideAnimation);

        // Remove old animation classes
        this.#element.header.classList.remove("header-slide-down");

        // Hide animation event handler
        this.#hideAnimation = () => {

            // Stop rendering the header
            this.#element.header.style.display = "none";

            // Remove the event listener
            this.#element.header.removeEventListener("animationend", this.#hideAnimation);

        };

        // Add an event listener that checks if the animation has finished playing
        this.#element.header.addEventListener("animationend", this.#hideAnimation);

        // Add animation classes
        this.#element.header.classList.add("header-slide-up");

    }

}

// Global header bar object
const headerBar = new HeaderBar();