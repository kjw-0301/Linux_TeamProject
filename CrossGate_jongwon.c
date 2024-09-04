#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVO_PIN 18

const int PWM_0_DEGREE = 50;
const int PWM_90_DEGREE = 150;

void setServoPosition(int pwmValue) {
    pwmWrite(SERVO_PIN, pwmValue);
}

void writeTagToFile(const char *taginfo) {
    FILE *file = fopen("tag.txt", "a");
    if (file) {
        fprintf(file, "%s\n", taginfo);
        fclose(file);
    } else {
        printf("Error to File\n");
    }
}

int hasNewEntryRecord(char *lastEntry) {
    FILE *file = fopen("entry_records.txt", "r");
    if (!file) {
        printf("Error to open Entry File\n");
        return 0;
    }

    char currentEntry[100];
    int isNewEntry = 0;

    while (fgets(currentEntry, sizeof(currentEntry), file) != NULL) {
        currentEntry[strcspn(currentEntry, "\n")] = 0;
        
        if (strcmp(currentEntry, lastEntry) != 0) {
            isNewEntry = 1;
            strcpy(lastEntry, currentEntry); 
            break;
        }
    }

    fclose(file);
    return isNewEntry;
}

int hasNewOutRecord(char *lastOut) {
    FILE *file = fopen("out_records.txt", "r");
    if (!file) {
        printf("Error to open Out File\n");
        return 0;
    }

    char currentOut[100];
    int isNewOut = 0;

    while (fgets(currentOut, sizeof(currentOut), file) != NULL) {
        currentOut[strcspn(currentOut, "\n")] = 0;
        
        if (strcmp(currentOut, lastOut) != 0) {
            isNewOut = 1;
            strcpy(lastOut, currentOut); 
            break;
        }
    }

    fclose(file);
    return isNewOut;
}

void clearFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fclose(file);
    } else {
        printf("Error to clear File\n");
    }
}

int main(void) {
    if (wiringPiSetupGpio() == -1) {
        printf("False init wiringPi \n");
        return 1;
    }

    pinMode(SERVO_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(200);
    pwmSetClock(192);

    char lastEntry[100] = "";
    char lastOut[100] = "";
    int Crossgate = 0;

    while (1) {
        if (hasNewEntryRecord(lastEntry) && Crossgate == 0) {
            setServoPosition(PWM_90_DEGREE);
            delay(1000);
            setServoPosition(PWM_0_DEGREE);
            Crossgate = 1;

            // Clear entry_records.txt after processing
            clearFile("entry_records.txt");
        } 
        else if (hasNewOutRecord(lastOut) && Crossgate == 0) {
            setServoPosition(PWM_90_DEGREE);
            delay(1000);
            setServoPosition(PWM_0_DEGREE);
            Crossgate = 1;

            // Clear out_records.txt after processing
            delay(5000);  // Wait for 5 seconds before clearing
            clearFile("out_records.txt");
        } 
        else {
            Crossgate = 0;
        }
        delay(2000); 
    }

    return 0;
}
