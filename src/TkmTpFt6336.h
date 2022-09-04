#ifndef TKM_TP_FT6336_h
#define TKM_TP_FT6336_h

#include <Arduino.h>
#include "lvgl.h"
#include "LvglInputAdapter.h"
#include "Log.h"
#include <Wire.h>

#define FT6336G_ADDR  0x38

#define FT6336G_DEVIDE_MODE 0x00
#define FT6336G_REG_NUM_FINGER  0x02
#define FT6336G_TP1_REG         0X03
#define FT6336G_TP2_REG         0X09
#define FT6336G_TP3_REG         0X0F
#define FT6336G_TP4_REG         0X15
#define FT6336G_TP5_REG         0X1B
#define	FT6336G_ID_G_LIB_VERSION  0xA1
#define FT6336G_ID_G_MODE       0xA4
#define FT6336G_ID_G_THGROUP    0x80
#define FT6336G_ID_G_PERIODACTIVE 0x88
#define FTS_REG_CHIP_ID         0xA3

#define READ_INTERVAL   2

/*!
 * @brief FT6336G 触摸屏
 */
class TkmTpFt6336 : public LvglInputAdapter {

  public:
    TkmTpFt6336(TwoWire *i2c, uint8_t rstPin, uint16_t width, uint16_t height);
    virtual ~TkmTpFt6336();
    static TkmTpFt6336* getInstance() {return instance;};
    bool init();
    void reset();
    void setRotation(uint8_t rotation);

  protected:
    static TkmTpFt6336* instance;
    static void read(lv_indev_drv_t *, lv_indev_data_t *);

    uint8_t rstPin;
    uint8_t rotation;
    uint16_t width;
    uint16_t height;

  private:
    void _read(lv_indev_drv_t *, lv_indev_data_t *);
    bool inited = false;
};
#endif // TKM_TP_FT6336_h