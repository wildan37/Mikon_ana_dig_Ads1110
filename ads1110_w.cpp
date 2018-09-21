//
//  Ads1110.cpp
//  i2cTBN
//
//  Created by Wildan Abdussalam on 6/30/15.
//  Copyright 2015 wildan. All rights reserved.

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "Ads1110.h"
using namespace std;

#define MAX_BUS 64
#define configReg 0x02
#define ads1110_MSB 0x00
#define ads1110_LSB 0x01

Ads1110::Ads1110(int bus, int address) {
    I2CBus = bus;
    I2CAddress = address;
  //  readAds1110Full();
}
int Ads1110::readAds1110Full(){

    cout << "Starting Ads1110 I2C ADC state read" << endl;
    char namebuf[MAX_BUS];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c/%d", I2CBus);
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
        cout << "Failed to open ADS1110 ADC on " << namebuf << " I2C Bus" << endl;
        return(1);
    }
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0){
        cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
        return(2);
    }

    int numberBytes = ADS1110_Buffer;
    int bytesRead = read(file, this->dataBuffer, numberBytes);
    if (bytesRead == -1){
        perror("Failure to read Byte Stream in readFull()");
    }
    close(file);
    this->digVoltage=convertVoltage(ads1110_MSB, ads1110_LSB);
    return 0;
}
int Ads1110::convertVoltage(int ads1110_Msb, int ads1110_Lsb){
    short data=dataBuffer[ads1110_Msb];
    data=(data<<8) | dataBuffer[ads1110_Lsb];
    return data;
}
void Ads1110::displayMode(int iterations){

    for(int i=0; i<iterations; i++){
        this->readAds1110Full();
        printf("Hasil Konversi (%4.2f)", digVoltage);
    }
}

Ads1110_SC Ads1110::getSC(){
   // this->readAds1110Full();
    char temp = dataBuffer[configReg];
    temp = temp & 0b00010000;
    temp = temp>>4;
    this->ConversionMode = (Ads1110_SC) temp;
    return this->ConversionMode;
}
Ads1110_PGA Ads1110::getPGA(){
  //  cout<<"starting to read PGA"<<endl;
   // this->readAds1110Full();
    char temp = dataBuffer[configReg];
    temp = temp & 0b0000011;
    this-> pgaamp= (Ads1110_PGA) temp;
    return this->pgaamp;
}

Ads1110_DR Ads1110::getDR(){
    //this->readAds1110Full();
    char temp = dataBuffer[configReg];
    temp = temp & 0b0001100;
    temp= temp>>2;
    this->drates= (Ads1110_DR) temp;
    return this->drates;
}

int Ads1110::setConfigReg(Ads1110_PGA pga, Ads1110_DR DRates, Ads1110_SC conversion){
    this->pgaamp=pga;
    this->drates=DRates;
    this->readAds1110Full();
    char current = dataBuffer[configReg];
    char temp = pga | (DRates<<2) | (conversion << 4); //move value into bits 4,3,2,1,0
    current = current & 0b11100000; //clear the current bits 4,3,2,1,0
    temp = current | temp;
    if(this->writeI2CDeviceByte(I2CAddress, temp)!=0){
        cout << "Failure to update Configuration value" << endl;
        return 1;
    }
    return 0;
}
float Ads1110::getConverVoltage(){
    int _pga;
    int _minData;
    //pga parsing
    if (pgaamp==0){
        _pga=1;}
    else if (pgaamp==1){
       _pga=2;
    }
    else if(pgaamp==2){
        _pga=4;
    }
    else{
        _pga=8;}
    // data rates parsing
    if (drates==0){
        _minData=2048;
    }
    else if (drates==1){
        _minData=8192;
    }
    else if (drates==2){
        _minData=16384;
    }
    else{
        _minData=32678;
    }
    return digVoltage*(2.048/(_minData*_pga))/20;

}



int Ads1110::writeI2CDeviceByte(char address, char value){

    cout << "Starting Ads1110 I2C converter state write" << endl;
    char namebuf[MAX_BUS];
   	snprintf(namebuf, sizeof(namebuf), "/dev/i2c/%d", I2CBus);
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
        cout << "Failed to open Ads1110 ADC on " << namebuf << " I2C Bus" << endl;
        return(1);
    }
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0){
        cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
        return(2);
    }


    char buffer[2];
    buffer[0] = address;
    buffer[1] = value;
    if ( write(file, buffer, 2) != 2) {
        perror("Failed to write to the i2c bus");
        return(3);
    }
    close(file);
    cout << "Finished Ads1110 I2C ADC state write" << endl;
    return 0;
}
Ads1110::~Ads1110() {
    // TODO Auto-generated destructor stub
}






