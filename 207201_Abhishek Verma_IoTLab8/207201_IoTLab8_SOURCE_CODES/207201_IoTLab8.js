var axios = require("axios");
let dgram = require("dgram");

const port = 7279;
let host = "aaaa::1";
let updServer = dgram.createSocket("udp6").bind(port, host);
const data = [];

updServer.on("message", (msg, sender) => {
  const message = `${msg}`;
  const temp = Number(message.substr(5, 2));
  if (!Number.isNaN(temp) && temp > 20) {
    console.log(
      `SENDER:${sender.address}:${sender.port} | Sending data to Ubidot, temp=${temp}`
    );
    sendData(temp);
  }
});

updServer.on("listening", () => {
  console.log(
    "Listening on ",
    updServer.address().address,
    updServer.address().port
  );
});

updServer.on("error", (err) => {
  console.log(err);
});

function sendData(temp) {
  var data = JSON.stringify({
    value: temp,
  });

  // console.log(data);

  var config = {
    method: "post",
    url: "https://industrial.api.ubidots.com/api/v1.6/devices/device1/temp/values",
    headers: {
      "X-Auth-Token": "BBUS-YUSYoVwbskQrzJvdzefnz8WzKjZNDA",
      "Content-Type": "application/json",
    },
    data: data,
  };

  const timeout = setTimeout(() => {
    axios(config)
      .then(function (response) {
        console.log("Data sent to Ubidot:", JSON.stringify(response.data));
      })
      .catch(function (error) {
        console.log("Error:", error);
      });
    clearTimeout(timeout);
  }, 5000);
}
