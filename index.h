#define INDEX_H
const char* index_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Control Dashboard</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <script src="https://static.elfsight.com/platform/platform.js" async></script>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f4;
      margin: 0;
      padding: 0;
    }
    header {
      background: rgb(25, 165, 49);
      color: #fff;
      padding: 10px;
      text-align: center;
      font-size: 24px;
      position: relative;
    }
    #nodemcu-status {
      position: absolute;
      left: 10px;
      top: 10px;
      font-size: 18px;
      background: #fff;
      color: rgb(0, 78, 0);
      padding: 5px 10px;
      border-radius: 5px;
    }
    #nodemcu-status.offline {
      background: #e74c3c;
      color: #fff;
    }
    #date-time {
      position: absolute;
      right: 10px;
      top: 15px;
      font-size: 16px;
    }
    .container {
      padding: 20px;
      background-color: rgb(77, 73, 73);
    }
    .card {
      background: #fff;
      margin: 20px 0;
      padding: 15px;
      border-radius: 5px;
      box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    }
    .gauge-container {
      display: flex;
      justify-content: space-around;
      flex-wrap: wrap;
    }
    .gauge {
      text-align: center;
      width: 200px;
      margin: 15px;
    }
    meter {
      width: 100%;
      height: 50px;  
    }
    #rain-status {
      font-size: 24px;
      margin-top: 20px;
      text-align: center;
    }
    #weather-widget {
      display: flex;
      flex-direction: column;
      align-items: center;
      text-align: center;
      font-size: 18px;
      margin-top: 15px;
    }
    button {
      padding: 10px 15px;
      margin: 5px;
      border: none;
      background: rgb(24, 173, 49);
      color: #fff;
      border-radius: 5px;
      cursor: pointer;
    }
    button:hover {
      background: rgba(48, 179, 202, 0.7);
    }
    canvas {
      max-width: 100%;
    }
    @media (max-width: 768px) {
      .gauge-container {
        flex-direction: column;
        align-items: center;
      }
      meter {
        height: 15px;
      }
    }
    .weather-card {
      width: 300px;
      background: rgba(48, 179, 202, 0.7);
      border-radius: 10px;
      overflow: hidden;
      box-shadow: 0 4px 8px rgba(133, 128, 128, 0.5);
    }
    .current-weather {
      text-align: center;
      padding: 20px;
      background: url('https://via.placeholder.com/300x150') no-repeat center/cover;
    }
    .current-weather h1 {
      font-size: 3rem;
      margin: 0;
    }
    .current-weather p {
      margin: 5px 0;
      font-size: 1.2rem;
    }
    .forecast {
      padding: 15px;
      background-color: #222;
    }
    .forecast-day {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 10px 0;
      border-bottom: 1px solid #444;
    }
    .forecast-day:last-child {
      border-bottom: none;
    }
    .day {
      font-weight: bold;
      color: #fcf0f0;
    }
    .temp-range {
      font-size: 1.0rem;
      color: #aaa;
    }
    .weather-icon {
      width: 30px;
      height: 30px;
    }
    /* Styling for webkit browsers */
    #humidity-gauge::-webkit-meter-bar {
      background: #e0e0e0;
      border-radius: 10px;
    }
    #humidity-gauge::-webkit-meter-optimum-value {
      background: rgb(63, 148, 182);
    }
    #temperature-gauge::-webkit-meter-bar {
      background: #e0e0e0;
      border-radius: 10px;
    }
    #temperature-gauge::-webkit-meter-optimum-value {
      background: rgb(223, 155, 29);
    }
    #moisture-gauge::-webkit-meter-bar {
      background: #e0e0e0;
      border-radius: 10px;
    }
    #moisture-gauge::-webkit-meter-optimum-value {
      background: rgb(30, 192, 165);
    }
    /* Style for the status container */
.status {
    font-family: Arial, sans-serif;
    font-size: 18px;
    font-weight: bold;
    margin: 10px auto;
    padding: 10px;
    border: 2px solid #19858c; /* Match the custom background color */
    border-radius: 8px;
    background-color: #f4f9f9; /* Light background for contrast */
    color: #333; /* Default text color */
    width: fit-content; /* Auto-adjust width based on content */
    text-align: center;
}
/* Style for the pump status text */
.status #pumpStatus {
    font-weight: bold;
    font-size: 20px;
    margin-left: 5px; /* Space between label and status */
    transition: color 0.3s ease-in-out; /* Smooth transition for color changes */
}
/* Specific status colors */
.status #pumpStatus.green {
    color: green;
}
.status #pumpStatus.red {
    color: red;
}
.status #pumpStatus.gray {
    color: gray;
}  
</style>
</head>
<body>
  <header>
    Control Dashboard
    <div id="nodemcu-status" class="offline">Checking...</div>
    <div id="date-time"></div>
  </header>
  <div class="container">
    <!-- Sensor Gauges -->
    <div class="card">
      <h3 align="center">Sensor Data</h3>
      <div class="gauge-container">
        <div class="gauge">
          <h4>Humidity</h4>
          <meter id="humidity-gauge" min="0" max="100" value="0"></meter>
          <p><span id="humidity-value">0</span>%</p>
        </div>
        <div class="gauge">
          <h4>Temperature</h4>
          <meter id="temperature-gauge" min="0" max="100" value="0"></meter>
          <p><span id="temperature-value">0</span>&deg;C</p>
        </div>
        <div class="gauge">
          <h4>Soil Moisture</h4>
          <meter id="moisture-gauge" min="0" max="100" value="0"></meter>
          <p><span id="moisture-value">0</span>%</p>
        </div>
      </div>
      <div class="status" align="center">
    Pump Status: <span id="pumpStatus">Loading...</span>
  </div>
      <div id="rain-status">Rain Status: <span id="rain-icon">☀️ Sunny</span></div>
    </div>
    <div class="card" align="center">
      <h3> Fertiliser Valve Controls</h3>
      <button id="valve1-btn" class="valve-btn" onclick="toggleValve(1)">NITROGEN ON</button>
      <button id="valve2-btn" class="valve-btn" onclick="toggleValve(2)">POTASSIUM ON</button>
      <button id="valve3-btn" class="valve-btn" onclick="toggleValve(3)">PHOSPHORUS ON</button>
    </div>
    <!-- Weather Widget -->
    <div class="card" align="center">
      <h3>Daily Weather</h3>
      <div class="weather-card">
        <div class="current-weather">
          <h1 id="temp">Loading...</h1>
          <p id="location">Loading...</p>
          <p id="description">Loading...</p>
          <p id="details">Loading...</p>
        </div>
        <div class="forecast">
          <div class="forecast-day">
            <span class="day">Today</span>
            <img class="weather-icon" id="today-icon" src="" alt="weather">
            <span class="temp-range" id="today-temp">Loading...</span>
          </div>
          <div class="forecast-day">
            <span class="day">Tomorrow</span>
            <img class="weather-icon" id="tomorrow-icon" src="" alt="weather">
            <span class="temp-range" id="tomorrow-temp">Loading...</span>
          </div>
          <div class="forecast-day">
            <span class="day">Friday</span>
            <img class="weather-icon" id="friday-icon" src="" alt="weather">
            <span class="temp-range" id="friday-temp">Loading...</span>
          </div>
        </div>
      </div>
    </div>
    <!-- Analysis Chart -->
    <div class="card">
      <h3 align="center">Analysis Report</h3>
      <canvas id="analysis-chart"></canvas>
    </div>
</div>
<script>
  // Update the date and time dynamically
  setInterval(() => {
    const now = new Date();
    document.getElementById('date-time').textContent = now.toLocaleString();
  }, 1000);
  // NodeMCU status check
function checkNodeMCUStatus() {
  fetch('/readADC')
    .then(() => {
      const statusElement = document.getElementById('nodemcu-status');
      statusElement.textContent = "Status: Online";
      statusElement.classList.remove("offline");
    })
    .catch(() => {
      const statusElement = document.getElementById('nodemcu-status');
      statusElement.textContent = "Status: Offline";
      statusElement.classList.add("offline");
    });
}
// Call the function once
checkNodeMCUStatus();
  // Update sensor data dynamically
  setInterval(() => {
    fetch('/readADC')
      .then(response => response.json())
      .then(data => {
        // Update gauges
        document.getElementById("humidity-value").textContent = data.Humidity;
        document.getElementById("temperature-value").textContent = data.Temperature;
        document.getElementById("moisture-value").textContent = Math.round((data.SoilMoisture / 1023) * 100);
        document.getElementById("humidity-gauge").value = data.Humidity;
        document.getElementById("temperature-gauge").value = data.Temperature;
        document.getElementById("moisture-gauge").value = Math.round((data.SoilMoisture / 1023) * 100);
        document.getElementById('rain-icon').textContent = data.Rain === '0' ? '🌧️ Rainy' : '☀️ Sunny';
        // Fix pump status
            const pumpStatus = data.PumpStatus; // Make sure the server sends this value
            const pumpStatusElement = document.getElementById("pumpStatus");
            if (pumpStatus == 1) { 
                pumpStatusElement.textContent = "ON";
                pumpStatusElement.style.color = "green";
            } else {
                pumpStatusElement.textContent = "OFF";
                pumpStatusElement.style.color = "red";
            }
            // Assume minSoilMoisture and maxSoilMoisture are the minimum and maximum possible raw values for soil moisture
const minSoilMoisture = 0; // Adjust this to the actual minimum value
const maxSoilMoisture = 1023; // Adjust this to the actual maximum value, e.g., for a 10-bit ADC
// Convert soil moisture to percentage
      const soilMoisturePercentage = ((data.SoilMoisture - minSoilMoisture) / (maxSoilMoisture - minSoilMoisture)) * 100;
        // Update analysis chart
        analysisChart.data.datasets[0].data.push(data.Humidity);
        analysisChart.data.datasets[1].data.push(data.Temperature);
        //analysisChart.data.datasets[2].data.push(data.SoilMoisture);//push value
        analysisChart.data.datasets[2].data.push(soilMoisturePercentage); // Push percentage value
        analysisChart.data.labels.push(new Date().toLocaleTimeString());
        if (analysisChart.data.datasets[0].data.length > 50) {
          analysisChart.data.datasets[0].data.shift();
          analysisChart.data.datasets[1].data.shift();
          analysisChart.data.datasets[2].data.shift();
          analysisChart.data.labels.shift();
        }

        analysisChart.update();
      })
      .catch(err => console.error('Error fetching sensor data:', err));
  }, 2000);
  // Initialize analysis chart
  const ctx = document.getElementById('analysis-chart').getContext('2d');
  const analysisChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: [],
      datasets: [
        {
          label: 'Humidity',
          borderColor: 'rgba(24, 173, 49, 0.8)',
          backgroundColor: 'rgba(24, 173, 49, 0.2)',
          data: [],
          fill: true
        },
        {
          label: 'Temperature',
          borderColor: 'rgba(255, 99, 132, 0.8)',
          backgroundColor: 'rgba(255, 99, 132, 0.2)',
          data: [],
          fill: true
        },
        {
          label: 'Soil Moisture',
          borderColor: 'rgba(54, 162, 235, 0.8)',
          backgroundColor: 'rgba(54, 162, 235, 0.2)',
          data: [],
          fill: true
        }
      ]
    },
    options: {
      responsive: true,
      scales: {
        y: {
          beginAtZero: true
        }
      }
    }
  });
  const cityin = prompt("Please enter your City:");
  const apiKey = 'df088c0957819b473da108c944b0aa6c'; // Your OpenWeatherMap API key
    const city = cityin; // Replace with your city
    const apiUrl = `https://api.openweathermap.org/data/2.5/weather?q=${city}&units=metric&appid=${apiKey}`;
    // Fetch current weather data
    fetch(apiUrl)
      .then(response => response.json())
      .then(data => {
        // Current weather
        document.getElementById('temp').innerText = `${data.main.temp}°C`;
        document.getElementById('location').innerText = data.name;
        document.getElementById('description').innerText = data.weather[0].description;
        document.getElementById('details').innerText = `${data.wind.speed} mph | ${data.main.pressure} mb | ${data.main.humidity}%`;
        // Forecast (for simplicity, using the same data for today, tomorrow, and Friday)
        const iconUrl = `http://openweathermap.org/img/wn/${data.weather[0].icon}.png`;
        document.getElementById('today-icon').src = iconUrl;
        document.getElementById('tomorrow-icon').src = iconUrl;
        document.getElementById('friday-icon').src = iconUrl;
        document.getElementById('today-temp').innerText = `${data.main.temp_max}°C / ${data.main.temp_min}°C`;
        document.getElementById('tomorrow-temp').innerText = `${data.main.temp_max}°C / ${data.main.temp_min}°C`;
        document.getElementById('friday-temp').innerText = `${data.main.temp_max}°C / ${data.main.temp_min}°C`;
      })
      .catch(error => console.error('Error fetching weather data:', error));
  // Initial valve states
  const valveStates = { 1: false, 2: false, 3: false };
  // Function to toggle valve
  function toggleValve(valveNumber) {
    const valveState = valveStates[valveNumber] ? 'off' : 'on';
    valveStates[valveNumber] = !valveStates[valveNumber];
    // Update button text and color
    const button = document.querySelector(`#valve${valveNumber}-btn`);
    button.style.backgroundColor = valveState === 'on' ?  ' rgb(24, 173, 49)':' #e74c3c';
    //alert(`Valve ${valveNumber} is now ${valveState.toUpperCase()}`);
    // Show the appropriate alert based on the valve number
    if (valveState === 'on') {
      switch (valveNumber) {
          case 1:
              alert("Nitrogen is turning OFF");
              button.textContent = `NITROGEN ${valveNumber} ${valveState === 'on' ? 'ON' : 'OFF'}`;
              break;
          case 2:
              alert("Potassium is turning OFF");
              button.textContent = `POTASSIUM ${valveNumber} ${valveState === 'on' ? 'ON' : 'OFF'}`;
              break;
          case 3:
              alert("Phosphorus is turning OFF");
              button.textContent = `PHOSPHORUS ${valveNumber} ${valveState === 'on' ? 'ON' : 'OFF'}`;
              break;
          default:
              alert("Unknown valve");
              break;
      }
  }
  if (valveState != 'on') {
    switch (valveNumber) {
        case 1:
            alert("Nitrogen is turning ON ! Plz wait 30 sec");
            button.textContent = `NITROGEN ${valveNumber} ${valveState === 'on' ? 'ON' : 'OFF'}`;
            break;
        case 2:
            alert("Potassium is turning ON ! Plz wait 30 sec");
            button.textContent = `POTASSIUM ${valveNumber} ${valveState === 'on' ? 'ON' : 'OFF'}`;
            break;
        case 3:
            alert("Phosphorus is turning ON ! Plz wait 30 sec");
            button.textContent = `PHOSPHORUS ${valveNumber} ${valveState === 'on' ? 'ON' : 'OFF'}`;
            break;
        default:
            alert("Unknown valve");
            break;
    }
}
    // Send command to NodeMCU
    fetch(`/control-valve/${valveNumber}/${valveState}`)
      .then(response => response.json())
      .then(data => {
        console.log(`Valve ${valveNumber} is now ${valveState}`);
      })
      .catch(error => {
        console.error('Error toggling valve:', error);
      });
  }
</script>
</body>
</html>
)rawliteral"; 
