const char htmlConfig[] PROGMEM = R"=====(
<!DOCTYPE html>
<html class="" lang="en">
<head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8"><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>ESP32 VoltMeter Config</title>
<script>
  function load(){
      var xmlHttp = new XMLHttpRequest();
        xmlHttp.onreadystatechange = function() { 
            if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
              var settings = JSON.parse(xmlHttp.responseText);
              for (var key of Object.keys(settings)) {
                  var element = document.getElementById(key);
                  if(element) {
                    if(element.type == "checkbox") {
                      element.checked = settings[key];
                    } else {
                      element.value = settings[key];
                    } 
                  }
              }
            }
        }
        xmlHttp.open("GET", '/settingget', true);
        xmlHttp.send(null);
  }
  (function() {
    load();
  })();

</script>

<style>div,fieldset,input,select{padding:5px;font-size:1em;}fieldset{background:#4f4f4f;}p{margin:0.5em 0;}input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#dddddd;color:#000000;}input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}input[type=range]{width:99%;}select{width:100%;background:#dddddd;color:#000000;}textarea{resize:vertical;width:98%;height:318px;padding:5px;overflow:auto;background:#1f1f1f;color:#65c115;}body{text-align:center;font-family:verdana,sans-serif;background:#252525;}td{padding:0px;}button{border:0;border-radius:0.3rem;background:#1fa3ec;color:#faffff;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}button:hover{background:#0e70a4;}.bred{background:#d43535;}.bred:hover{background:#931f1f;}.bgrn{background:#47c266;}.bgrn:hover{background:#5aaf6f;}a{color:#1fa3ec;text-decoration:none;}.p{float:left;text-align:left;}.q{float:right;text-align:right;}.r{border-radius:0.3em;padding:2px;margin:6px 2px;}.hf{display:none;}</style>
</head>
<body>
  <div style="text-align:left;display:inline-block;color:#eaeaea;min-width:340px;">
    <div style="text-align:center;color:#eaeaea;">
      <noscript>To use This enable JavaScript<br></noscript>
      <h2>ESP32 CAM Car config</h2>
    </div>
    <fieldset><legend><b>&nbsp;General&nbsp;</b></legend>
      <form method="get" action="/settingset"><p></p>
            <label><input id="apMode" type="checkbox" name="apMode"><b>AP mode</b></label><br><br>
            <label><b>SSID</b></label><br><input id="ssid" type="text" name="ssid"><br>
            <label><b>Password</b></label><br><input id="passwd" type="text" name="passwd"><br><br>
            <label><input id="staticIp" type="checkbox" name="staticIp"><b>Static IP</b></label><br>
            <label><b>IP</b></label><br><input id="ip" type="text" name="ip"><br>
        <br>
        <button type="submit" class="button bgrn">Save</button>
      </form>
    </fieldset>
    <br>

    <form id="but3" style="display: block;" action="/" method="get">
      <button name="">Back</button>
    </form><p></p>
    <form id="but3" style="display: block;" action="/restart" method="get">
      <button name="">Restart</button>
    </form><p></p>
    <div style="text-align:right;font-size:11px;"><hr><a href="https://bit.ly/tasmota" target="_blank" style="color:#aaa;">Tasmota GUI by Theo Arends</a></div>
  </div>

</body>
</html>
)=====";
