

function displayError(msg, type) {
    $('#' + type + 'MessageDiv').show();
    $('#' + type + 'MessageDiv').html('<p class="alert alert-danger" role="alert" id="errorDisplay">' + msg + '</p>');
}

function displaySuccess(msg, type) {
    $('#' + type + 'MessageDiv').show();
    $('#' + type + 'MessageDiv').html('<p class="alert alert-primary" id="errorDisplay">' + msg + '</p>');
}

function mapUI(data, type){
    $('#' + type + "NotificationSwitch").prop('checked', data.status == 1 ? true : false );
     $('#' + type + 'API_KEY').val(data.api_key);
     $('#' + type + 'ID').val(data.id);
}

function saveNotification(type){
    var telegramNotificationSwitch = $('#' + type + 'NotificationSwitch').is(':checked') == false ? 0 : 1;
    var apiKey = $('#' + type + 'API_KEY').val();
    var id = $('#' + type + 'ID').val();

    if (apiKey == undefined || apiKey == null || apiKey == "" || id == undefined || id == null || id == ""){
        
        displayError("Please fill data inputs!", type);

        return;
    }else{
        displaySuccess("Valid", type);
    }

    $.ajax({
        url: "/set-notification?name=" + type + "&apikey=" + apiKey + "&id=" + id + "&status=" + telegramNotificationSwitch,
        type: "GET",
        success: function (data, status, xhr) {
            // TODO: Fix return value is not mapped correctly!
            mapUI(data.config, type);
            displaySuccess("Loaded", type);
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage, type);
        }
    });
}

function loadNotification(type) {
    $.ajax({
        url: "/get-notification?name=" + type,
        type: "GET",
        success: function (data, status, xhr) {
            mapUI(data.config, type);
            displaySuccess("Loaded", type);
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            displayError(errorMessage, type);
        }
    });

    // Switches
    $('#' + type + 'NotificationSwitch').on('change', function () {
        saveNotification(type);
    })

    // On Value
    $('#' + type + 'API_KEY').on('change', function () {
        saveNotification(type);
    })

    $('#' + type + 'ID').on('change', function () {
        saveNotification(type);
    })
}