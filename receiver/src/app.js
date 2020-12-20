const express = require('express');

let lastReport = {};
let lastAlertTime = Date.parse('2000-01-01');

function createApp(apiKey, minVoltage, alertIntervalInSeconds) {
  const app = express();
  app.use(express.json());
  app.use(auth(apiKey));
  app.get('/healthz', (_, res) => res.sendStatus(200));
  app.route('/voltage')
    .get(sendLastReport)
    .post(reportHandler(minVoltage, alertIntervalInSeconds));
  return app;
}

function auth(apiKey) {
  return (req, res, next) => {
    try {
      if (req.headers['x-api-key'] !== apiKey) {
        return res.sendStatus(401);
      }
      next();
    } catch (error) {
      console.error(error);
      res.sendStatus(500);
    }
  };
}

function sendLastReport(_, res) {
  res.send(lastReport);
}

function reportHandler(minVoltage, alertIntervalInSeconds) {
  return (req, res) => {
    try {
      if (typeof req.body.voltage !== 'number') {
        return res.sendStatus(400);
      }

      const now = new Date();

      // Track the most recent report
      lastReport = {
        time: now.toISOString(),
        voltage: req.body.voltage,
      };

      // Alert on low voltage
      if (req.voltage < minVoltage) {
        if ((now - lastAlertTime) / 1000 > alertIntervalInSeconds) {
          // TODO: Send email
          lastAlertTime = now;
        }
      }

      // TODO: Store lastReport

      res.sendStatus(200);
    } catch (error) {
      console.error(error);
      res.sendStatus(500);
    }
  };
}

module.exports = createApp;
