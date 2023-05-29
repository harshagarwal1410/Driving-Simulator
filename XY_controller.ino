/*
 * Reads a potentiometer on pin 34 and maps the reading to the X axis
 *
 * Potentiometers can be noisy, so the sketch can take multiple samples to average out the readings
 */

#include <Arduino.h>
#include <BleGamepad.h>

BleGamepad bleGamepad;

const int potPinS = 34;
const int potPinA = 35;  
const int potPinB = 36;               // Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
const int numberOfPotSamples = 5;     // Number of pot samples to take (to smooth the values)
const int delayBetweenSamples = 4;    // Delay in milliseconds between pot samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleGamepad.begin();

    // bleGamepad.setY(32737/2);
    // bleGamepad.setX(32737/2);
    // bleGamepad.setZ(0);
    // bleGamepad.setRX(0);
}

void loop()
{
    if (bleGamepad.isConnected())
    {
        int potValuesS[numberOfPotSamples];
        int potValuesA[numberOfPotSamples]; // Array to store pot readings
        int potValuesB[numberOfPotSamples];
        int potValueS = 0;                  // Variable to store calculated pot reading average
        int potValueA = 0;
        int potValueB = 0;
        // Populate readings
        for (int i = 0; i < numberOfPotSamples; i++)
        {
            potValuesS[i] = analogRead(potPinS);
            potValueS += potValuesS[i];
            potValuesA[i] = analogRead(potPinA);
            potValueA += potValuesA[i];
            potValuesB[i] = analogRead(potPinB);
            potValueB += potValuesB[i];
            delay(delayBetweenSamples);
        }

        // Calculate the average
        potValueS = potValueS / numberOfPotSamples;
        potValueA = potValueA / numberOfPotSamples;
        potValueB = potValueB / numberOfPotSamples;
        // Map analog reading from 0 ~ 4095 to 32737 ~ 0 for use as an axis reading
        int adjustedValueS = map(potValueS, 0, 4095, 32737, 0);
        int adjustedValueA = map(potValueA, 0, 4095, 32737, 0);
        int adjustedValueB = map(potValueB, 0, 4095, 32737, 0);
        // Update X axis and auto-send report
        bleGamepad.setY(32737/2);
        bleGamepad.setX(adjustedValueS);
        bleGamepad.setZ(adjustedValueA);
        bleGamepad.setRX(adjustedValueB);
        delay(delayBetweenHIDReports);

        // The code below (apart from the 2 closing braces) is for pot value degugging, and can be removed
        // Print readings to serial port
        Serial.print("Sent: ");
        Serial.print(adjustedValueA);
        Serial.print("\tRaw Avg: ");
        Serial.println( potValueA);
        //Serial.print("\tRaw: {");

        // Iterate through raw pot values, printing them to the serial port
        
    }
}