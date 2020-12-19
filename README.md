# Proj_FuzzyPD

## SIMULIDE

Load Firmware:

Proj_FuzzyPD.ino.mega.hex

## DASHBOARD NODE-RED

### Função de entrada

Função para pegar os dados que está saindo da placa Arduino.

```c++
    var output = msg.payload.split(";");

    var nivel = parseFloat(output[0]);
    var erro = parseFloat(output[1]);
    var potencia = parseFloat(output[2]);

    var msg1 = {payload:nivel};
    var msg2 = {payload:erro};
    var msg3 = {payload:potencia};

    msg1.topic = "Line1";

    return [msg1,msg2,msg3];
```

  
  
### Função de entrada do Setpoint

```c++
    var newMsg = {payload: msg.payload};
      newMsg.topic = "Line2";
   return newMsg;
```

## VIRTUAL SERIAL PORTS EMULATOR

COM1 = Connector

COM2 = Connector

COM1 <=> COM2 = Serial Redirector



  
