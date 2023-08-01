const char htmlRoot[] PROGMEM = R"=====(
<!DOCTYPE html>
<html class="" lang="en">
<head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>ESP32 Cam CAR</title>

<script>
  // javascript monitors pressed keys, and sends command every 500 miliseconds and right after keypress change
  var state = {
    "ArrowUp": false,
    "ArrowDown": false,
    "ArrowRight": false,
    "ArrowLeft": false,
  };
  var xhttp = new XMLHttpRequest();
  function getsend(arg) { xhttp.open('GET', '/'+arg +'?' + new Date().getTime(), true); xhttp.send() }
  function sendAction(arg)
  {
    console.log(arg);
    xhttp.open('GET', '/action?action=' + arg, true); 
    xhttp.send() 
  }
  
  window.addEventListener('keydown', (e) => {
    //alert(`Key pressed ${event.key} \n Key code Value: ${event.code}`);
    if(state[e.key] == true)
      return;
    state[e.key] = true;
    sendCommand(1);
  });
  window.addEventListener('keyup', (e) => {
    if(state[e.key] == false)
      return;
    state[e.key] = false;
    sendCommand(1);
  });

  function sendCommand(force=0) {
    var x = "";
    x+=state.ArrowUp ? "1" : "0";
    x+=state.ArrowDown ? "1" : "0";
    x+=state.ArrowRight ? "1" : "0";
    x+=state.ArrowLeft ? "1" : "0";
    if(force) {
      sendAction(x);
    } else if (x != "0000") {
      sendAction(x);
    }
  }

  setInterval(sendCommand, 600);

  function setStream() {
    document.getElementById('stream-src').src = window.location.protocol + "//" + window.location.hostname + ":81/stream";
  }
</script>
  
<style>
    div,fieldset,input,select{padding:5px;font-size:1em;}
    fieldset{background:#4f4f4f;}p{margin:0.5em 0;}
    input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#dddddd;color:#000000;}
    input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}
    input[type=range]{width:99%;}
    select{width:100%;background:#dddddd;color:#000000;}
    textarea{resize:vertical;width:98%;height:318px;padding:5px;overflow:auto;background:#1f1f1f;color:#65c115;}
    body{text-align:center;font-family:verdana,sans-serif;background:#252525;}td{padding:0px;}
    button{border:0;border-radius:0.3rem;background:#1fa3ec;color:#faffff;max-width:300px;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.1s;transition-duration:0.1s;cursor:pointer;box-shadow: 2px 2px #999;}
    button:hover{background:#0e70a4;}.bred{background:#d43535;}.bred:hover{background:#931f1f;}.bgrn{background:#47c266;}.bgrn:hover{background:#5aaf6f;}a{color:#1fa3ec;text-decoration:none;}.p{float:left;text-align:left;}.q{float:right;text-align:right;}.r{border-radius:0.3em;padding:2px;margin:6px 2px;}.hf{display:none;}
    .navbtn{width:150px;height:60px;}
    button:active {box-shadow: 1px 1px #666;transform: translateY(4px) translateX(4px);}
</style>
</head>
<body onload="setStream()">
  <div style="text-align:left;display:inline-block;color:#eaeaea;min-width:640px;"><div style="text-align:center;color:#eaeaea;">
  <noscript>To use This, enable javascript<br></noscript>
  <h2>ESP32 Cam Car</h2></div>
  <div style="padding:0;" id="l1" name="l1">
    <p align=center><img id="stream-src" style='width:600px; transform:rotate(180deg);'></p><br/><br/>
    <p align=center><button id="btngo" class="navbtn" onmousedown=getsend('go') onmouseup=getsend('stop') ontouchstart=getsend('go') ontouchend=getsend('stop') ><b>Forward</b></button></p>

    <p align=center>
    <button id="btnleft" class="navbtn" onmousedown=getsend('left') onmouseup=getsend('stop') ontouchstart=getsend('left') ontouchend=getsend('stop')><b>Left</b></button>&nbsp;
    <button id="btnstop" class="navbtn" onmousedown=getsend('stop') onmouseup=getsend('stop')><b>Stop</b></button>&nbsp;
    <button id="btnright" class="navbtn" onmousedown=getsend('right') onmouseup=getsend('stop') ontouchstart=getsend('right') ontouchend=getsend('stop')><b>Right</b></button>
    </p>
    <p align=center><button id="btnback" style="width:150px;height:60px" onmousedown=getsend('back') onmouseup=getsend('stop') ontouchstart=getsend('back') ontouchend=getsend('stop') ><b>Backward</b></button></p>
  </div>
  <br><br><br>
<p align=center>
  <button style=width:150px;height:40px onmousedown=getsend('ledon')><b>Light ON</b></button>
  <button style=width:150px;height:40px onmousedown=getsend('ledoff')><b>Light OFF</b></button>
</p>
<br>

  <form id="but3" style="left:50%" action="/setting" method="get">
    <p align=center><button>Configuration</button></p>
  </form>

  <div style="text-align:right;font-size:11px;"><hr><a href="https://bit.ly/tasmota" target="_blank" style="color:#aaa;">Tasmota GUI by Theo Arends</a></div>
</body>
</html>
)=====";
