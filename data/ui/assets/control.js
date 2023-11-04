var units = ['lightSource', 'inputPump', 'outputPump', 'fanIntake', 'fanExhaus', 'oxygenPump'];
var global_features = [];

var feature_id = "_feature_";

var devices = units.map(function (u) {
    return {
        status: 0,
        unit: u,
        rule: 0,
        onWhen: {
            rule: 0,
            condition: 0,
            value: 0
        },
        offWhen: {
            rule: 0,
            condition: 0,
            value: 0
        }
    }
});

var features = [];

function displayError(msg) {
    $('#messageDiv').show();
    $('#messageDiv').html('<p class="alert alert-danger" role="alert" id="errorDisplay">' + msg + '</p>');
}

function displaySuccess(msg) {
    $('#messageDiv').show();
    $('#messageDiv').html('<p class="alert alert-primary" id="errorDisplay">' + msg + '</p>');
}

function hideMSG() {
    $('#messageDiv').hide();
}

function isTime(v) {
    var regex = /^(2[0-3]|[01]?[0-9]):([0-5]?[0-9])$/;

    return regex.test(v);
}


function isValid(d) {
    // Validate only rule based
    if (d.rule == 1) {
        // Time based RULE validation
        if (d.onWhen.rule == 4) {
            if (isTime(d.onWhen.value) && isTime(d.offWhen.value)) {
                displaySuccess("Updated");
                return true
            } else {
                displayError("Please Fill Rule Conditions!");
                return false
            }
        } else {
            // Validation for all ather rules except Time Based
            if ((d.onWhen.condition > 0 && d.onWhen.value > 0) &&
                (d.offWhen.condition > 0 && d.offWhen.value > 0)) {
                displaySuccess("Updated");
                return true
            } else {
                displayError("Please Fill Rule Conditions!");
                return false
            }
        }
    }

    displaySuccess("Updated");
    return true;
}

function getHourBasedValue(v) {
    if (isTime(v)) {
        var sanitizedInput = v.trim().split(':');
        return ((Number(sanitizedInput[0]) * 60) * 60) + (Number(sanitizedInput[1]) * 60);
    }
}

function run() {
    $.ajax({
        url: "/get-control-data",
        type: "GET",
        success: function (data, status, xhr) {
            mapResponseToDOM(data);
            displaySuccess("Loaded");
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage);
        }
    });
}

function setFeatureChanges(loaderId, featureName) {
    for (var i = 0; i < features.length; i++) {
        var f = features[i];
        var onWhenRuleValue = f.onWhen.value;
        var offWhenRuleValue = f.offWhen.value;

        if (f.onWhen.rule == 4) {
            // Time based
            onWhenRuleValue = getHourBasedValue(onWhenRuleValue)
        }

        if (f.offWhen.rule == 4) {
            // Time based
            offWhenRuleValue = getHourBasedValue(offWhenRuleValue)
        }

            // TODO: Id of Feature is missing from request
        var tUrl = "/set-feature?";
        var props = [
            "id=" + f.id,
            "name=" + f.name,
            "status=" + f.status,
            "rule=" + f.rule,
            "unit=" + f.unit,
            "onWhenRule=" + f.onWhen.rule,
            "onWhenCondition=" + f.onWhen.condition,
            "onWhenValue=" + onWhenRuleValue,
            "offWhenRule=" + f.offWhen.rule,
            "offWhenCondition=" + f.offWhen.condition,
            "offWhenValue=" + offWhenRuleValue
        ];


        for (var v = 0; v < props.length; v++) {
            var element = props[v];

            if (v == props.length - 1) {
                tUrl += element;
            } else {
                tUrl += element + "&";
            }

        }

        $('#' + featureName + "TableRow").addClass("rowLoader");
        displaySuccess("Updating ...");

        $.ajax({
            url: tUrl, // add all other props
            type: "GET",
            success: function (devices, status, xhr) {
                displaySuccess("Loaded!");
                $('#' + featureName + "TableRow").removeClass("rowLoader");
                run();
            },
            error: function (jqXhr, textStatus, errorMessage) { // error callback 
                displayError(errorMessage);
                $('#' + featureName + "TableRow").removeClass("rowLoader");
            }

        });

        return 1;


    }

    return 0;
}

function setSaveFeature() {
    $('#save_feature_button').removeClass('btn-outline-warning');
    $('#save_feature_button').addClass('btn-warning');
}

function setChanges(loaderId, unitToCall) {
    setSaveFeature();

    for (var i = 0; i < devices.length; i++) {
        var d = devices[i];

        if (d.unit == unitToCall) {
            if (isValid(d)) {
                var onWhenRuleValue = d.onWhen.value;
                var offWhenRuleValue = d.offWhen.value;

                if (d.onWhen.rule == 4) {
                    // Time based
                    onWhenRuleValue = getHourBasedValue(onWhenRuleValue)
                }

                if (d.offWhen.rule == 4) {
                    // Time based
                    offWhenRuleValue = getHourBasedValue(offWhenRuleValue)
                }

                var tUrl = "/control-data?";
                var props = [
                    "status=" + d.status,
                    "rule=" + d.rule,
                    "unit=" + d.unit,
                    "onWhenRule=" + d.onWhen.rule,
                    "onWhenCondition=" + d.onWhen.condition,
                    "onWhenValue=" + onWhenRuleValue,
                    "offWhenRule=" + d.offWhen.rule,
                    "offWhenCondition=" + d.offWhen.condition,
                    "offWhenValue=" + offWhenRuleValue
                ];


                for (var v = 0; v < props.length; v++) {
                    var element = props[v];

                    if (v == props.length - 1) {
                        tUrl += element;
                    } else {
                        tUrl += element + "&";
                    }

                }

                $('#' + unitToCall + "TableRow").addClass("rowLoader");
                displaySuccess("Updating ...");

                $.ajax({
                    url: tUrl, // add all other props
                    type: "GET",
                    success: function (devices, status, xhr) {
                        displaySuccess("Loaded!");
                        $('#' + unitToCall + "TableRow").removeClass("rowLoader");
                    },
                    error: function (jqXhr, textStatus, errorMessage) { // error callback 
                        displayError(errorMessage);
                        $('#' + unitToCall + "TableRow").removeClass("rowLoader");
                    }

                });

                return 1;
            }
        }
    }

    return 0;
}

function addFeatureListeners(newControl) {
    // Switches
    $('#' + newControl.unit + 'Switch').on('change', function () {
        if ($(this).is(':checked')) {
            newControl.status = 1;
        } else {
            newControl.status = 0;
        }

        setFeatureChanges('#' + newControl.unit + 'Switch', newControl.unit);     
    })
}



function addListeners(newControl) {
    // Switches
    $('#' + newControl.unit + 'Switch').on('change', function () {
        if ($(this).is(':checked')) {
            newControl.status = 1;
        } else {
            newControl.status = 0;
        }

        setChanges('#' + newControl.unit + 'Switch', newControl.unit);
    })

    // Rules
    $('#' + newControl.unit + 'RuleSwitch').on('change', function () {
        if ($(this).is(':checked')) {
            newControl.rule = 1;

            $('#' + newControl.unit + 'OnWhenRuleSelect').show();
            $('#' + newControl.unit + 'OnWhenRuleCondition').show();
            $('#' + newControl.unit + 'OnWhenConditionValue').show();
            $('#' + newControl.unit + 'OffWhenRuleSelect').show();
            $('#' + newControl.unit + 'OffWhenRuleCondition').show();
            $('#' + newControl.unit + 'OffWhenConditionValue').show();
        } else {
            newControl.rule = 0;
            $('#' + newControl.unit + 'OnWhenRuleSelect').hide();
            $('#' + newControl.unit + 'OnWhenRuleCondition').hide();
            $('#' + newControl.unit + 'OnWhenConditionValue').hide();
            $('#' + newControl.unit + 'OffWhenRuleSelect').hide();
            $('#' + newControl.unit + 'OffWhenRuleCondition').hide();
            $('#' + newControl.unit + 'OffWhenConditionValue').hide();
        }

        setChanges('#' + newControl.unit + 'Switch', newControl.unit);
        
    })

    // On Rule Selects
    $('#' + newControl.unit + 'OnWhenRuleSelect').on('change', function () {
        var onWhenRuleSelectValue = $('#' + newControl.unit + 'OnWhenRuleSelect').val();
        newControl.onWhen.rule = onWhenRuleSelectValue;
        if (onWhenRuleSelectValue == 4) {
            $('.' + newControl.unit + 'OnWhenRuleCondition').hide();
            $('#' + newControl.unit + 'OnWhenConditionValue').attr('type', 'text');
            $('#' + newControl.unit + 'OnWhenConditionValue').val('HH:MM');
        } else {
            $('.' + newControl.unit + 'OnWhenRuleCondition').show();
            $('#' + newControl.unit + 'OnWhenConditionValue').attr('type', 'number');
        }
        setChanges('#' + newControl.unit + 'OnWhenRuleSelect', newControl.unit);
    })

    // On Rule Condition
    $('#' + newControl.unit + 'OnWhenRuleCondition').on('change', function () {
        newControl.onWhen.condition = $('#' + newControl.unit + 'OnWhenRuleCondition').val();
        setChanges('#' + newControl.unit + 'OnWhenRuleCondition', newControl.unit);
    })

    // On Rule Value
    $('#' + newControl.unit + 'OnWhenConditionValue').on('change', function () {
        newControl.onWhen.value = $('#' + newControl.unit + 'OnWhenConditionValue').val();
        setChanges('#' + newControl.unit + 'OnWhenConditionValue', newControl.unit);
    })

    // Off Rule Selects
    $('#' + newControl.unit + 'OffWhenRuleSelect').on('change', function () {
        var offWhenRuleSelectValue = $('#' + newControl.unit + 'OffWhenRuleSelect').val();
        newControl.offWhen.rule = offWhenRuleSelectValue
        if (offWhenRuleSelectValue == 4) {
            // change control
            $('.' + newControl.unit + 'OffWhenRuleCondition').hide();
            $('#' + newControl.unit + 'OffWhenConditionValue').attr('type', 'text');
            $('#' + newControl.unit + 'OffWhenConditionValue').val('HH:MM');
        } else {
            // change control
            $('.' + newControl.unit + 'OffWhenRuleCondition').show();
            $('#' + newControl.unit + 'OffWhenConditionValue').attr('type', 'number');
        }
        setChanges('#' + newControl.unit + 'OffWhenRuleSelect', newControl.unit);
    })

    // Off Rule Condition
    $('#' + newControl.unit + 'OffWhenRuleCondition').on('change', function () {
        newControl.offWhen.condition = $('#' + newControl.unit + 'OffWhenRuleCondition').val();
        setChanges('#' + newControl.unit + 'OffWhenRuleCondition', newControl.unit);
    })

    // Off Rule Value
    $('#' + newControl.unit + 'OffWhenConditionValue').on('change', function () {
        newControl.offWhen.value = $('#' + newControl.unit + 'OffWhenConditionValue').val();
        setChanges('#' + newControl.unit + 'OffWhenConditionValue', newControl.unit);
    })
}

function mapResponseToSensorsDOM(data) {
    $('#temperatureSensorData').text(data.sensors.temperature);
    $('#humiditySensorData').text(data.sensors.humidity);
    $('#waterTankSensorData').text(data.sensors.waterTankLevel);
}

function parseHourValue(original) {
    var hours = Math.floor(original / 3600)
    var minutes = Math.floor((original % 3600) / 60)

    return hours.toString().padStart(2, '0') + ":" + minutes.toString().padStart(2, '0');
}

function mapResponseToFeaturesDOM(data) {

    var table = $('#featuresTBody');

    table.empty();
    global_features = [];
    // MAP
    var total = data.features.length;
    features = data.features;

    for (var i = 0; i < total; i++) {
        var feature = features[i];
        features[i].unit = feature.id + feature_id;

        global_features.push(
            {
                name: feature.name,
                id: feature.id
            });

        // CREATE UI ELEMENT
        var unit = features[i].unit;
        var template = '<tr id="' + unit + 'TableRow">'
        template += '<td>'
        template += '    <div class="custom-control custom-switch">'
        template += '        <input type="checkbox" class="custom-control-input" id="' + unit + 'Switch">'
        template += '    </div>'
        template += '</td>'
        template += '<td>' + feature.name + '</td>'

        template += '</tr>';

        table.append(template);

        // MAP VALUES TO JS FEATURES AND NEW UI
        $('#' + unit + "Switch").prop('checked', feature.status);
        features[i].status = feature.status;

        // ADD Listeners
        addFeatureListeners(feature);
    }
}

function mapResponseToDOM(data) {
    $('#temperatureSensorData').text(data.sensors.temperature);
    $('#humiditySensorData').text(data.sensors.humidity);
    $('#waterTankSensorData').text(data.sensors.waterTankLevel);


    for (var j = 0; j < devices.length; j++) {
        var private_device = devices[j];


        for (var i = 0; i < data.devices.length; i++) {
            var device = data.devices[i];

            if (private_device.unit == device.unit) {
                for (var j = 0; j < units.length; j++) {
                    var unit = units[j];

                    if (unit == device.unit) {
                        $('#' + device.unit + "Switch").prop('checked', device.status);
                        devices[j].status = device.status;

                        $('#' + device.unit + "RuleSwitch").prop('checked', device.rule);
                        devices[j].rule = device.rule;
                        if (devices[j].rule == 1) {
                            $('#' + device.unit + 'OnWhenRuleSelect').show();
                            $('#' + device.unit + 'OnWhenRuleCondition').show();
                            if (device.onWhen.rule == 4) {
                                $('#' + device.unit + 'OffWhenConditionValue').attr('type', 'text');
                                $('#' + device.unit + 'OffWhenConditionValue').val('HH:MM');
                                $('.' + device.unit + 'OffWhenRuleCondition').hide();
                                $('.' + device.unit + 'OnWhenRuleCondition').hide();
                                $('#' + device.unit + 'OnWhenConditionValue').attr('type', 'text');
                                $('#' + device.unit + 'OnWhenConditionValue').val('HH:MM');

                            } else {
                                $('#' + device.unit + 'OnWhenConditionValue').attr('type', 'number');
                                $('#' + device.unit + 'OffWhenConditionValue').attr('type', 'number');
                                $('.' + device.unit + 'OffWhenRuleCondition').show();
                                $('#' + device.unit + 'OffWhenConditionValue').show();
                                $('#' + device.unit + 'OnWhenConditionValue').show();
                            }

                        } else {
                            $('#' + device.unit + 'OnWhenRuleSelect').hide();
                            $('#' + device.unit + 'OnWhenRuleCondition').hide();
                            $('#' + device.unit + 'OnWhenConditionValue').hide();
                            $('#' + device.unit + 'OffWhenRuleSelect').hide();
                            $('#' + device.unit + 'OffWhenRuleCondition').hide();
                            $('#' + device.unit + 'OffWhenConditionValue').hide();
                        }

                        $('#' + device.unit + "OnWhenRuleSelect").val(device.onWhen.rule);
                        devices[j].onWhen.rule = device.onWhen.rule;

                        $('#' + device.unit + "OnWhenRuleCondition").val(device.onWhen.condition);
                        devices[j].onWhen.condition = device.onWhen.condition;

                        if (device.onWhen.rule == 4) {
                            // Time based
                            var onWhenValueParsed = parseHourValue(device.onWhen.value);
                            $('#' + device.unit + "OnWhenConditionValue").val(onWhenValueParsed);
                            devices[j].onWhen.value = onWhenValueParsed;
                        } else {
                            $('#' + device.unit + "OnWhenConditionValue").val(device.onWhen.value);
                            devices[j].onWhen.value = device.onWhen.value;
                        }

                        if (device.offWhen.rule == 4) {
                            var offWhenValueParsed = parseHourValue(device.offWhen.value);
                            $('#' + device.unit + "OffWhenConditionValue").val(offWhenValueParsed);
                            devices[j].offWhen.value = offWhenValueParsed;
                        } else {
                            $('#' + device.unit + "OffWhenConditionValue").val(device.offWhen.value);
                            devices[j].offWhen.value = device.offWhen.value;
                        }

                        $('#' + device.unit + "OffWhenRuleSelect").val(device.offWhen.rule);
                        devices[j].offWhen.rule = device.offWhen.rule;

                        $('#' + device.unit + "OffWhenRuleCondition").val(device.offWhen.condition);
                        devices[j].offWhen.condition = device.offWhen.condition;



                        break;
                    }
                }
            }

        }
    }
}


function refreshSensorsPage() {

    $.ajax({
        url: "/get-control-data",
        type: "GET",
        success: function (data, status, xhr) {
            mapResponseToSensorsDOM(data);
            displaySuccess("Loaded");
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage);
        }
    });
}


function refreshFeaturesSection() {

    $.ajax({
        url: "/get-features",
        type: "GET",
        success: function (data, status, xhr) {
            mapResponseToFeaturesDOM(data);
            displaySuccess("Loaded");
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage);
        }
    });
}

function loadFeaturesClicked() {
    var fd = new FormData();
    var files = $('#load-file')[0].files[0];
    fd.append('file', files);

    $.ajax({
        url: '/load-features',
        type: 'POST',
        data: fd,
        contentType: false,
        processData: false,
        success: function (response) {
            if (response != 0) {
                displaySuccess("Loaded");
                location.reload();
            }
            else {
                displayError(response);
            }
        },
    });
}

function pageListeners() {
    $('#load-file').on('change', function () {
        var files = $('#load-file')[0].files[0];
        $('#load-file-path').text(files.name);
    })
}

function downloadFeatures() {
    window.location = '/download-features'
}

function saveAsFeature() {
    var name = prompt("Please enter Feature Name", "Feature #1");

    if (name == null) {
        displayError("Please enter a feature name!");

        return;
    }

    $.ajax({
        url: '/save-as-feature?name=' + name,
        type: 'GET',
        success: function (response) {
            if (response != 0) {
                displaySuccess("Loaded");
                location.reload();
            }
            else {
                displayError(response);
            }
        },
    });


    return 0;
}

$(function () {
    hideMSG();
    pageListeners();

    for (var i = 0; i < devices.length; i++) {
        addListeners(devices[i]);
    }

    // Errors
    var errorDisplay = $("#errorDisplay");

    // Call Endpoint
    // GET
    run();

    refreshFeaturesSection();

    setInterval('refreshSensorsPage()', 10000);
})