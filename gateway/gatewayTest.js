/*******************************************************************/
/*** Configuration ***/
/*******************************************************************/
const thingSpeakAPIKey = "BP2FFIPZR22Z8Y83";
/*******************************************************************/


/*******************************************************************/
/*** Gateway code ***/
/*******************************************************************/
const https = require('https');

num1 = 0;
num2 = 69;
num3 = 420;

var discard = false;

var state = 0;
var mode = 0;
var length;
var buff;

function APIWrite() {
	console.log('Received command 0x01, num1=' + num1, 'num2=' + num2, 'num3=' + num3);
	url = "https://api.thingspeak.com/update.json?api_key=" + thingSpeakAPIKey 
                + "&field1=" + num1
                + "&field2=" + num2
                + "&field3=" + num3;

	console.log('GET to ' + url);
	https.get(url, (resp) => {
	let data = '';
	resp.on('data', (chunk) => {
	    data += chunk;
	});

	resp.on('end', () => {
		if (resp.statusCode != 200) {
                	console.log("statusCode: " + resp.statusCode);
                        discard = false;
                } else {
                	data = JSON.parse(data);
                        if (typeof data === 'object' && data !== null) {
                            console.log('Received', data);
                            discard = false;
                        } else {
                            console.log('Received', data, '(error, too fast?)');
                            discard = false;
                        }
                    }
                });
            }).on("error", (err) => {
                console.log("Error: " + err.message);
                discard = false;
            });
}

console.log("------------------------------------------");
console.log("22.99 2019 - Gateway Test v0.00");
console.log("Using API Key:", thingSpeakAPIKey);
console.log("Use CTRL+C to exit");
console.log("------------------------------------------");
APIWrite();
