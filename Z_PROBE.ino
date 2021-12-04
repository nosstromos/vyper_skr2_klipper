/**
 * @author nosstromOS 2021-12-04
 * https://github.com/nosstromos/vyper_skr2_klipper
 *
 * PROOF OF CONCEPT - USE AT YOUR OWN RISK!
 *  
 * Partial replacemend for original NF_007_EXTRUDER_V0.0.5 board of anycubic vyper.
 * As part of the solution of replacing the motherboard with an SKR2 operating under Klipper.
 * 
 * HX711 circuit wiring; loadcell (LC)
 * anycubic vyper 2021 DMS (LC = black holder for hotend)
 *
 * E+ = green; EXEC means power supply of DMS
 * E- = yellow
 * 
 * A+ (S+) = black; SIG means output value from DMS
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

const int HX_DT_PIN = 2;        // connection of the HX711 also referred as 'DOUT'
const int HX_SCK_PIN = 3;       // connection of the HX711

const int signal_delay = 1000;  // delay to hold z_probe pin HIGH after touch; default 1000
const int Z_PROBE_PIN = 5;      // delayed output signal for z_probe to 3D printer-board; HIGH = touched

float diff_raw = 0;             // difference between two raw measured values
const int t_raw = 5000;         // threshold of raw measured values for signal z_probe
                                // must be determined by yourself - see README

HX711 scale;

void setup()
{
  Serial.begin(38400);                  // after setup comment '//' all serial prints!
  Serial.println("MANUEL SETUP MODE Z_PROBE START");
  
  scale.begin(HX_DT_PIN, HX_SCK_PIN);   // start HX711 scale 
  
  digitalWrite(Z_PROBE_PIN, HIGH);      // start up set HIGH
  delay(signal_delay);
  digitalWrite(Z_PROBE_PIN, LOW);       // after delay set LOW
}

void loop()
{
  diff_raw = (scale.read() - scale.read());
  Serial.println(diff_raw);
  
  if (diff_raw > t_raw)
    {
    digitalWrite(Z_PROBE_PIN, HIGH);
    delay(signal_delay);
    } 
  
  digitalWrite(Z_PROBE_PIN, LOW);
}
