#ifndef TK_Adc_Key_Group_h
#define TK_Adc_Key_Group_h

#include <Arduino.h>


/*!
 * @brief 单个 ADC-键值 对应关系
 */
typedef struct
{
    int keyCode;
    uint16_t regionBot;
    uint16_t regionTop;
}adcRegion;

/*!
 * @brief ADC-键值 对应表
 * @note  ADC表和键值表的长度应等于键的个数
 */
typedef struct
{
    uint8_t keyNum;
    adcRegion *adcRegions;
}AdcKeyTable;


/*!
 * @brief 使用一个ADC口变量起来的键盘
 */
class TK_AdcKeyGroup {
public:
    TK_AdcKeyGroup(int adcPin);
    TK_AdcKeyGroup(int adcPin,AdcKeyTable *table);
    ~TK_AdcKeyGroup();
    bool addKey(adcRegion *region);     //此方法主要用于支持Ticos下可变节点输入 adc-键值 对应关系
    int getKeyCode();
protected:
    AdcKeyTable table;
    int adcPin;
};

#endif // TK_Adc_Key_Group_h
