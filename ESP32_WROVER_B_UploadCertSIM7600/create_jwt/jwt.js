const {readFileSync} = require('fs');
const jwt = require('jsonwebtoken');

const createJwt = (projectId, privateKeyFile, algorithm) => {
  // Create a JWT to authenticate this device. The device will be disconnected
  // after the token expires, and will have to reconnect with a new token. The
  // audience field should always be set to the GCP project id.
  const token = {
    iat: parseInt(Date.now() / 1000),
    exp: parseInt(Date.now() / 1000) + 240 * 60, // 60 minutes
    aud: projectId,
  };
  const privateKey = readFileSync(privateKeyFile);

  return jwt.sign(token, privateKey, {algorithm: algorithm});
};


let data = createJwt("testmqtt-325807", "./data/privKey.pem","ES256");
console.log(data);

