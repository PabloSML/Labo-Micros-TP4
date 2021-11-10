/*******************************************************************/
/*** Configuration ***/
/*******************************************************************/
const serialPortName = "COM5";
/*******************************************************************/


/*******************************************************************/
/*** Gateway code ***/
/*******************************************************************/
const SerialPort = require("serialport");


const port = new SerialPort(serialPortName, {  
    baudRate: 1200
});

port.on('data', function (data) {
    console.log('--Received Data--')
    console.log('Discard', data);
});

port.on('open', () => {
    console.log("Serial port open");
    
});

console.log("------------------------------------------");
console.log("22.99 2019 - Gateway v0.00");
console.log("Using COM port:", serialPortName);
console.log("Use CTRL+C to exit");
console.log("------------------------------------------");
