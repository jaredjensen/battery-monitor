const http = require('http');

function createServer(app, port, shutdownTimeoutMs) {
  const shutdown = (signal) => {
    console.log(`Received ${signal} signal. Shutting down.`);
    server.close(() => {
      console.log('Service shutdown');
      process.exit(0);
    });
    setTimeout(() => {
      console.log('Did not shutdown within timeout, forcing quit');
      console.log('Service shutdown');
      process.exit(1);
    }, shutdownTimeoutMs);
  };

  const onError = (err) => console.log('Server heard error event', err);
  const onListening = () => console.log(`Server started and listening on ${port}`);

  const server = http.createServer(app);
  server.listen(port);
  server.on('error', onError);
  server.on('listening', onListening);
  process.on('SIGTERM', shutdown.bind(null, 'SIGTERM'));
  process.on('SIGINT', shutdown.bind(null, 'SIGINT'));
  return server;
}

module.exports = createServer;
