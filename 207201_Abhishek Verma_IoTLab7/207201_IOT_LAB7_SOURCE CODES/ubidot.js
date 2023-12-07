var axios = require("axios");
let dgram = require("dgram");

const port = 7279;
let host = "aaaa::1";
let updServer = dgram.createSocket("udp6").bind(port, host);

updServer.on("message", (msg, sender) => {
  console.log(
    `SENDER:${sender.address}:${sender.port} | Sending data to Ubidot`
  );
  const timeout = setTimeout(() => {
    sendData();
    clearTimeout(timeout);
  }, 3000);
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

function sendData() {
  var data = JSON.stringify({
    value: Math.random(),
  });

  var config = {
    method: "post",
    url: "https://industrial.api.ubidots.com/api/v1.6/devices/device1/var1/values",
    headers: {
      "X-Auth-Token": "BBUS-YUSYoVwbskQrzJvdzefnz8WzKjZNDA",
      "Content-Type": "application/json",
    },
    data: data,
  };

  axios(config)
    .then(function (response) {
      console.log("Data sent to Ubidot:", JSON.stringify(response.data));
    })
    .catch(function (error) {
      console.log("Error:", error);
    });
}
