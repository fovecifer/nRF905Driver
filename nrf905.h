#include<stdio.h>
#include<stdint.h>
#include<sys/time.h>
#include<wiringPi.h>
#include<wiringPiSPI.h>

//Write Configuration Register
#define WC      0x00
//Read Configuration Register
#define RC      0x10
//Write TX-payload
#define WTP     0x20
//Read TX-payload
#define RTP     0x21
//Write TX-address
#define WTA     0x22
//Read TX-Address
#define RTA     0x23
//Read RX-payload
#define RRP     0x24
			    
long getCurrentTime();
void nRF905_init();
void nRF905_send(uint8_t *data, int length, uint8_t *address);
int nRF905_recive(uint8_t *data, long timeout);
long getCurrentTime();
