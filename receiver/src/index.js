const createApp = require('./app');
const createServer = require('./server');

const API_KEY = process.env.API_KEY;
const ALERT_INTERVAL = Number(process.env.ALERT_INTERVAL || (60 * 15));
const MIN_VOLTAGE = Number(process.env.MIN_VOLTAGE || 12.6);
const PORT = Number(process.env.PORT || 80);
const SHUTDOWN_TIMEOUT = Number(process.env.SHUTDOWN_TIMEOUT || 10000);

const app = createApp(API_KEY, MIN_VOLTAGE, ALERT_INTERVAL);
createServer(app, PORT, SHUTDOWN_TIMEOUT);
