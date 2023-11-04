function displayError(msg, type) {
    $('#' + type + 'MessageDiv').show();
    $('#' + type + 'MessageDiv').html('<p class="alert alert-danger" role="alert" id="errorDisplay">' + msg + '</p>');
}

function displaySuccess(msg, type) {
    $('#' + type + 'MessageDiv').show();
    $('#' + type + 'MessageDiv').html('<p class="alert alert-primary" id="errorDisplay">' + msg + '</p>');
}

function hideMSG(type) {
    $('#' + type + 'MessageDiv').hide();
}

function mapUI(data, type){
    $('#' + data.name + 'MinLimit').val(data.min);
    $('#' + data.name + 'MaxLimit').val(data.max);
}

function refresh(type){
    $.ajax({
        url: "/get-limits?name=" + type,
        type: "GET",
        success: function (data, status, xhr) {
            mapUI(data);
            displaySuccess("Loaded", type);
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage, type);
        }
    });
}

function update(type, min, max) {
    $.ajax({ 
        url: "/set-limit?name=" + type + "&min=" + min + "&max=" + max,
        type: "GET",
        dataType: "json",
        success: function (data, status, xhr) {
            // TODO: Fix return value is not mapped correctly!
            mapUI(data.limits);
            displaySuccess("Loaded", type);
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage, type);
        }
    });
}

function initSensorLimitHandler(type){
    var minLimit = $('#' + type + 'MinLimit');
    var maxLimit = $('#' + type + 'MaxLimit');

    minLimit.on('change', function () {
        update(type, minLimit.val(), maxLimit.val());
    });

    maxLimit.on('change', function () {
        update(type, minLimit.val(), maxLimit.val());
    });

    refresh(type);
}

