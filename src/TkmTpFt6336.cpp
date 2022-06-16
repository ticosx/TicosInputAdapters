#include "TkmTpFt6336.h"
#include "HwHelper.h"

TkmTpFt6336 *TkmTpFt6336::instance = NULL;
static const uint8_t FT5206_TPX_TBL[5] = {FT6336G_TP1_REG, FT6336G_TP2_REG, FT6336G_TP3_REG, FT6336G_TP4_REG, FT6336G_TP5_REG};

TkmTpFt6336::TkmTpFt6336(TwoWire *i2c, uint8_t rstPin, uint16_t width, uint16_t height)
    : LvglInputAdapter(i2c, FT6336G_ADDR), rstPin(rstPin), width(width), height(height)
{
  // Release the origin instance
  if (instance != NULL)
  {
    delete (instance);
  }
  input_cb = TkmTpFt6336::read;
  instance = this;
  init();
}

TkmTpFt6336::~TkmTpFt6336()
{
  instance = NULL;
}

void TkmTpFt6336::reset()
{
  digitalWrite(rstPin, 0);
  delay(20);
  digitalWrite(rstPin, 1);
}

bool TkmTpFt6336::init()
{
  // reset();
  delay(50);

  uint8_t reg = 0;
  //正常操作模式
  I2C_WRITE_REG(FT6336G_DEVIDE_MODE, reg);
  //查询模式
  I2C_WRITE_REG(FT6336G_ID_G_MODE, reg);
  //触摸有效值，越小越灵敏
  reg = 22;
  I2C_WRITE_REG(FT6336G_ID_G_THGROUP, reg);
  //激活周期
  reg = 12;
  I2C_WRITE_REG(FT6336G_ID_G_PERIODACTIVE, reg);
  uint8_t data[2];
  //读取版本号
  // TODO: 现在读不到判断 TP 的有意义的值，考虑调整
  if (I2C_READ_ARRAY(FT6336G_ID_G_LIB_VERSION, data))
  {
    logInfo("TP ID:%x\r\n", (((uint16_t)data[0]) << 8) + data[1]);
  }
  else
  {
    logErr("Read TP I2C wrong\r\n");
    return false;
  }
  // if ((data[0] == 0X30 && data[1] == 0X03) || data[1] == 0X01 || data[1] == 0X02)
  // {
  //   logErr("TP ID:%x\r\n", ((uint16_t)data[0] << 8) + data[1]);
  //   return false;
  // }
  inited = true;
  return true;
}

void TkmTpFt6336::read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (getInstance() != NULL)
  {
    getInstance()->_read(indev_driver, data);
  }
}

void TkmTpFt6336::_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

  uint8_t buf[4];
  uint8_t res = 0;
  uint8_t temp;
  static uint16_t x[6], y[6];
  static uint8_t filter = 0;
  static uint8_t mode = 0;
  if (!inited)
  {
    // 尚未成功初始化
    return;
  }
  //空闲时降低读取频率
  if ((filter++ % READ_INTERVAL) == 0 || filter < READ_INTERVAL)
  {
    //读取触摸点的状态
    if (I2C_READ_REG(FT6336G_REG_NUM_FINGER, mode))
    {
      //读到些结果
      if ((mode & 0XF) && ((mode & 0XF) < 6))
      {
        //将点的个数转换为1的位数
        temp = 0XFF << (mode & 0XF);
        temp = ~temp;
        for (uint8_t i = 0; i < 5; i++)
        {
          //该点按下
          if (temp & (1 << i))
          {
            //读取该点坐标
            if (I2C_READ_ARRAY(FT5206_TPX_TBL[i], buf))
            {
              if (width >= height)
              {
                y[i] = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
                if(y[i]>=height){
                  y[i] = height - 1;
                }
                y[i] = height - 1 - y[i];
                x[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
              }
              else
              {
                // x[i] = width - (((uint16_t)(buf[0] & 0X0F) << 8) + buf[1]);
                x[i] = (((uint16_t)(buf[0] & 0X0F) << 8) + buf[1]);
                y[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
              }
              if ((buf[0] & 0XF0) != 0X80)
              {
                //不是contact事件，无效
                x[i] = y[i] = 0;
              }
            }
            else
            {
              logErr("Read TP point data error for mode %d in index %d\r\n", mode, i);
            }
          }
        }
        res = 1;
        if (x[0] == 0 && y[0] == 0)
        {
          //无效数据
          mode = 0;
        }
        //重置扫描标志
        filter = 0;
      }
    }
    else
    {
      logErr("Read TP data error\r\n");
    }
  }
  if ((mode & 0X3F) == 0)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = x[0];
    logVerbose("x :%d\r\n", x[0]);
    data->point.y = y[0];
    logVerbose("y :%d\r\n", y[0]);
  }
  if (filter >= 200)
  {
    filter = READ_INTERVAL;
  }
}