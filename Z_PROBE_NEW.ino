/**
 * @author nosstromOS 2021-12-20  
 * 
 * PROOF OF CONCEPT - USE AT YOUR OWN RISK!
 *  
 * Partial replacemend for original NF_007_EXTRUDER_V0.0.5 board of anycubic vyper.
 * As part of the solution of replacing the motherboard with an SKR2 operating under Klipper.
 * 
 * HX711 circuit wiring; loadcell (LC)
 * anycubic vyper 2021 Z_PROBE with strain gauge (LC = black holder for hotend)
 *
 * E+ = green; EXEC means power supply of strain gauge
 * E- = yellow
 * 
 * A+ (S+) = black; SIG means output signal from strain gauge
 * A- (S-) = red
 * 
 * B+ = not used because 4-wiring
 * B- = not used because 4-wiring
 *
 * wheatstone bridge (kE = kiloohm)
 * resistance value between E+ and E- = 0.35kE
 * resistance value between A+ and A- = 0.35kE
 *
 * based on:
 * HX711 library for Arduino
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/

#include "HX711.h"

// connection to HX711
#define HX_DT_PIN 2
#define HX_SCK_PIN 3

// Output: Z_LED signals touch to bed
#define SIG_Z_LED_PIN 5
// Output: Z_PROBE signal to SKR2 (PE4)
#define SIG_Z_PROBE_PIN 4

const int signal_delay = 10;        // delay to hold z_probe pin LOW after touch; default 800
// const float signal_wait = 0.5;   // wait for the 2. measurmend
float measure = 0.0; 
float threshold = 5.0;              // threshold of grams [g] measured values for signal z_probe
                                    // must be determined by yourself - to high demage the nozzle/bed!!!

HX711 scale;

void setup()
{
  // Serial.begin(38400);                  // after setup comment '//' all serial prints!
  // Serial.println("MANUALLY SETUP MODE Z_PROBE START");
  
  scale.begin(HX_DT_PIN, HX_SCK_PIN);   // start HX711 scale 

  scale.set_scale(870.f);               // value is obtained by calibrating LC
  scale.tare();
  
  pinMode(SIG_Z_LED_PIN, OUTPUT);
  pinMode(SIG_Z_PROBE_PIN, OUTPUT);
  
  digitalWrite(SIG_Z_LED_PIN, HIGH);      // start up set HIGH
  delay(signal_delay);
  digitalWrite(SIG_Z_LED_PIN, LOW);       // after delay set LOW

}

void loop()
{
   float measure = scale.get_units();
   // Serial.print(scale.get_units(2), 1);
  
  digitalWrite(SIG_Z_PROBE_PIN, HIGH);
  digitalWrite(SIG_Z_LED_PIN, LOW);
  
  if (measure > threshold)
    {
    digitalWrite(SIG_Z_PROBE_PIN, LOW);
    digitalWrite(SIG_Z_LED_PIN, HIGH);
    delay(signal_delay);
    }
}
