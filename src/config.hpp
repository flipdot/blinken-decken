#define NUM_LEDS 300
#define DATA_PIN 2
#define BAUDRATE 9600

#define MIN_PPM 600
#define MAX_PPM 3000

#define FRAME_SLEEP 1
#define HEARTBEAT_INC 0.01

#define C_CYAN    CRGB(5, 121, 64)
#define C_RED     CRGB(250, 5, 5)
#define C_GREY    CRGB(25, 25, 25)
#define C_BLACK   CRGB(0, 0, 0)
#define C_YELLOW  CRGB(245, 198, 0)

#define MQTT_TOPIC_CO2   "sensors/lounge/co2"
#define MQTT_TOPIC_USERS "sensors/all/users"

constexpr const char* SSID = "~(._.)~";
constexpr const char* PASSWORD = "w3333333333";
constexpr const char* MQTT_HOST = "HeikesComputer";
