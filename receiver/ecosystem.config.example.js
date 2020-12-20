module.exports = {
  apps : [
      {
        name: 'bms',
        script: './src/index.js',
        watch: true,
        env: {
          ALERT_INTERVAL: 300,
          API_KEY: 'foo',
          MIN_VOLTAGE: 12.6,
          NODE_ENV: 'production',
          PORT: 3000,
          SHUTDOWN_TIMEOUT: 10000,
        }
      }
  ]
};
