#include <wiringPi.h>
#include <stdio.h>

#define LED_PIN 27

int main() {
    wiringPiSetup();
    pinMode(LED_PIN, OUTPUT);

    while (1) {
        digitalWrite(LED_PIN, HIGH);  // LED 켜기
        delay(1000);                   // 1초 대기
        digitalWrite(LED_PIN, LOW);   // LED 끄기
        delay(1000);                   // 1초 대기
    }

    return 0;
}
