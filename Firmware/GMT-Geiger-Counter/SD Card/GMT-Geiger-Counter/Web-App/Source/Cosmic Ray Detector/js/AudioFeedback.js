class AudioFeedback {

    // --------------------------------------------------------------------------------------------
    // Private

    // Audio feedback properties
    #initialized        = false;
    #interacted         = false;
    #volume             = 0.5;
    #simulateEvents     = false;
    #playing            = false;

    // Audio context
    #audioContext = new window.AudioContext();

    // ============================================================================================
    // Resume audio context
    // ============================================================================================
    async #resumeAudioContext() {

        // If the audio context is suspended
        if (this.#audioContext.state === "suspended") {

            // Resume the audio context
            await this.#audioContext.resume();

        }

    }

    // ============================================================================================
    // Play a note
    // ============================================================================================
    #playNote(frequency, durationSeconds) {

        // If there was a previous user interaction with the page and its not muted
        if (this.#interacted && this.#simulateEvents) {

            // Resume audio context if suspended
            this.#resumeAudioContext();

            // Create oscillator and gain node
            const oscillator = this.#audioContext.createOscillator();
            const gainNode = this.#audioContext.createGain();

            // Setup oscillator
            oscillator.type = "sine";
            oscillator.frequency.setValueAtTime(frequency, this.#audioContext.currentTime);

            // Calculate attack sustain and release time of the note
            const attack  = this.#audioContext.currentTime + (durationSeconds * 0.25);
            const sustain = this.#audioContext.currentTime + (durationSeconds * 0.5);
            const release = this.#audioContext.currentTime + durationSeconds;

            // Set gain node timings
            gainNode.gain.setValueAtTime(0.0001, this.#audioContext.currentTime);
            gainNode.gain.exponentialRampToValueAtTime(this.#volume, attack);
            gainNode.gain.exponentialRampToValueAtTime(this.#volume, sustain);
            gainNode.gain.exponentialRampToValueAtTime(0.0001, release);

            // Connect oscillator and gain node
            oscillator.connect(gainNode);
            gainNode.connect(this.#audioContext.destination);

            // Play the note
            oscillator.start();
            oscillator.stop(release);

            // Resolve new promise when note has finished playing
            return new Promise(resolve => setTimeout(resolve, durationSeconds * 1000));

        }

    }

    // --------------------------------------------------------------------------------------------
    // Public

    // ============================================================================================
    // Initialize everything
    // ============================================================================================
    initialize() {

        // If not initialized
        if(!this.#initialized) {

            // Set initialization flag to true
            this.#initialized = true;

            // Event handler for user interaction
            const interaction = () => {

                // Remove the global event listener for user interactions
                document.removeEventListener("click", interaction);

                // Set the interacted flag to true
                this.#interacted = true;

                // Resume the audio context
                this.#resumeAudioContext();

            };

            // Add an event listener to handle global user interactions to resume the audio context
            document.addEventListener("click", interaction);

        }

    }

    // ============================================================================================
    // Set the simulate events state
    // ============================================================================================
    setSimulateEvents(state) {

        this.#simulateEvents = state;

    }

    // ============================================================================================
    // Set the warning state
    // ============================================================================================
    setVolume(volume) {

        this.#volume = volume;

    }

    // ============================================================================================
    // Set audio feedback properties
    // ============================================================================================
    setProperties(properties) {

        this.setSimulateEvents(properties.simulateEvents);
        this.setVolume(properties.volume);

    }

    // ============================================================================================
    // Play a number of event sounds
    // ============================================================================================
    playEvent(events, durationSeconds) {

        // For every event
        for (let event = 0; event < events; event++) {

            // Randomly spread the event sound out over the next update interval
            setTimeout(async () => {

                // If detections are enabled and not already playing something
                if (this.#simulateEvents && !this.#playing) {

                    // Event melody
                    const melody = [

                        {frequency: 2349.318, duration: 0.075},
                        {frequency: 3135.963, duration: 0.075}

                    ];

                    // Set the playback state
                    this.#playing = true;

                    // For every note in the melody
                    for (let note = 0; note < melody.length; note++) {

                        // Play a note
                        await this.#playNote(melody[note].frequency, melody[note].duration);

                    }

                    // Reset the playback state
                    this.#playing = false;

                }

            }, Math.random() * (durationSeconds * 1000));

        }

    }

}

// Global audio feedback object
const audioFeedback = new AudioFeedback();