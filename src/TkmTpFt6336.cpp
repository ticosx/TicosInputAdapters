#include "TkmTpFt6336.h"

TkmTpFt6336* TkmTpFt6336::instance = NULL;

TkmTpFt6336::TkmTpFt6336(Adafruit_I2CDevice * i2c) : LvglInputAdapter(i2c) {
  // Release the origin instance
  if(instance!=NULL){
    delete(instance);
  }
  input_cb = TkmTpFt6336::read;
  instance = this;
}

TkmTpFt6336::~TkmTpFt6336(){
}

void TkmTpFt6336::init(){

}

void TkmTpFt6336::read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data){
    uint16_t touchX, touchY;

    bool touched = true;// TODO: Get touch info from device

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}