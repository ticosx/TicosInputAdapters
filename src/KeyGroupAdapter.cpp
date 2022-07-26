#include "KeyGroupAdapter.h"

KeyGroupAdapter::KeyGroupAdapter(uint8_t keyNum) : InputAdapter() {
    this->keyNum = keyNum;
}

KeyGroupAdapter::~KeyGroupAdapter(){

}

void KeyGroupAdapter::readInputData(input_data *input){
    input->key = getKeyCode();
    input->dx = input->dy = input->x = input->y = -1;
    input->state = TC_UNDEF;
}


