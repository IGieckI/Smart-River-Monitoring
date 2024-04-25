<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="styles.css">
        <title>Water Level Monitor</title>
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

    </head>
    <body>
        <canvas width="400" height="400" id="graph-container">
            
        </canvas>
        
        <div>
            <p>Current water level: <span id="water_level">null</span></p>
            <p>System status: <span id="system-status">null</span></p>
            <p>Valve opening: <span id="valve-opening">null</span></p>
            <p>Valve control: <span id="valve-control">null</span></p>
        </div>

        <div>
            <label for="manual-switch">Manual Mode:</label>
            <input type="checkbox" onchange="updateValveSettings()" id="manual-switch">

            <label for="valve-input">Valve Opening:</label>
            <input type="range" onchange="updateValveSettings()" id="valve-input" min="0" max="100" disabled>
        </div>
        
        <script src="script.js"></script>
    </body>
</html>
