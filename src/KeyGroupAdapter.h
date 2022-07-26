#ifndef Key_Group_Adapter_h
#define Key_Group_Adapter_h

#include "InputAdapter.h"

/*!
 * @brief 键盘适配器
 */
class KeyGroupAdapter : public InputAdapter{
public:
    KeyGroupAdapter(uint8_t keyNum);
    ~KeyGroupAdapter();
    virtual int getKeyCode() = 0;
    virtual void readInputData(input_data* input) ;
protected:
    uint8_t keyNum;
};

#endif //Key_Group_Adapter_h

