function errorMessageBox(msg) {
    $('.messageDiv').show();
    $('.messageDiv').html('<p class="alert alert-danger" role="alert" id="errorDisplay">' + msg + '</p>');
}

function successMessageBox(msg) {
    $('.messageDiv').show();
    $('.messageDiv').html('<p class="alert alert-primary" id="errorDisplay">' + msg + '</p>');
}

function loadingMessageBox(msg) {
    $('.messageDiv').show();
    $('.messageDiv').html('<p class="alert alert-warning" id="errorDisplay">' + msg + '</p>');
}

function buttonGetCall(url) {
    $.ajax({
        url: url,
        type: "GET",
        success: function (data, status, xhr) {
            successMessageBox("Updated");
        },
        error: function (jqXhr, textStatus, errorMessage) { // error callback 
            errorMessageBox(errorMessage);
        }
    });
}
