#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPiI2C.h>

#define SERVO_PIN 18
#define LED_PIN 27
#define LIGHT_THRESHOLD 40 
#define PCF8591_ADDR 0x48

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
	 pinMode(LED_PIN, OUTPUT);
    int fd = wiringPiI2CSetup(PCF8591_ADDR);

    if (fd == -1) {
        printf("I2C fail \n");
        return -1;
    }

    pinMode(SERVO_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(200);
    pwmSetClock(192);

    char lastEntry[100] = "";
    char lastOut[100] = "";
    int Crossgate = 0;

    while (1) {
		
		int adcValue = wiringPiI2CReadReg8(fd, PCF8591_ADDR | 0x40); 
        printf("ADC Value: %d\n", adcValue);
        
        if (hasNewEntryRecord(lastEntry) && Crossgate == 0) {
            setServoPosition(PWM_90_DEGREE);
            while(1)
            {
				adcValue = wiringPiI2CReadReg8(fd, PCF8591_ADDR | 0x40); 
				printf("ADC Value_in: %d\n", adcValue);
				if(adcValue > LIGHT_THRESHOLD)
				{
					printf("The car didn't pass\n");
				}
				else if(adcValue < LIGHT_THRESHOLD)
				{
					printf("car in");
					delay(3000);
					break;
				}
			}
			setServoPosition(PWM_0_DEGREE);
			Crossgate = 1;
            // Clear entry_records.txt after processing
            clearFile("entry_records.txt");
        } 
        else if (hasNewOutRecord(lastOut) && Crossgate == 0) {
            setServoPosition(PWM_90_DEGREE);
            delay(3000);

            while (1) 
            {
				adcValue = wiringPiI2CReadReg8(fd, PCF8591_ADDR | 0x40);
				printf("ADC Value_out: %d\n", adcValue);
        
				if (adcValue > LIGHT_THRESHOLD) 	
				{
					printf("Waiting for light threshold to drop...\n");
				} 
				else if (adcValue < LIGHT_THRESHOLD) 
				{
					printf("out of car");
					delay(3000);
					//setServoPosition(PWM_0_DEGREE);  
					break;  
				}
			}
            setServoPosition(PWM_0_DEGREE);
            Crossgate = 1;
            // Clear out_records.txt after processing
            clearFile("out_records.txt");
        } 
        else {
            Crossgate = 0;
        }
        delay(500); 
    }

    return 0;
}
