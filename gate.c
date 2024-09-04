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

int hasNewTag(char *lastTag) {
    FILE *file = fopen("tag.txt", "r");
    if (!file) {
        printf("Error to open File\n");
        return 0;
    }

    char currentTag[100];
    int isNewTag = 0;

    while (fgets(currentTag, sizeof(currentTag), file) != NULL) {
		currentTag[strcspn(currentTag, "\n")] = 0;
		
        if (strcmp(currentTag, lastTag) != 0) {
            isNewTag = 1;
            strcpy(lastTag, currentTag); 
            break;
        }
    }

    fclose(file);
    return isNewTag;
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

    char lastTag[100] = "";
    int Crossgate = 0;

    while (1) {
        if (hasNewTag(lastTag) && Crossgate == 0) 
        {
			if(strstr(lastTag,"Entry") != NULL)
			{
				setServoPosition(PWM_90_DEGREE);
				delay(1000);
				setServoPosition(PWM_0_DEGREE);
				Crossgate = 1;
			}
			else if(strstr(lastTag, "Out") != NULL)
			{
				setServoPosition(PWM_90_DEGREE);
				delay(1000);
				setServoPosition(PWM_0_DEGREE);
				Crossgate = 1;
			}
        }
        else
        {
			Crossgate = 0;
		}
        delay(2000); 
    }

    return 0;
}
