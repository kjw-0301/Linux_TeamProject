#include <wiringPi.h>
#include <stdio.h>

#define LED_PIN 27

int main() {
    wiringPiSetup();
    pinMode(LED_PIN, OUTPUT);

    while (1) {
        digitalWrite(LED_PIN, HIGH);  // LED �ѱ�
        delay(1000);                   // 1�� ���
        digitalWrite(LED_PIN, LOW);   // LED ����
        delay(1000);                   // 1�� ���
    }

    return 0;
}
