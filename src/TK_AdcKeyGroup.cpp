#include "TK_AdcKeyGroup.h"
#include "Log.h"

//extern ticos::TimeMs ticos::transactionTime();  //Ticos Luban public API

TK_AdcKeyGroup::TK_AdcKeyGroup(int adcPin){
    this->adcPin = adcPin;
    table.keyNum = 0;
    table.adcRegions = NULL;
}

TK_AdcKeyGroup::TK_AdcKeyGroup(int adcPin,AdcKeyTable *table){
    //复制adc-键值表
    if(table->keyNum){
        this->table.adcRegions = new adcRegion[table->keyNum];
        this->table.keyNum = table->keyNum;
        for(int temp=0; temp < table->keyNum; temp++){
            this->table.adcRegions[temp] = table->adcRegions[temp];
        }
    }else{
        this->table.adcRegions = NULL;
    }
    this->adcPin = adcPin;
}

TK_AdcKeyGroup::~TK_AdcKeyGroup(){
    if(table.adcRegions != NULL){
        delete table.adcRegions;
    }
}

bool TK_AdcKeyGroup::addKey(adcRegion *region){
    if(table.adcRegions){
        adcRegion *temp = new adcRegion[table.keyNum+1];
        for(int t=0;t<table.keyNum;t++){
            temp[t] = table.adcRegions[t];
        }
        temp[table.keyNum] = *region;
        table.keyNum++;
        delete table.adcRegions;
        table.adcRegions = temp;
    }else{
        table.adcRegions = new adcRegion;
        *table.adcRegions = *region;
        table.keyNum = 1;
    }
    return true;
}

int TK_AdcKeyGroup::getKeyCode(){
    uint16_t adcValue = analogRead(adcPin);
    int keyCode = -1;
    //logDebug("%d",adcValue);
    //遍历表
    for(int temp=0; temp < table.keyNum; temp++){
        if(adcValue >= table.adcRegions[temp].regionBot && adcValue <= table.adcRegions[temp].regionTop ){
            keyCode = table.adcRegions[temp].keyCode;
            //lastPressTime = transactionTime();
            break;
        }
        //logDebug("%d adcRegions:%d ~ %d",table.adcRegions[temp].keyCode,table.adcRegions[temp].regionBot,table.adcRegions[temp].regionTop);
    }
    return keyCode;
}



