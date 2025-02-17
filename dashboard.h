#ifndef DASHBOARD_H
#define DASHBOARD_H
const char dashboard_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DASHBOARD</title>
  <style>
    @import url('https://fonts.googleapis.com/css?family=Montserrat');
    @import url('https://fonts.googleapis.com/css?family=Advent+Pro:400,200');
    * { margin: 0; padding: 0; }

    body {
      background:rgb(83, 78, 77);
      font-family: Montserrat, Arial, sans-serif;
    }

    h2 { font-size: 14px; }

    .widget {
      box-shadow: 0 40px 10px 5px rgba(0, 0, 0, 0.4);
      margin: 100px auto;
      height: 360px;
      position: relative;
      width: 500px;
    }

    .upper {
      border-radius: 5px 5px 0 0;
      background: #f5f5f5;
      height: 200px;
      padding: 20px;
    }

    .date { font-size: 40px; }

    .year {
      font-size: 30px;
      color: #c1c1c1;
    }

    .place { color: #222; font-size: 40px; }

    .status {
      color: #222;
      font-size: 20px;
      margin-top: 10px;
    }
    .lower {
      background: rgb(24, 173, 49);
      border-radius: 0 0 5px 5px;
      font-family: 'Montserrat';
      font-weight: 200;
      height: 160px;
      width: 100%;
    }
    .clock {
      background: rgb(21, 202, 60);
      border-radius: 100%;
      box-shadow: 0 0 0 15px #f5f5f5, 0 10px 10px 5px rgba(0, 0, 0, 0.3);
      height: 150px;
      position: absolute;
      right: -25px;
      top: -35px;
      width: 150px;
    }
    .hour, .min {
      background: #f5f5f5;
      border-radius: 5px;
      position: absolute;
      left: 50%;
      transform-origin: bottom center;
      transition: all 0.5s linear;
    }
    .hour { height: 50px; width: 4px; top: 25px; }
    .min { height: 65px; width: 4px; top: 10px; }
    .infos { list-style: none; }
    .info {
      color: #fff;
      float: left;
      height: 100%;
      padding-top: 20px;
      text-align: center;
      width: 25%;
    }
    .info span {
      display: inline-block;
      font-size: 30px;
      margin-top: 25px;
    }
    .weather p { font-size: 20px; padding: 10px 0; }
    .anim { animation: fade 0.8s linear; }
    @keyframes fade {
      0% { opacity: 0; }
      100% { opacity: 1; }
    }
    a {
      text-align: center;
      text-decoration: none;
      color: white;
      font-size: 15px;
      font-weight: 500;
    }
  </style>
</head>
<body>
  <div class="widget"> 
    <div class="clock">
      <div class="min" id="min"></div>
      <div class="hour" id="hour"></div>
    </div>
    <div class="upper">
      <div class="date" id="date">21 March</div>
      <div class="year">Temperature</div>
      <div class="place update" id="temperature">23 &deg;C</div>
      <div class="status" id="status">Status: Offline</div>
      <div class="status">Pump Status: <span id="pumpStatus">OFF</span></div>
    </div>
    <div class="lower">    
      <ul class="infos">
        <li class="info temp">
          <h2 class="title">TEMPERATURE</h2>
          <span class='update' id="temp">21 &deg;C</span>
        </li>
        <li class="info weather">
          <h2 class="title">SOIL MOISTURE</h2>
          <span class="update" id="moisture">0%</span>
        </li>
        <li class="info wind">
          <h2 class="title">RAIN</h2>
          <span class='update' id="rain">0%</span>
        </li>
        <li class="info humidity">
          <h2 class="title">HUMIDITY</h2>
          <span class='update' id="humidity">23%</span>
        </li>
      </ul>
    </div>
  </div>
  <script>
    setInterval(drawClock, 2000);

    function drawClock() {
      var now = new Date();
      var hour = now.getHours();
      var minute = now.getMinutes();

      // Date
      var options = { year: 'numeric', month: 'long', day: 'numeric' };
      var today = new Date();
      document.getElementById("date").innerHTML = today.toLocaleDateString("en-US", options);

      // Hour
      var hourAngle = (360 * (hour / 12)) + ((360 / 12) * (minute / 60));
      document.getElementById("hour").style.transform = "rotate(" + hourAngle + "deg)";

      // Minute
      var minAngle = 360 * (minute / 60);
      document.getElementById("min").style.transform = "rotate(" + minAngle + "deg)";

      // Get Humidity, Temperature, Rain, and Soil Moisture Data
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var obj = JSON.parse(this.responseText); // Parse JSON response
          document.getElementById("status").innerHTML = "Status: Online";
          document.getElementById("rain").innerHTML = obj.Rain == 0 ? "Rainy" : "No Rain"; 
          document.getElementById("temperature").innerHTML = Math.round(obj.Temperature) + "&deg;C";
          document.getElementById("temp").innerHTML = Math.round(obj.Temperature) + "&deg;C";
          document.getElementById("humidity").innerHTML = Math.round(obj.Humidity) + "%";
          document.getElementById("moisture").innerHTML = Math.round(100-((obj.SoilMoisture / 1023) * 100)) + "%"; 
          document.getElementById("pumpStatus").innerText = obj.PumpStatus == 1 ? "ON!" : "OFF!";
        } else if (this.readyState == 4) {
          document.getElementById("status").innerHTML = "Status: Offline";
        }
      };
      xhttp.open("GET", "readADC", true);
      xhttp.send();
    }
  </script>
</body>
</html>
)rawliteral";
#endif
