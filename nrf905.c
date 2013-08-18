#include<stdio.h>
#include<stdint.h>
#include<sys/time.h>
#include<wiringPi.h>
#include<wiringPiSPI.h>

#include"nrf905.h"

int TX_EN = 0;
int TRX_CE = 1;
int PWR_UP = 2;
int DR = 3;

void nRF905_init(){
    if (wiringPiSetup () == -1)
    pinMode(TX_EN, OUTPUT);
    pinMode(TRX_CE, OUTPUT);
    pinMode(PWR_UP, OUTPUT);
    pinMode(DR, INPUT);

    uint8_t RFConf[11]=
    {
        0x00,
        0x4c,
        0x0c,
	0x44,
	0x08,0x08,
        0xcc,0xcc,0xcc,0xcc,
	0x58
    };

    //Init nRF905
    pinMode(DR, OUTPUT);
    digitalWrite(DR, 0);
    pinMode(DR, INPUT);
    digitalWrite(PWR_UP, 1);
    digitalWrite(TRX_CE, 0);
    digitalWrite(TX_EN, 0);
    if(wiringPiSPISetup(0, 16000000) == -1){
        printf("Could not initialise SPI\n");
    }

    wiringPiSPIDataRW(0, RFConf, 11);
}

void nRF905_send(uint8_t *data, int length, uint8_t *address){
    //Set Tx Mode
    digitalWrite(TRX_CE, 0);
    digitalWrite(TX_EN, 1);
    delay(1);

    uint8_t buff[32];
    int i;
    // Tx Packet
    buff[0] = WTP;
    for(i = 1; i < 9; i++){
        buff[i] = data[i-1];
    }
    wiringPiSPIDataRW(0, buff, length + 1);
    delay(1);
     
    buff[0] = WTA;
    for(i = 1; i < 5; i++){
        buff[i] = address[i-1];
    }
    wiringPiSPIDataRW(0, buff, 5);
    digitalWrite(TRX_CE, 1);
    while(digitalRead(DR) != 1);
    digitalWrite(TRX_CE, 0);
}

int nRF905_recive(uint8_t *data, long timeout){
    //Set Rx Mode
    digitalWrite(TRX_CE, 1);
    digitalWrite(TX_EN, 0);
    delay(1);
    long t1 = getCurrentTime();
    while(digitalRead(DR) == 0){
        if((getCurrentTime() - t1) > timeout){
	    return 0;
	}
    }
    digitalWrite(TRX_CE, 0);
    uint8_t buff9[9];
    buff9[0] = RRP;
    wiringPiSPIDataRW(0, buff9, 9);
    int i;
    for(i = 0; i < 8; i++){
        data[i] = buff9[i+1];
    }
    digitalWrite(TRX_CE, 1);
    return 8;
}

long getCurrentTime(){    
    struct timeval tv;    
    gettimeofday(&tv,NULL);    
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
}

