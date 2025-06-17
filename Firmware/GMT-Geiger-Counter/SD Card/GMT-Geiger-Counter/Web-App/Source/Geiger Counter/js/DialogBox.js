class DialogBox {

    // --------------------------------------------------------------------------------------------
    // Private

    // Dialog box properties
    #initialized = false;

    // DOM elements
    #element = {

        dialog:  null,
        box:     null,
        title:   null,
        message: null,
        actions: null

    };

    // Dialog animation handler
    #dialogAnimation;

    // --------------------------------------------------------------------------------------------
    // Public

    // ============================================================================================
    // Initialize the dialog box
    // ============================================================================================
    initialize() {

        // If not initialized
        if(!this.#initialized) {

            // Get DOM elements
            this.#element.dialog   = document.querySelector("#dialog");
            this.#element.box      = document.querySelector("#dialog-box");
            this.#element.title    = document.querySelector("#dialog-box-title");
            this.#element.message  = document.querySelector("#dialog-box-message");
            this.#element.actions  = document.querySelector("#dialog-box-actions");

            // Set initialization flag to true
            this.#initialized = true;

        }

    }

    // ============================================================================================
    // 
    // ============================================================================================
    notify(title, message, actions = []) {

        // Remove the old event listener
        this.#element.dialog.removeEventListener("animationend", this.#dialogAnimation);

        // Remove the old animation classes
        this.#element.dialog.classList.remove("dialog-fade-out");
        this.#element.box.classList.remove("dialog-box-scale-up");

        // Set the notification title and message
        this.#element.title.innerHTML   = title;
        this.#element.message.innerHTML = message;

        // Clear old action elements
        this.#element.actions.innerHTML = "";

        // Stop rendering the actions element
        this.#element.actions.style.display = "none";

        // For each action
        actions.forEach(action => {
            
            // Create a button element
            const button = document.createElement("button");

            // Set the class name of the button
            button.className = "dialog-box-actions-button";

            // Set the button title
            button.innerHTML = action.title;

            // Set the button onclick event
            button.onclick = action.event;

            // Append the button to the actions element
            this.#element.actions.appendChild(button);

            // Render the actions element
            this.#element.actions.style.display = "flex";

        });

        // Prevent page scroll
        document.body.style.overflow = "hidden";

        // Add the animation classes
        this.#element.dialog.classList.add("dialog-fade-in");
        this.#element.box.classList.add("dialog-box-scale-up");

        // Render the dialog element
        this.#element.dialog.style.display = "grid";

    }

    // ============================================================================================
    // 
    // ============================================================================================
    dismiss() {

        // Remove the old event listener
        this.#element.dialog.removeEventListener("animationend", this.#dialogAnimation);

        // Remove old animation classes
        this.#element.dialog.classList.remove("dialog-fade-in");

        // Dialog animation event handler
        this.#dialogAnimation = () => {

            // Stop rendering the dialog element
            this.#element.dialog.style.display = "none";

            // Allow page scroll
            document.body.style.overflow = "auto";

            // Remove the old animation classes
            this.#element.dialog.classList.remove("dialog-fade-out");
            this.#element.box.classList.remove("dialog-box-scale-up");

            // Remove the event listener
            this.#element.dialog.removeEventListener("animationend", this.#dialogAnimation);

        };

        // Add the event listener for stopping the animation
        this.#element.dialog.addEventListener("animationend", this.#dialogAnimation);

        // Add new animation classes
        this.#element.dialog.classList.add("dialog-fade-out");

    }

}

// Global dialog box object
const dialogBox = new DialogBox();