#define DEBUG

// WiFi credentials
#define WIFI_SSID "my-ssid"
#define WIFI_PASS "password"

// Remote config
#define REMOTE_API_KEY "secret"
#define REMOTE_HOST "foo.com"
#define REMOTE_PATH "/voltage"
#define REMOTE_PORT 443

// Battery settings
#define FULL_CHARGE 9
#define CHECK_INTERVAL 2000

// Board settings
#define R1 1000000.0
#define R2 220000.0
#define BOARD_VOLTAGE 3.3
#define WIFI_CHIPSELECT 8
#define WIFI_IRQ 7
#define WIFI_RESET 4
#define WIFI_ENABLE 2

// LED registers
// See assignments on pinout (e.g. https://cdn-learn.adafruit.com/assets/assets/000/046/250/original/feather_m0_wifi_pinout_v1.2-1.png?1504885927)
#define LED_REG_R 7
#define LED_REG_G 18
#define LED_REG_B 16