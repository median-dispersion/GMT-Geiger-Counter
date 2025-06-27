class AudioFeedback {

    // --------------------------------------------------------------------------------------------
    // Private

    // Audio feedback properties
    #initialized        = false;
    #interacted         = false;
    #volume             = 0.5;
    #enabled            = false;
    #simulateDetections = true;
    #playWarning        = true;
    #playAlarm          = true;

    // Playback states
    #playing = {

        detection: false,
        warning:   false,
        alarm:     false

    };

    // Alarm interval
    #alarm;

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
        if (this.#interacted && this.#enabled) {

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
    // Play a number of detections sounds
    // ============================================================================================
    playDetections(counts, durationSeconds) {

        // For every count
        for (let count = 0; count < counts; count++) {

            // Randomly spread the detections sound out over the next update interval
            setTimeout(async () => {

                // If detections are enabled and not already playing something
                if (this.#simulateDetections && !this.#playing.detection && !this.#playing.warning) {

                    // If the alarm is not sounding
                    if (!this.#playing.alarm || this.#playing.alarm && !this.#playAlarm) {

                        // Set the playback state
                        this.#playing.detection = true;

                        // Play a note
                        await this.#playNote(2489.016, 0.075);

                        // Reset the playback state
                        this.#playing.detection = false;

                    }

                }

            }, Math.random() * (durationSeconds * 1000));

        }

    }

    // ============================================================================================
    // Play the warning sound
    // ============================================================================================
    async playWarning() {

        // Warning melody
        const melody = [

            {frequency: 2349.318, duration: 0.2},
            {frequency: 2793.826, duration: 0.2},
            {frequency: 2349.318, duration: 0.2},
            {frequency: 2793.826, duration: 0.2},
            {frequency: 0,        duration: 0.5},
            {frequency: 2349.318, duration: 0.2},
            {frequency: 2793.826, duration: 0.2},
            {frequency: 2349.318, duration: 0.2},
            {frequency: 2793.826, duration: 0.2}

        ];

        // Set the playback state
        this.#playing.warning = true;

        // For every note in the melody
        for (let note = 0; note < melody.length; note++) {
            
            // If enabled and not playing the alarm
            if (this.#playWarning && !this.#playing.alarm) {

                // Play the note from the melody
                await this.#playNote(melody[note].frequency, melody[note].duration);

            }

        }

        // Reset the playback state
        this.#playing.warning = false;

    }

    // ============================================================================================
    // Play the alarm sound
    // ============================================================================================
    async playAlarm() {

        // If not already playing the alarm
        if (!this.#playing.alarm) {
        
            // Set playback state
            this.#playing.alarm = true;
            
            // Set the alarm interval
            this.#alarm = setInterval(async () => {
                
                // If enabled
                if (this.#playAlarm) {

                    // Play the alarm note
                    await this.#playNote(2349.318, 0.2);

                }

            }, 201);

        }

    }

    // ============================================================================================
    // Clear the alarm sound
    // ============================================================================================
    clearAlarm() {

        // Clear the alarm interval
        clearInterval(this.#alarm);

        // Reset playback state
        this.#playing.alarm = false;

    }

    // ============================================================================================
    // Set the audio enabled state
    // ============================================================================================
    setEnableState(state) {

        this.#enabled = state;

    }

    // ============================================================================================
    // Set the simulate detections state
    // ============================================================================================
    setSimulateDetectionsState(state) {

        this.#simulateDetections = state;

    }

    // ============================================================================================
    // Set the warning state
    // ============================================================================================
    setWarningState(state) {

        this.#playWarning = state;

    }

    // ============================================================================================
    // Set the warning state
    // ============================================================================================
    setAlarmState(state) {

        this.#playAlarm = state;

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
        
        this.setEnableState(properties.enableAudio);
        this.setSimulateDetectionsState(properties.simulateDetections);
        this.setWarningState(properties.playWarning);
        this.setAlarmState(properties.playAlarm);
        this.setVolume(properties.volume);

    }

}

// Global audio feedback object
const audioFeedback = new AudioFeedback();