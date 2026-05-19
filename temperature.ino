#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SimpleDHT.h>

// WIFI
const char* ssid = "ESP_LIGHT_WEATHER";
const char* password = "12345678";

ESP8266WebServer server(80);

// SENSOR
SimpleDHT11 dht11(D4);
byte temp = 0;
byte hum = 0;

// SENSOR READ
void readSensor() {
  int err = dht11.read(&temp, &hum, NULL);
  if (err != 0) {
    temp = 28;
    hum = 60;
  }
}

// API
void handleData() {
  readSensor();

  String json = "{";
  json += "\"t\":";
  json += temp;
  json += ",\"h\":";
  json += hum;
  json += "}";

  server.send(200, "application/json", json);
}

// WEB PAGE - Professional Dashboard with Mobile First Approach
const char webpage[] PROGMEM = R"====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover, user-scalable=no">
  <title>ESP Weather | Professional Dashboard</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      background: linear-gradient(145deg, #0B1120 0%, #111827 100%);
      font-family: 'Inter', system-ui, -apple-system, 'Segoe UI', Roboto, Helvetica, sans-serif;
      color: #F9FAFB;
      padding: 20px 16px 40px;
      min-height: 100vh;
    }

    .dashboard {
      max-width: 520px;
      margin: 0 auto;
    }

    .header-card {
      background: rgba(18, 25, 40, 0.75);
      backdrop-filter: blur(16px);
      border-radius: 48px;
      padding: 18px 24px;
      margin-bottom: 28px;
      border: 1px solid rgba(72, 187, 255, 0.2);
      box-shadow: 0 12px 30px rgba(0, 0, 0, 0.3);
      text-align: center;
      transition: all 0.2s ease;
    }

    .wifi-badge {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      background: #1E2A44;
      padding: 6px 14px;
      border-radius: 60px;
      font-size: 0.75rem;
      font-weight: 500;
      letter-spacing: 0.3px;
      color: #B9E6FF;
      margin-bottom: 10px;
    }

    .wifi-badge span:first-child {
      font-size: 1rem;
    }

    .ip-text {
      font-family: 'SF Mono', 'Fira Code', monospace;
      font-size: 0.85rem;
      background: #0F172A;
      display: inline-block;
      padding: 4px 12px;
      border-radius: 32px;
      color: #60A5FA;
      margin-top: 6px;
    }

    .header-title {
      font-size: 1.4rem;
      font-weight: 600;
      margin-top: 6px;
      background: linear-gradient(135deg, #FFFFFF, #94A3B8);
      background-clip: text;
      -webkit-background-clip: text;
      color: transparent;
    }

    .metrics-grid {
      display: flex;
      flex-direction: row;
      flex-wrap: wrap;
      gap: 18px;
      margin-bottom: 28px;
    }

    .metric-card {
      flex: 1;
      min-width: 150px;
      background: rgba(15, 23, 42, 0.7);
      backdrop-filter: blur(8px);
      border-radius: 40px;
      padding: 24px 12px 28px;
      text-align: center;
      border: 1px solid rgba(255, 255, 255, 0.08);
      transition: transform 0.2s, box-shadow 0.2s;
      box-shadow: 0 20px 35px -12px rgba(0, 0, 0, 0.4);
    }

    .metric-card:hover {
      transform: translateY(-3px);
      border-color: rgba(96, 165, 250, 0.4);
    }

    .metric-icon {
      font-size: 2.5rem;
      margin-bottom: 10px;
    }

    .metric-label {
      font-size: 0.85rem;
      text-transform: uppercase;
      letter-spacing: 1px;
      font-weight: 500;
      color: #9CA3AF;
      margin-bottom: 12px;
    }

    .metric-value {
      font-size: 3rem;
      font-weight: 700;
      line-height: 1;
      margin: 8px 0;
      letter-spacing: -0.5px;
    }

    .metric-unit {
      font-size: 1rem;
      font-weight: 400;
      color: #CBD5E1;
    }

    .status-chip {
      display: inline-block;
      padding: 5px 14px;
      border-radius: 50px;
      font-size: 0.7rem;
      font-weight: 600;
      background: #1F2A48;
      margin-top: 12px;
      backdrop-filter: blur(4px);
    }

    .gauge-wrapper {
      margin-top: 18px;
      display: flex;
      justify-content: center;
    }

    canvas {
      width: 120px;
      height: 120px;
    }

    .condition-panel {
      background: rgba(0, 0, 0, 0.35);
      border-radius: 60px;
      padding: 18px 24px;
      margin: 12px 0 24px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      flex-wrap: wrap;
      gap: 14px;
      backdrop-filter: blur(4px);
      border: 1px solid rgba(255, 215, 0, 0.2);
    }

    .condition-icon {
      font-size: 3rem;
      filter: drop-shadow(0 6px 10px rgba(0,0,0,0.4));
    }

    .condition-text {
      font-weight: 500;
      background: #111827CC;
      padding: 5px 16px;
      border-radius: 40px;
      font-size: 0.9rem;
    }

    .live-badge {
      font-size: 0.7rem;
      background: #10B98120;
      padding: 4px 10px;
      border-radius: 24px;
      color: #34D399;
    }

    .footer-note {
      text-align: center;
      font-size: 0.7rem;
      color: #6B7280;
      margin-top: 32px;
      border-top: 1px solid rgba(255,255,255,0.05);
      padding-top: 20px;
    }

    @media (max-width: 450px) {
      body {
        padding: 12px 12px 30px;
      }
      .metric-value {
        font-size: 2.4rem;
      }
      .metric-card {
        padding: 18px 8px 22px;
      }
      canvas {
        width: 100px;
        height: 100px;
      }
    }

    ::-webkit-scrollbar {
      width: 4px;
    }
  </style>
</head>
<body>
<div class="dashboard">
  
  <div class="header-card">
    <div class="wifi-badge">
      <span>📡</span> <span>ESP_LIGHT_WEATHER</span>
    </div>
    <div class="header-title">
      ⚡ Weather Station Pro
    </div>
    <div class="ip-text">
      🌐 192.168.4.1 · LIVE
    </div>
    <div class="live-badge" style="display: inline-block; margin-top: 8px;">
      🔄 auto-refresh 2s
    </div>
  </div>

  <div class="metrics-grid">
    <div class="metric-card" id="tempCard">
      <div class="metric-icon">🌡️</div>
      <div class="metric-label">TEMPERATURE</div>
      <div class="metric-value">
        <span id="tempValue">--</span><span class="metric-unit">°C</span>
      </div>
      <div class="gauge-wrapper">
        <canvas id="tempGauge" width="140" height="140" style="width:140px; height:140px;"></canvas>
      </div>
      <div class="status-chip" id="tempStatus">—</div>
    </div>

    <div class="metric-card" id="humCard">
      <div class="metric-icon">💧</div>
      <div class="metric-label">HUMIDITY</div>
      <div class="metric-value">
        <span id="humValue">--</span><span class="metric-unit">%</span>
      </div>
      <div class="gauge-wrapper">
        <canvas id="humGauge" width="140" height="140" style="width:140px; height:140px;"></canvas>
      </div>
      <div class="status-chip" id="humStatus">—</div>
    </div>
  </div>

  <div class="condition-panel">
    <div class="condition-icon" id="mainIcon">☀️</div>
    <div class="condition-text" id="conditionDesc">Stable Environment</div>
    <div class="live-badge">📊 realtime</div>
  </div>

  <div class="footer-note">
    🧪 DHT11 sensor · WiFi Access Point mode · secure dashboard
  </div>
</div>

<script>
  function drawGauge(canvasId, percent, color) {
    let canvas = document.getElementById(canvasId);
    if (!canvas) return;
    let ctx = canvas.getContext("2d");
    let size = canvas.width;
    let centerX = size / 2;
    let centerY = size / 2;
    let radius = (size * 0.42);
    let lineWidth = size * 0.09;
    
    ctx.clearRect(0, 0, size, size);
    
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
    ctx.strokeStyle = "#2D3A5E";
    ctx.lineWidth = lineWidth;
    ctx.stroke();
    
    let safePercent = Math.min(100, Math.max(0, percent));
    let endAngle = (safePercent / 100) * Math.PI * 2;
    let startAngle = -Math.PI / 2;
    
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, startAngle, startAngle + endAngle);
    ctx.strokeStyle = color;
    ctx.lineWidth = lineWidth;
    ctx.lineCap = "round";
    ctx.stroke();
    
    if (safePercent > 0) {
      let angle = startAngle + endAngle;
      let dotX = centerX + radius * Math.cos(angle);
      let dotY = centerY + radius * Math.sin(angle);
      ctx.beginPath();
      ctx.arc(dotX, dotY, lineWidth * 0.5, 0, 2 * Math.PI);
      ctx.fillStyle = color;
      ctx.fill();
    }
  }
  
  function getTemperatureStyles(tempC) {
    if (tempC < 20) return { color: "#3B82F6", label: "❄️ COOL" };
    if (tempC > 30) return { color: "#EF4444", label: "🔥 HOT" };
    return { color: "#10B981", label: "✓ NORMAL" };
  }
  
  function getHumidityStyles(hum) {
    if (hum < 30) return { color: "#F59E0B", label: "💨 DRY" };
    if (hum > 70) return { color: "#06B6D4", label: "💦 HUMID" };
    return { color: "#10B981", label: "✓ COMFORT" };
  }
  
  function getWeatherIconAndDesc(temp, hum) {
    if (temp >= 34) return { icon: "🔥🌡️", desc: "Extreme Heat Warning" };
    if (hum >= 85) return { icon: "🌧️⛈️", desc: "Heavy Humidity / Rain likely" };
    if (hum >= 70) return { icon: "☁️💧", desc: "Very Humid, Overcast" };
    if (temp > 30 && hum < 35) return { icon: "🌵🔥", desc: "Hot & Dry" };
    if (temp > 30) return { icon: "☀️🔥", desc: "Hot & Sunny" };
    if (temp < 18) return { icon: "❄️🧥", desc: "Chilly Conditions" };
    if (hum < 25) return { icon: "🍂💨", desc: "Very Dry Air" };
    if (hum > 65) return { icon: "🌥️💧", desc: "Mild & Humid" };
    return { icon: "☀️🌿", desc: "Pleasant Climate" };
  }
  
  function fetchSensorData() {
    fetch("/data")
      .then(response => response.json())
      .then(data => {
        let t = data.t;
        let h = data.h;
        
        document.getElementById("tempValue").innerHTML = t;
        document.getElementById("humValue").innerHTML = h;
        
        let tempStyle = getTemperatureStyles(t);
        let humStyle = getHumidityStyles(h);
        
        document.getElementById("tempStatus").innerHTML = tempStyle.label;
        document.getElementById("humStatus").innerHTML = humStyle.label;
        
        drawGauge("tempGauge", t, tempStyle.color);
        drawGauge("humGauge", h, humStyle.color);
        
        let weather = getWeatherIconAndDesc(t, h);
        document.getElementById("mainIcon").innerHTML = weather.icon;
        document.getElementById("conditionDesc").innerHTML = weather.desc;
      })
      .catch(err => {
        console.log("fetch error", err);
      });
  }
  
  setInterval(fetchSensorData, 2000);
  fetchSensorData();
</script>
</body>
</html>
)====";

// ROOT
void handleRoot() {
  server.send_P(200, "text/html", webpage);
}

// SETUP
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("WiFi Name: ESP_LIGHT_WEATHER");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

// LOOP
void loop() {
  server.handleClient();
}