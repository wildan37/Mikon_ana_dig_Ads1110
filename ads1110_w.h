//
//  Ads1110.h
//  i2cTBN
//
//  Created by Wildan Abdussalam on 6/30/15.
//  Copyright (c) 2015 wildan. All rights reserved.


#ifndef _Ads1110__
#define _Ads1110__

#include <stdio.h>
#define ADS1110_Buffer 0x03

//For more details about register, see Ads1110 datasheet.
enum Ads1110_PGA{ //programmable gain amplifier, bits 0,1
    pga_1 = 0,
    pga_2 = 1,
    pga_4 = 2,
    pga_8 = 3
};
enum Ads1110_DR{ //data rates , bits 2,3
    _240sps = 0,
    _60sps = 1,
    _30sps = 2,
    _15sps = 3
};
enum Ads1110_SC{  // conversion modes : single conversion/continous conversion bits 4
    single_conversion = 1,
    continous_conversion = 0
};

class Ads1110{

private:
    int I2CBus, I2CAddress;
    char dataBuffer[ADS1110_Buffer];
    //float convertedVolt;
    Ads1110_SC ConversionMode;
    Ads1110_DR drates;
    Ads1110_PGA pgaamp;
    double digVoltage;
    int writeI2CDeviceByte(char address, char value);
    //char readI2CDeviceByte(char address);


public:
    Ads1110(int bus, int address);

    int readAds1110Full();
    int convertVoltage(int ads1110_MSB, int ads1110_LSB);
    float getConverVoltage();
    int setConfigReg(Ads1110_PGA pga, Ads1110_DR DRates, Ads1110_SC conversion);
    Ads1110_SC getSC();
    Ads1110_PGA getPGA();
    Ads1110_DR getDR();
    virtual ~Ads1110();
    void displayMode(int iterations);
};

#endif
