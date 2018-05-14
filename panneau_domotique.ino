// Dans la marine on ne fait pas grand chose, mais on le fait de bonne heure.
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <APA102.h>

/**************** Timings ***************/
#define DO_EVERY(period, func) do { if(seqNumber % period == 0) { func(); } } while(0)
#define MATRIX_PERIOD 10
#define LEDS_PERIOD 4
#define VOLTMETER_PERIOD 2

/************** Voltmeters **************/
#define VOLT_GA 6
#define VOLT_BU 5
#define VOLT_ZO 3

/************** LED strip **************/
#define LED_DATA_PIN 11
#define LED_CLOCK_PIN 13

#define BRIGHTNESS 10 // from 0 to 31
#define LED_COUNT 20

// first 16 LEDs are in the sequencer, the next ones are defined below
#define COURSE_LUNE colors[16]
#define SIESTE colors[17]
#define PERCAGE colors[19]
#define ORGANISATION colors[18]

// smooth gaussian gradient for the K2000 fade in/fade out
static const uint8_t gradient[] = {
	0, 1, 3, 11, 35, 83, 155, 225, 255, 225, 155, 83, 35, 11, 3, 1
};

rgb_color colors[LED_COUNT]; // holds the colors to write to the LED strip
APA102<LED_DATA_PIN, LED_CLOCK_PIN> ledStrip; // contains methods to write to the LED strip

/************** LED matrix **************/
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();


void setup() {
	matrix.begin(0x70);

	pinMode(VOLT_GA, OUTPUT);
	pinMode(VOLT_BU, OUTPUT);
	pinMode(VOLT_ZO, OUTPUT);
}

void loop() {
	static int seqNumber = 0;

	DO_EVERY(MATRIX_PERIOD, updateMatrix);
	DO_EVERY(LEDS_PERIOD, updateLEDs);
	DO_EVERY(VOLTMETER_PERIOD, updateVoltmeters);

	delay(10);
	seqNumber++;
}

// from polulu LED strip examples
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v) {
	uint8_t f = (h % 60) * 255 / 60;
	uint8_t p = (255 - s) * (uint16_t)v / 255;
	uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
	uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
	uint8_t r = 0, g = 0, b = 0;
	switch((h / 60) % 6){
		case 0: r = v; g = t; b = p; break;
		case 1: r = q; g = v; b = p; break;
		case 2: r = p; g = v; b = t; break;
		case 3: r = p; g = q; b = v; break;
		case 4: r = t; g = p; b = v; break;
		case 5: r = v; g = p; b = q; break;
	}
	return rgb_color(r, g, b);
}

// generate a K2000 like effect with rainbow colors
void alex2000(int LEDseq) {
	int centerIndex = abs(16 - (LEDseq % 32));

	for(int i = 0; i < 16; i++) {
		unsigned int gradIndex = centerIndex - i + 8;
		colors[i] = hsvToRgb((i+LEDseq)*14, 255, gradIndex < 16 ? gradient[gradIndex] : 0);
	}
}

void updateLEDs() {
	static unsigned int LEDseq = 0;

	alex2000(LEDseq);

	if(LEDseq % 200 == 10) {
		SIESTE = rgb_color(0, 255, 0);
	} else if(LEDseq % 200 == 30) {
		ORGANISATION = rgb_color(0, 255, 0);
	} else if(LEDseq % 200 == 80) {
		SIESTE = rgb_color(255, 0, 0);
	} else if(LEDseq % 200 == 90) {
		ORGANISATION = rgb_color(0, 0, 0);
		SIESTE = rgb_color(0, 0, 0);
	}

	if(LEDseq % 200 > 90) {
		COURSE_LUNE = rgb_color(0, 100*(LEDseq % 10 < 5), 250*(LEDseq % 10 < 5));
	}

	ledStrip.write(colors, LED_COUNT, BRIGHTNESS);
	LEDseq++;
}

void updateMatrix() {
	uint8_t redLayer[8];
	uint8_t greenLayer[8];
	uint8_t yellowLayer[8];

	for(size_t i = 0; i < 8; i++) {
		redLayer[i]    = rand();
		greenLayer[i]  = rand();
		yellowLayer[i] = redLayer[i] & greenLayer[i];
	}

	matrix.clear();
	matrix.drawBitmap(0, 0, redLayer, 8, 8, LED_RED);
	matrix.drawBitmap(0, 0, greenLayer, 8, 8, LED_GREEN);
	matrix.drawBitmap(0, 0, yellowLayer, 8, 8, LED_YELLOW);
	matrix.writeDisplay();
}

void updateVoltmeters() {
	static uint8_t voltSeq = 0;
	int voltIndex = abs(63 - (voltSeq % 128));

	analogWrite(VOLT_GA, voltIndex*4);
	analogWrite(VOLT_BU, 127 - voltIndex*2);
	analogWrite(VOLT_ZO, voltIndex*4);

	voltSeq++;
}
