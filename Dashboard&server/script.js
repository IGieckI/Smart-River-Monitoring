const socket = new WebSocket('ws://localhost:8765');

socket.addEventListener('close',(event) => {
    location.reload();
    
    
});

// Handle incoming messages from the server
socket.addEventListener('message', (event) => {
    const data = JSON.parse(event.data);
    document.querySelector('#system-status').innerText=data['current_status'];
    document.querySelector('#valve-opening').innerText=data['current_valve_opening'];
    if(data['hardware_manual']){
        document.querySelector('#valve-control').innerText='Hardware Manual';
    }else if(data['dashboard_manual']){
        document.querySelector('#valve-control').innerText='Dashboard Manual';
    }else{
        document.querySelector('#valve-control').innerText='Automatic Mode';
    }
    let manual_old = document.querySelector('#manual-switch').checked;
    document.querySelector('#manual-switch').checked=data['dashboard_manual'];
    document.querySelector('#valve-input').value = data['dashboard_open_value'];
    document.querySelector('#water_level').innerText = data['history'][data['history'].length -1]['value'];
    
    if(manual_old != data['dashboard_manual']){
        enableControl();
    }
        
    
    const formatDatetime = (datetimeStr) => {
        const date = new Date(datetimeStr);
        return date.toLocaleDateString('en-US', { year: 'numeric', month: '2-digit', day: '2-digit' }) + ' ' + date.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit' });
    }
    
    const datetimeData = data['history'].map(item => formatDatetime(item.datetime));
    const valueData = data['history'].map(item => item.value);

    const canvas = document.getElementById('graph-container');

    if (window.myChart instanceof Chart) {
        // If yes, destroy the existing chart
        window.myChart.destroy();
    }

    // Creating the chart
    const ctx = document.getElementById('graph-container').getContext('2d');
         
    window.myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: datetimeData,
                datasets: [{
                    label: 'Value',
                    data: valueData,
                    backgroundColor: 'rgba(255, 99, 132, 0.2)',
                    borderColor: 'rgba(255, 99, 132, 1)',
                    borderWidth: 1
                }]
            },
            options: {
                scales: {
                    y: {
                        ticks: {
                            beginAtZero: true
                        },
                        beginAtZero: true
                    }
                },
                animation: false
            }
        });
    
    // Update your graph or other UI elements with the received data
    console.log('Received data:', data);
});

// Function to send data to the server (e.g., when the user updates settings)
function updateValveSettings() {
    const valveOpening = document.getElementById('valve-input').value;
    const manualMode = document.getElementById('manual-switch').checked;
    const data = {
        valve: valveOpening,
        remote_control: manualMode
    };

    // Send the data to the server
    socket.send(JSON.stringify(data));
    enableControl();
    
}

function enableControl(){
    //console.log(document.querySelector('#manual-switch').checked);
    if(document.querySelector('#manual-switch').checked){
        document.getElementById("valve-input").disabled = false;
    }else{
        document.getElementById("valve-input").disabled = true;
    }
}