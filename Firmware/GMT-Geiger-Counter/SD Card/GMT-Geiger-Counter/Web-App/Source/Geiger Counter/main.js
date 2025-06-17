// ================================================================================================
// Initialize web app
// ================================================================================================
document.addEventListener("DOMContentLoaded", () => {

    // Get DOM elements
    const element = {

        document:                 document.querySelector("html"),
        documentTitle:            document.querySelector("title"),
        headerTitle:              document.querySelector("#header-title"),
        equivalentDoses:          document.querySelector("#content-title-equivalent-doses"),
        radiationRating:          document.querySelector("#content-title-radiation-rating"),
        radiationHistory:         document.querySelector("#content-title-radiation-history"),
        tubeStatistics:           document.querySelector("#content-title-tube-statistics"),
        tubeInfo:                 document.querySelector("#content-tube-header-tube-info"),
        mainTube:                 document.querySelector("#content-tube-header-main-tube"),
        followerTube:             document.querySelector("#content-tube-header-follower-tube"),
        totalCounts:              document.querySelector("#content-tube-info-row-total-counts").querySelector(".content-tube-info-title"),
        numberOfTubes:            document.querySelector("#content-tube-info-row-number-of-tubes").querySelector(".content-tube-info-title"),
        tubeType:                 document.querySelector("#content-tube-info-row-tube-types").querySelector(".content-tube-info-title"),
        mainTubeAbsolute:         document.querySelector("#content-tube-info-row-main-absolute").querySelector(".content-tube-info-title"),
        mainTubeRelative:         document.querySelector("#content-tube-info-row-main-relative").querySelector(".content-tube-info-title"),
        followerTubeAbsolute:     document.querySelector("#content-tube-info-row-follower-absolute").querySelector(".content-tube-info-title"),
        followerTubeRelative:     document.querySelector("#content-tube-info-row-follower-relative").querySelector(".content-tube-info-title"),
        settings:                 document.querySelector("#settings-panel-header-title"),
        unitSettings:             document.querySelector("#settings-panel-content-section-title-unit-settings"),
        autoRange:                document.querySelector("#settings-panel-content-section-toggle-auto-range").querySelector(".settings-panel-content-section-toggle-title"),
        screenUnit:               document.querySelector("#settings-panel-content-section-title-screen-unit"),
        sieverts:                 document.querySelector("#settings-panel-content-section-radio-sieverts").querySelector(".settings-panel-content-section-radio-title"),
        rem:                      document.querySelector("#settings-panel-content-section-radio-rem").querySelector(".settings-panel-content-section-radio-title"),
        rontgen:                  document.querySelector("#settings-panel-content-section-radio-rontgen").querySelector(".settings-panel-content-section-radio-title"),
        gray:                     document.querySelector("#settings-panel-content-section-radio-gray").querySelector(".settings-panel-content-section-radio-title"),
        updateInterval:           document.querySelector("#settings-panel-content-section-title-update-interval"),
        refreshRate:              document.querySelector("#settings-panel-content-section-range-update-interval").querySelector(".settings-panel-content-section-range-title"),
        refreshDataUnit:          document.querySelector("#settings-panel-content-section-range-update-interval").querySelector(".settings-panel-content-section-range-state-init"),
        radiationHistorySettings: document.querySelector("#settings-panel-content-section-title-radiation-history"),
        verticalSteps:            document.querySelector("#settings-panel-content-section-range-vertical-steps").querySelector(".settings-panel-content-section-range-title"),
        horizontalSteps:          document.querySelector("#settings-panel-content-section-range-horizontal-steps").querySelector(".settings-panel-content-section-range-title"),
        smoothingFactor:          document.querySelector("#settings-panel-content-section-range-smoothing-factor").querySelector(".settings-panel-content-section-range-title"),
        actions:                  document.querySelector("#settings-panel-content-section-title-actions"),
        saveHistory:              document.querySelector("#settings-panel-content-section-action-save-history").querySelector(".settings-panel-content-section-action-title"),
        downloadLog:              document.querySelector("#settings-panel-content-section-action-download-log").querySelector(".settings-panel-content-section-action-title"),
        restartSystem:            document.querySelector("#settings-panel-content-section-action-restart-system").querySelector(".settings-panel-content-section-action-title")

    };

    // Selected language based on the browser language or fall back to english
    const selectedLanguage = navigator.language.slice(0, 2) in language ? navigator.language.slice(0, 2) : "en";

    // Set language strings to the selected language
    string = language[selectedLanguage];

    // Set the document language
    element.document.setAttribute("lang", selectedLanguage);

    // Translate DOM element
    element.documentTitle.innerHTML            = string.document_title;
    element.headerTitle.innerHTML              = string.geiger_counter;
    element.equivalentDoses.innerHTML          = string.equivalent_doses;
    element.radiationRating.innerHTML          = string.radiation_rating;
    element.radiationHistory.innerHTML         = string.radiation_history;
    element.tubeStatistics.innerHTML           = string.tube_statistics;
    element.tubeInfo.innerHTML                 = string.tube_info;
    element.mainTube.innerHTML                 = string.main_tube;
    element.followerTube.innerHTML             = string.follower_tube;
    element.totalCounts.innerHTML              = string.total_counts;
    element.numberOfTubes.innerHTML            = string.number_of_tubes;
    element.tubeType.innerHTML                 = string.tube_type;
    element.mainTubeAbsolute.innerHTML         = string.counts;
    element.mainTubeRelative.innerHTML         = string.relative;
    element.followerTubeAbsolute.innerHTML     = string.counts;
    element.followerTubeRelative.innerHTML     = string.relative;
    element.settings.innerHTML                 = string.settings;
    element.unitSettings.innerHTML             = string.unit_settings;
    element.autoRange.innerHTML                = string.auto_range_units;
    element.screenUnit.innerHTML               = string.main_screen_unit;
    element.sieverts.innerHTML                 = string.sieverts;
    element.rem.innerHTML                      = string.rem;
    element.rontgen.innerHTML                  = string.rontgen;
    element.gray.innerHTML                     = string.gray;
    element.updateInterval.innerHTML           = string.update_interval;
    element.refreshRate.innerHTML              = string.refresh_rate;
    element.refreshDataUnit.innerHTML          = string.second_abbreviation;
    element.radiationHistorySettings.innerHTML = string.radiation_history;
    element.verticalSteps.innerHTML            = string.vertical_steps;
    element.horizontalSteps.innerHTML          = string.horizontal_steps;
    element.smoothingFactor.innerHTML          = string.smoothing_factor;
    element.saveHistory.innerHTML              = string.save_history;
    element.actions.innerHTML                  = string.actions;
    element.downloadLog.innerHTML              = string.download_log;
    element.restartSystem.innerHTML            = string.restart_system_title;

    // Initialize components
    geigerCounter.initialize();
    radiationHistory.initialize();
    headerBar.initialize();
    settingsPanel.initialize();
    dialogBox.initialize();

});