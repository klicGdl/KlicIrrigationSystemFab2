/*Ww10.4 1:08 AM Im creating this module to enable RTC in the Irrigation ssystem
I need to review functionality and it it compiles enough for today.
*/

#include <Wire.h>
#include <RTClib.h>

// Create an RTC object
RTC_DS1307 rtc;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Initialize I2C communication
  Wire.begin();

  // Check if the RTC is connected properly
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC lost power and if so, set the time
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // The following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // You can also set the RTC manually:
    // rtc.adjust(DateTime(2023, 10, 5, 12, 0, 0)); // YYYY, MM, DD, HH, MM, SS
  }
}

void loop() {
  // Get the current time
  DateTime now = rtc.now();

  // Print the current time
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Wait for a second before printing again
  delay(1000);
}

// Function to set the RTC time
void setRTCTime(int year, int month, int day, int hour, int minute, int second) {
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
}