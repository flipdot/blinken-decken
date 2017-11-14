int wifi_connect();
void buf_random(CRGB buf[], size_t size, CRGB c1, CRGB c2);
void buf_progress_bar(CRGB buf[], size_t size, CRGB c1, CRGB c2, double progress);
void buf_grey(CRGB buf[], size_t size);
void led_stripe_update(double heartbeat, CRGB buf[], CRGB buf_co2[], CRGB buf_usr[], size_t size);
void callback(char* topic, byte* payload, unsigned int length);
