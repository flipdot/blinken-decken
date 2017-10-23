all:
	platformio run

upload:
	platformio run -e nodemcuv2 -t upload --upload-port=/dev/ttyUSB0

monitor:
	platformio device monitor -b 9600
