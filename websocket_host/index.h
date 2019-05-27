const char MAIN_page[] PROGMEM = R"=====(
<!doctypehtml><meta charset=utf-8><title>WebSocket Test</title><script>var output,button,canvas,context,url="ws://192.168.4.1:1337/";function init(){button=document.getElementById("toggleButton"),output=document.getElementById("output"),canvas=document.getElementById("led"),(context=canvas.getContext("2d")).arc(25,25,15,0,2*Math.PI,!1),context.lineWidth=3,context.strokeStyle="black",context.stroke(),context.fillStyle="black",context.fill(),wsConnect(url)}function wsConnect(e){websocket=new WebSocket(e),websocket.onopen=function(e){onOpen(e)},websocket.onclose=function(e){onClose(e)},websocket.onmessage=function(e){onMessage(e)},websocket.onerror=function(e){onError(e)}}function onOpen(e){console.log("Connected"),button.disabled=!1,doSend("getLEDState")}function onClose(e){console.log("Disconnected"),button.disabled=!0,setTimeout(function(){wsConnect(url)},2e3)}function onMessage(e){switch(console.log("Received: "+e.data),e.data){case"0":console.log("LED is off"),context.fillStyle="black",context.fill();break;case"1":console.log("LED is on"),context.fillStyle="red",context.fill()}}function onError(e){console.log("ERROR: "+e.data)}function doSend(e){console.log("Sending: "+e),websocket.send(e)}function onPress(){doSend("toggleLED"),doSend("getLEDState")}window.addEventListener("load",init,!1)</script><h2>LED Control</h2><table><tr><td><button disabled id=toggleButton onclick=onPress()>Toggle LED</button><td><canvas height=50 id=led width=50></canvas></table><div id=output></div>
)=====";
