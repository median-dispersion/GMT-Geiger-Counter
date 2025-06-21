// ================================================================================================
// Initialize web app
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Get DOM elements
    const element = {

        document:                  document.querySelector("html"),
        documentTitle:             document.querySelector("title"),
        headerTitle:               document.querySelector("#header-title"),
        screenUnit:                document.querySelector("#content-screen-unit"),
        doseUnit:                  document.querySelector("#content-dose-reading-unit"),
        coincidenceEventHistory:   document.querySelector("#content-title-radiation-history"),
        tubeStatistics:            document.querySelector("#content-title-tube-statistics"),
        coincidenceTube:           document.querySelector("#content-tube-header-coincidence-tube"),
        coincidenceTubeTotalEvent: document.querySelector("#content-tube-info-row-coincidence-total").querySelector(".content-tube-info-title"),
        mainTube:                  document.querySelector("#content-tube-header-main-tube"),
        mainTubeCounts:            document.querySelector("#content-tube-info-row-main-counts").querySelector(".content-tube-info-title"),
        mainTubeEvents:            document.querySelector("#content-tube-info-row-main-events").querySelector(".content-tube-info-title"),
        mainTubePercentage:        document.querySelector("#content-tube-info-row-main-event-percentage").querySelector(".content-tube-info-title"),
        followerTube:              document.querySelector("#content-tube-header-follower-tube"),
        followerTubeCounts:        document.querySelector("#content-tube-info-row-follower-counts").querySelector(".content-tube-info-title"),
        followerTubeEvents:        document.querySelector("#content-tube-info-row-follower-events").querySelector(".content-tube-info-title"),
        followerTubePercentage:    document.querySelector("#content-tube-info-row-follower-event-percentage").querySelector(".content-tube-info-title"),
        settings:                  document.querySelector("#settings-panel-header-title"),
        updateInterval:            document.querySelector("#settings-panel-content-section-title-update-interval"),
        refreshRate:               document.querySelector("#settings-panel-content-section-range-update-interval").querySelector(".settings-panel-content-section-range-title"),
        refreshDataUnit:           document.querySelector("#settings-panel-content-section-range-update-interval").querySelector(".settings-panel-content-section-range-state-init"),
        eventHistorySettings:      document.querySelector("#settings-panel-content-section-title-event-history"),
        ignoreFirstHour:           document.querySelector("#settings-panel-content-section-toggle-ignore-first-hour").querySelector(".settings-panel-content-section-toggle-title"),
        verticalSteps:             document.querySelector("#settings-panel-content-section-range-vertical-steps").querySelector(".settings-panel-content-section-range-title"),
        horizontalSteps:           document.querySelector("#settings-panel-content-section-range-horizontal-steps").querySelector(".settings-panel-content-section-range-title"),
        smoothingFactor:           document.querySelector("#settings-panel-content-section-range-smoothing-factor").querySelector(".settings-panel-content-section-range-title"),
        actions:                   document.querySelector("#settings-panel-content-section-title-actions"),
        saveHistory:               document.querySelector("#settings-panel-content-section-action-save-history").querySelector(".settings-panel-content-section-action-title"),
        downloadLog:               document.querySelector("#settings-panel-content-section-action-download-log").querySelector(".settings-panel-content-section-action-title"),
        restartSystem:             document.querySelector("#settings-panel-content-section-action-restart-system").querySelector(".settings-panel-content-section-action-title")

    };

    // Selected language based on the browser language or fall back to english
    const selectedLanguage = navigator.language.slice(0, 2) in language ? navigator.language.slice(0, 2) : "en";

    // Set language strings to the selected language
    string = language[selectedLanguage];

    // Set the document language
    element.document.setAttribute("lang", selectedLanguage);

    // Translate DOM element
    element.documentTitle.innerHTML             = string.document_title;
    element.headerTitle.innerHTML               = string.cosmic_ray_detector;
    element.screenUnit.innerHTML                = string.coincidence_events;
    element.doseUnit.innerHTML                  = string.counts_per_hour_abbreviation;
    element.coincidenceEventHistory.innerHTML   = string.coincidence_event_history;
    element.tubeStatistics.innerHTML            = string.tube_statistics;
    element.coincidenceTube.innerHTML           = string.coincidence_tube;
    element.coincidenceTubeTotalEvent.innerHTML = string.total_events;
    element.mainTube.innerHTML                  = string.main_tube;
    element.mainTubeCounts.innerHTML            = string.counts;
    element.mainTubeEvents.innerHTML            = string.events;
    element.mainTubePercentage.innerHTML        = string.event_percentage;
    element.followerTube.innerHTML              = string.follower_tube;
    element.followerTubeCounts.innerHTML        = string.counts;
    element.followerTubeEvents.innerHTML        = string.events;
    element.followerTubePercentage.innerHTML    = string.event_percentage;
    element.settings.innerHTML                  = string.settings;
    element.updateInterval.innerHTML            = string.update_interval;
    element.refreshRate.innerHTML               = string.refresh_rate;
    element.refreshDataUnit.innerHTML           = string.second_abbreviation;
    element.eventHistorySettings.innerHTML      = string.event_history;
    element.ignoreFirstHour.innerHTML           = string.ignore_first_hour;
    element.verticalSteps.innerHTML             = string.vertical_steps;
    element.horizontalSteps.innerHTML           = string.horizontal_steps;
    element.smoothingFactor.innerHTML           = string.smoothing_factor;
    element.actions.innerHTML                   = string.actions;
    element.saveHistory.innerHTML               = string.save_history;
    element.downloadLog.innerHTML               = string.download_log;
    element.restartSystem.innerHTML             = string.restart_system_title;

    // Initialize components
    cosmicRayDetector.initialize();
    eventHistory.initialize();
    headerBar.initialize();
    settingsPanel.initialize();
    dialogBox.initialize();

});