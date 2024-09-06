#include <wiringPi.h>
#include <stdio.h>


#define TRIG1 2
#define ECHO1 3
#define LED1  4

#define TRIG2 17
#define ECHO2 27
#define LED2  22

#define SEG_A 0
#define SEG_B 1
#define SEG_C 19
#define SEG_D 16
#define SEG_E 26
#define SEG_F 20
#define SEG_G 21

double distance1 = 0.0;
double distance2 = 0.0;
int led1_vacancy = 0;	// 1 == vacant, 0 == occupied
int led2_vacancy = 0;
int vacancy_count = 0;

// Initializes the ultrasonic sensor by setting up the TRIG, ECHO, and LED pins.
void initSensor(int trig, int echo, int led)
{
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(led, OUTPUT);

    digitalWrite(trig, LOW);
}

void init7Segment()
{
    pinMode(SEG_A, OUTPUT);
    pinMode(SEG_B, OUTPUT);
    pinMode(SEG_C, OUTPUT);
    pinMode(SEG_D, OUTPUT);
    pinMode(SEG_E, OUTPUT);
    pinMode(SEG_F, OUTPUT);
    pinMode(SEG_G, OUTPUT);
}

// Initializes the wiringPi library and sets up the ultrasonic sensors, and LED
void setup()
{
    wiringPiSetupGpio();

    initSensor(TRIG1, ECHO1, LED1);
    initSensor(TRIG2, ECHO2, LED2);

    delay(30);
}

void displayNumber(int number)
{
    delay(500);
    switch (number)
    {
        case 0:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, HIGH);
            digitalWrite(SEG_F, HIGH);
            digitalWrite(SEG_G, LOW);
            break;
        case 1:
            digitalWrite(SEG_A, LOW);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, LOW);
            digitalWrite(SEG_E, LOW);
            digitalWrite(SEG_F, LOW);
            digitalWrite(SEG_G, LOW);
            break;
        case 2:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, LOW);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, HIGH);
            digitalWrite(SEG_F, LOW);
            digitalWrite(SEG_G, HIGH);
            break;
        case 3:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, LOW);
            digitalWrite(SEG_F, LOW);
            digitalWrite(SEG_G, HIGH);
            break;
        case 4:
            digitalWrite(SEG_A, LOW);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, LOW);
            digitalWrite(SEG_E, LOW);
            digitalWrite(SEG_F, HIGH);
            digitalWrite(SEG_G, HIGH);
            break;
        case 5:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, LOW);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, LOW);
            digitalWrite(SEG_F, HIGH);
            digitalWrite(SEG_G, HIGH);
            break;
        case 6:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, LOW);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, HIGH);
            digitalWrite(SEG_F, HIGH);
            digitalWrite(SEG_G, HIGH);
            break;
        case 7:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, LOW);
            digitalWrite(SEG_E, LOW);
            digitalWrite(SEG_F, LOW);
            digitalWrite(SEG_G, LOW);
            break;
        case 8:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, HIGH);
            digitalWrite(SEG_F, HIGH);
            digitalWrite(SEG_G, HIGH);
            break;
        case 9:
            digitalWrite(SEG_A, HIGH);
            digitalWrite(SEG_B, HIGH);
            digitalWrite(SEG_C, HIGH);
            digitalWrite(SEG_D, HIGH);
            digitalWrite(SEG_E, LOW);
            digitalWrite(SEG_F, HIGH);
            digitalWrite(SEG_G, HIGH);
            break;
        default:
           
            break;
    }
}

double getDistance(int trig, int echo)
{
    // Trigger the ultrasonic pulse
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Wait 1sec for the ECHO pin to go HIGH
    unsigned long startWait = millis();
    while (digitalRead(echo) == LOW)
    {
        if (millis() - startWait > 1000) { // 1sec timeout
            printf("ECHO pin has not changed to HIGH\n");
            return -1; // return error
        }
    }

    // Record the start time
    long startTime = micros();

    // Wait for the ECHO pin to go LOW
    while (digitalRead(echo) == HIGH);

    // Calculate the travel time of the pulse
    long travelTime = micros() - startTime;

    // Calculate the distance
    double distance = (travelTime / 2.0) * 0.0343;

    return distance;
}

void controlLED(int led, double distance, double threshold)
{
    // Control the LED based on the distance compared to the threshold
    if (distance <= threshold)
    {
        digitalWrite(led, LOW);
        if (led == LED1) led1_vacancy = 0;
        if (led == LED2) led2_vacancy = 0;
    }
    else
    {
        digitalWrite(led, HIGH);
        if (led == LED1) led1_vacancy = 1;
        if (led == LED2) led2_vacancy = 1;
    }
    printf("LED1 vacancy: %d, LED2 vacancy: %d\n", led1_vacancy, led2_vacancy);
}
