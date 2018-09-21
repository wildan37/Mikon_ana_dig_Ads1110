//
//  main.cpp
//  i2cTBN
//
//  Created by Wildan Abdussalam on 6/30/15.
//  Copyright (c) 2015 wildan. All rights reserved.
//

#include <stdlib.h>
#include <unistd.h>
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

int main() {
    Ads1110 Ads1110(0, 0x4c);
    //Ads1110.setConfigReg(pga_2, _15sps,continous_conversion);
    Ads1110.readAds1110Full();
    cout<<"Konfigurasi untuk PGA adalah : "<<Ads1110.getPGA()<<endl;
    cout<<"Konfigurasi untuk data rates adalah : "<<Ads1110.getDR()<<endl;
    cout<<"Konfigurasi untuk Conversion mode adalah : "<<Ads1110.getSC()<<endl;
    //Ads1110.setConfigReg(pga_2, _15sps,continous_conversion);
    cout<<"Hasil dari konversi Ads1110 adalah:" << Ads1110.getConverVoltage()<<" V"<<endl;

    return 0;


}
