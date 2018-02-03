/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vinayak
 *
 * Created on 12 June, 2017, 3:45 PM
 */
#include <stdio.h>
#include <cstdlib>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define OUTPUTPIN1   26
#define OUTPUTPIN2   27
#define OUTPUTPIN3   28
#define OUTPUTPIN4   29

/*
 * 
 */ 
int main(int argc, char** argv) {
    char buffer[1024],*pointer,*nextpointer;
    long SensorTimes[4][10000];
    int SensorTimeSizes[4],sensorpointer[4];
    bool SensorStates[4];
    FILE *input;
    size_t size = 0;
    int sensor_number=-1,valuecount=0;
    
    long starttime=0;
    if(argc<2){
        printf("No ride file attached, exiting\n");
        
                input=fopen("~/logs/log225/ride10","r");
    }else{
    input=fopen(argv[1],"r");
    }
    if(argc>2){
        sscanf(argv[2],"%ld",&starttime);
        printf("Starting at %f seconds",(float)starttime/(float)1000000);
    }
    
    if (wiringPiSetup() == -1) {
        printf("Could not initialize wiring pi\n");
        return 1;
    }
    SensorStates[0]=!digitalRead(OUTPUTPIN1);
    SensorStates[1]=!digitalRead(OUTPUTPIN2);
    SensorStates[2]=!digitalRead(OUTPUTPIN3);
    SensorStates[3]=!digitalRead(OUTPUTPIN4);
    pinMode(OUTPUTPIN1,OUTPUT);
    pinMode(OUTPUTPIN2,OUTPUT);
    pinMode(OUTPUTPIN3,OUTPUT);
    pinMode(OUTPUTPIN4,OUTPUT);
        pointer=buffer;
    sprintf(pointer, "test\n");
    while (!feof(input)) {
        pointer = buffer;
        getline(&pointer,&size,input);
        if (*pointer == 'S') {
            if (sensor_number>-1){
                SensorTimeSizes[sensor_number]=valuecount+1;
            }
            sscanf(pointer, "Senor Log For Sensor %d", &sensor_number);
            printf("Reading logs for sensor %d\n",sensor_number);
            sensor_number--;
            valuecount = 0;
        } else if (sensor_number>-1) {
            sscanf(pointer, "%ld", &(SensorTimes[sensor_number][valuecount++]));
        } else {
            printf("error in parsing %s", pointer);
            return 1;
        }
    }
                SensorTimeSizes[sensor_number]=valuecount+1;
    printf("Starting\n");
    for(int i=0;i<4;i++){
        printf("Found %d values for sensor %d\n",SensorTimeSizes[i],i+1);
    }
        for(int i=0;i<4;i++){
         sensorpointer[i]=0;   
        }
        bool finished=false;
        long previousprinttime=0;
        starttime=micros()-starttime;
    while(!finished){
        finished=true;
        long time=micros()-starttime;
        if((time-previousprinttime)>1000000){
            
        for(int i=0;i<4;i++){
            printf("Sensor %d reached %d, with time %ld\n",i,sensorpointer[i],SensorTimes[i][sensorpointer[i]]);
        }
           previousprinttime=time;
        }
        for(int i=0;i<4;i++){
            if((SensorTimeSizes[i]>sensorpointer[i])){
                if(SensorTimes[i][sensorpointer[i]]<time){
                    sensorpointer[i]++;
                    switch(i){
                        case 0:
                            SensorStates[i]=!SensorStates[i];
                            digitalWrite(OUTPUTPIN1,SensorStates[i]);
                            break;
                        case 1:
                            SensorStates[i]=!SensorStates[i];
                            digitalWrite(OUTPUTPIN2,SensorStates[i]);
                            break;
                        case 2:
                            SensorStates[i]=!SensorStates[i];
                            digitalWrite(OUTPUTPIN3,SensorStates[i]);
                            break;
                        case 3:
                            SensorStates[i]=!SensorStates[i];
                            digitalWrite(OUTPUTPIN4,SensorStates[i]);
                            break;
                    }
                }
                finished=false;
            }
        }
        //usleep(20);
    }
    return 0;
}

