const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
<meta charset="utf-8" />
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.container1 {
  width: 100%;
  height:50vh;
  background: #ffffff;
  top: 0;
  left: 200px;
}

.slider {
  -webkit-appearance: slider-vertical;
  height: 100%;
  width: 15px;
}

html {
        overflow: auto;
}
</style>
</head>

<title>WebSocket Test</title>
<body>


<p align="center">
<canvas id="led" width="50" height="50"></canvas><br>
<button onclick="onLedPressed()" align="center">LIGHT</button>
</p>
<br>
<table style="width:100%;table-layout:fixed"  border="0px" align="center" >
  <tr>
  <tr>
    <td></td>
    <td>
      <div class="container1" >
      <input type="range" min="-255" max="255" value="0" class="slider" id="leftSlider">
      <p><span id="leftValue"></span></p>
      </div>
    </td>
    <td>
      <div class="container1">
        <input type="range" min="-255" max="255" value="0" class="slider" id="centerSlider">
        </div>
    </td>
    <td>
      <div class="container1">
        <input type="range" min="-255" max="255" value="0" class="slider" id="rightSlider">
        <p><span id="rightValue"></span></p>
        </div>
    </td>
  </tr>
</table>
<br>
<p align="center">
<button onclick="centerAll()" align="center">STOP</button>
</p>

<script>
var url = "ws://192.168.4.1:1337/";
var output;
var button;
var canvas;
var context;
// This is called when the page finishes loading
function init() {
    // Assign page elements to variables
    button = document.getElementById("toggleButton");
    output = document.getElementById("output");
    canvas = document.getElementById("led");
    
    // Draw circle in canvas
    context = canvas.getContext("2d");
    context.arc(25, 25, 15, 0, Math.PI * 2, false);
    context.lineWidth = 3;
    context.strokeStyle = "black";
    context.stroke();
    context.fillStyle = "white";
    context.fill();
    
    // Connect to WebSocket server
    wsConnect(url);
}
// Call this to connect to the WebSocket server
function wsConnect(url) {
    
    // Connect to WebSocket server
    websocket = new WebSocket(url);
    
    // Assign callbacks
    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}
// Called when a WebSocket connection is established with the server
function onOpen(evt) {
    // Log connection state
    console.log("Connected");
    
    // Enable button
    button.disabled = false;
    
    // Get the current state of the LED
    doSend("getLEDState");
}
// Called when the WebSocket connection is closed
function onClose(evt) {
    // Log disconnection state
    console.log("Disconnected");
    
    // Disable button
    button.disabled = true;
    
    // Try to reconnect after a few seconds
    setTimeout(function() { wsConnect(url) }, 2000);
}
// Called when a message is received from the server
function onMessage(evt) {
    // Print out our received message
    console.log("Received: " + evt.data);
    
    // Update circle graphic with LED state
    switch(evt.data) {
        case "0":
            console.log("LED is off");
            context.fillStyle = "white";
            context.fill();
            break;
        case "1":
            console.log("LED is on");
            context.fillStyle = "blue";
            context.fill();
            break;
        default:
            break;
    }
}
// Called when a WebSocket error occurs
function onError(evt) {
    console.log("ERROR: " + evt.data);
}
// Sends a message to the server (and prints it to the console)
function doSend(message) {
    console.log("Sending: " + message);
    websocket.send(message);
}
// Called whenever the HTML button is pressed
function onLedPressed() {
    doSend("toggleLED");
    doSend("getLEDState");
}
// Call the init function as soon as the page loads
window.addEventListener("load", init, false);

////////////// LEVERS /////////////////
var sliderLeft = document.getElementById("leftSlider");
var outputLeft = document.getElementById("leftValue");
outputLeft.innerHTML = sliderLeft.value;

var sliderRight = document.getElementById("rightSlider");
var outputRight = document.getElementById("rightValue");
outputRight.innerHTML = sliderRight.value;

var sliderCenter = document.getElementById("centerSlider");

sliderRight.oninput = function() {
  setRight(this.value);
}

sliderLeft.oninput = function() {
  setLeft(this.value);
}

sliderCenter.oninput = function() {
  value = this.value;
  setRight(value);
  setLeft(value);
  sliderLeft.value = value;
  sliderRight.value = value;
  sliderCenter.value = value;
}

function centerAll() {
  value = 0;
  setRight(value);
  setLeft(value);
  sliderLeft.value = value;
  sliderRight.value = value;
  sliderCenter.value = value;
}

function setRight(value) {
  outputRight.innerHTML = value;
  doSend("setLeftMotor:"+value);
}

function setLeft(value) {
  outputLeft.innerHTML = value;
  doSend("setRightMotor:"+value);
}


</script>

</body>
</html>




)=====";
