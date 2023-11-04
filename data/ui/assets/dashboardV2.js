var _messageDivId = 'dashboardMSGDiv';
var mainChartDisplay = document.getElementById("myAreaChart");
var _units_to_display = [
    { name: 'fanExhaus', labels: [], data: [], color: "rgba(255,247,0", from: '2023-09-27', to: '2023-09-27' },
    { name: 'inputPump', labels: [], data: [], color: "rgba(0,171,225", from: '2023-09-27', to: '2023-09-27' },
    { name: 'outputPump', labels: [], data: [], color: "rgba(247,137,40", from: '2023-09-27', to: '2023-09-27' },
    { name: 'fanIntake', labels: [], data: [], color: "rgba(112,107,250", from: '2023-09-27', to: '2023-09-27' },
    { name: 'lightSource', labels: [], data: [], color: "rgba(250,107,231", from: '2023-09-27', to: '2023-09-27' },
    { name: 'oxygenPump', labels: [], data: [], color: "rgba(182,168,180", from: '2023-09-27', to: '2023-09-27' }
];

function hideMessages() {
    $('#' + _messageDivId + '').hide();
}

function displayError(msg) {
    $('#' + _messageDivId + '').show();
    $('#' + _messageDivId + '').html('<p class="alert alert-danger" role="alert" id="errorDisplay">' + msg + '</p>');
}

function displaySuccess(msg) {
    $('#' + _messageDivId + '').show();
    $('#' + _messageDivId + '').html('<p class="alert alert-primary" id="errorDisplay">' + msg + '</p>');
}

function createId(id) { return id + "_control_filter" };
function getDateTimeFormat() {

    var currentDate = new Date();

    // Format the date and time into the required string format (YYYY-MM-DDTHH:mm)
    var year = currentDate.getFullYear();
    var month = String(currentDate.getMonth() + 1).padStart(2, '0'); // Months are 0-based, so add 1
    var day = String(currentDate.getDate()).padStart(2, '0');
    var hours = String(currentDate.getHours()).padStart(2, '0');
    var minutes = String(currentDate.getMinutes()).padStart(2, '0');

    return year + '-' + month + '-' + day + 'T' + hours + ':' + minutes;
}


function createFiltersArea(elements) {

    var currentDate = getDateTimeFormat();

    elements.forEach(function (e) {
        e.from = currentDate;
        e.to = currentDate;
    })


    var date_from = $('#dateFROM');
    var date_to = $('#dateTO');

    date_from.val(currentDate);
    date_to.val(currentDate);

    date_from.on('change', function () {
        // Get the selected date value
        var selectedDate = $(this).val();

        // Display the selected date in a specific format (you can adjust the format)
        var formattedDate = new Date(selectedDate);

        var year = formattedDate.getFullYear();
        var month = String(formattedDate.getMonth()).padStart(2, '0'); // Months are 0-based, so add 1
        var day = String(formattedDate.getDate()).padStart(2, '0');
        var hours = String(formattedDate.getHours()).padStart(2, '0');
        var minutes = String(formattedDate.getMinutes()).padStart(2, '0');

        _units_to_display.forEach(function (e) {

            e.from = year + '-' + month + '-' + day + 'T' + hours + ':' + minutes;
        })

        redraw();
    });

    date_to.on('change', function () {
        // Get the selected date value
        var selectedDate = $(this).val();

        // Display the selected date in a specific format (you can adjust the format)
        var formattedDate = new Date(selectedDate);
        var formattedDate = new Date(selectedDate);

        var year = formattedDate.getFullYear();
        var month = String(formattedDate.getMonth()).padStart(2, '0'); // Months are 0-based, so add 1
        var day = String(formattedDate.getDate()).padStart(2, '0');
        var hours = String(formattedDate.getHours()).padStart(2, '0');
        var minutes = String(formattedDate.getMinutes()).padStart(2, '0');

        _units_to_display.forEach(function (e) {
            e.to = year + '-' + month + '-' + day + 'T' + hours + ':' + minutes;
        })

        redraw();
    });

}

async function redraw() {
    const promises = _units_to_display.map(async function (u) {
        return initiateData(u);
    });

    try {
        await Promise.all(promises);
        // All initiateData calls are done, you can perform your action here
        console.log("All data retrieval is complete.");

        drawUnit();
    } catch (error) {
        console.error("Error occurred during data retrieval: " + error);
    }
}

function drawUnit() {
    var myAreaChart = new Chart(mainChartDisplay, {
        type: 'line',
        data: {
            labels: _units_to_display[0].labels.sort(),
            datasets: _units_to_display.map(function (unit) {

                return {
                    type: 'line',
                    label: unit.name,
                    labels: _units_to_display[0].labels.sort(),
                    data: unit.data.map(function (d) {
                        return d.status
                    }),
                    borderColor: unit.color + ', 132)',
                    backgroundColor: unit.color + ', 0.2)',
                };
            }),
        },
        options: {
            scales: {
                xAxes: [{
                    time: {
                        unit: 'date'
                    },
                    gridLines: {
                        display: false
                    },
                    ticks: {
                        maxTicksLimit: 7
                    }
                }],
                yAxes: [{
                    ticks: {
                        min: 0,
                        max: 1,
                        maxTicksLimit: 5
                    },
                    gridLines: {
                        color: "rgba(0, 0, 0, .125)",
                    }
                }],
            },
            legend: {
                display: true
            }
        }
    });

    myAreaChart.update();
}

function processData(data) {
    var dataLines = data.split("\n");
    for (var lineIndexInDataRow = 0; lineIndexInDataRow < dataLines.length; lineIndexInDataRow++) {
        var splitLineOfData = dataLines[lineIndexInDataRow].split("/");
        if (splitLineOfData && splitLineOfData.length > 0 && splitLineOfData[0].length > 0) {          
            // Date
            var parsedDataFromLine = splitLineOfData[0] + ":" + splitLineOfData[1] + ":" + splitLineOfData[2];
            // Time
            var parsedTimeFromLine = splitLineOfData[3] + ":" + splitLineOfData[4] + ":" + splitLineOfData[5]; // Extracted date
            var dataObject = JSON.parse(splitLineOfData[6].replace('\r', ''));
            // The Controls in this line of data
            var controls = dataObject.devices;
            // For each control in data line
            for (var controlInDataLine = 0; controlInDataLine < controls.length; controlInDataLine++) {
                var control = controls[controlInDataLine];
                var done = false;
                for (var k = 0; k < _units_to_display.length; k++) {
                    var selected_unit = _units_to_display[k];

                    if (selected_unit.name == control.unit) {

                        _units_to_display[k].labels.push(parsedTimeFromLine);
                        _units_to_display[k].data.push(control);// data = [ [] ,[], ]

                        // done = true;
                        // break;
                    }
                }

                // if (done) {
                //     break;
                // }
            }
        }

    }
}

// function initiateData(e) {

//     if (e.show) {
//         var from_date_search = new Date(e.from);
//         var to_date_search = new Date(e.to);

//         // Full compare makes things work fine
//         while (from_date_search <= to_date_search) {
//             $.ajax({
//                 url: "http://192.168.100.38/get-history?name=" + e.name + "&d=" + from_date_search.getDate() + "&m=" + (from_date_search.getMonth() + 1) + "&y=" + from_date_search.getFullYear(), // TODO: Parse datetime
//                 type: "GET",
//                 success: function (data, status, xhr) {
//                     processData(data);
//                 },
//                 error: function (jqXhr, textStatus, errorMessage) { // error callback
//                     displayError(errorMessage);
//                 }
//             });

//             // Get the current year and month
//             var currentYear = from_date_search.getFullYear();
//             var currentMonth = from_date_search.getMonth();

//             // Increment the day
//             from_date_search.setDate(from_date_search.getDate() + 1);

//             if (from_date_search.getFullYear() !== currentYear || from_date_search.getMonth() !== currentMonth) {
//                 break; // Exit the loop if the month or year changes
//             }
//         }
//     }




// }

function getHistory(name, day, month, year) {

    return new Promise(function (resolve, reject) {
        $.ajax({
            url: "http://192.168.100.38/get-history?name=" + name + "&d=" + day + "&m=" + month + "&y=" + year,
            type: "GET",
            success: function (data, status, xhr) {
                resolve(data);
            },
            error: function (jqXhr, textStatus, errorMessage) {
                reject(errorMessage);
            }
        });
    });
}

function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function initiateData(e) {
    var from_date_search = new Date(e.from);
    var to_date_search = new Date(e.to);

    while (from_date_search <= to_date_search) {
        try {

            const data = getHistory(e.name, from_date_search.getDate(), from_date_search.getMonth() + 1, from_date_search.getFullYear());

            processData(data);
        } catch (error) {
            displayError(error);
        }

        var currentYear = from_date_search.getFullYear();
        var currentMonth = from_date_search.getMonth();

        from_date_search.setDate(from_date_search.getDate() + 1);

        if (from_date_search.getFullYear() !== currentYear || from_date_search.getMonth() !== currentMonth) {
            break;
        }

        // Introduce a 1-second delay before the next iteration

    }
}


$(function () {
    // Set new default font family and font color to mimic Bootstrap's default styling
    Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
    Chart.defaults.global.defaultFontColor = '#292b2c';

    createFiltersArea(_units_to_display);

    redraw();
})

