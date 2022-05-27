#ifndef TKM_TP_FT6336_h
#define TKM_TP_FT6336_h

#include <Arduino.h>
#include "lvgl.h"
#include "LvglInputAdapter.h"

/*!
 * @brief The XRenderer of 16x16 LED screen
 */
class TkmTpFt6336 : public LvglInputAdapter {

  public:
    TkmTpFt6336(Adafruit_I2CDevice *);
    virtual ~TkmTpFt6336();
    static TkmTpFt6336* getInstance() {return instance;};

  protected:
    static TkmTpFt6336* instance;
    static void read(lv_indev_drv_t *, lv_indev_data_t *);

  private:
    void init();
};
#endif // TKM_TP_FT6336_h