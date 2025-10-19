#ifndef __STC32F12K_GPIO_H__
#define __STC32F12K_GPIO_H__

typedef enum {
    GPIO_P0 = 0,
    GPIO_P1,
    GPIO_P2, 
    GPIO_P3,
    GPIO_P4,
    GPIO_P5,
    GPIO_P6,
    GPIO_P7
} GPIO_Port;

typedef enum {
    GPIO_PIN_0 = 0x01,
    GPIO_PIN_1 = 0x02,
    GPIO_PIN_2 = 0x04,
    GPIO_PIN_3 = 0x08,
    GPIO_PIN_4 = 0x10,
    GPIO_PIN_5 = 0x20,
    GPIO_PIN_6 = 0x40,
    GPIO_PIN_7 = 0x80,
    GPIO_PIN_ALL = 0xFF
} GPIO_Pin;

typedef enum {
    GPIO_MODE_QUASI = 0,    // 准双向口
    GPIO_MODE_PUSHPULL,     // 推挽输出
    GPIO_MODE_INPUT,        // 高阻输入
    GPIO_MODE_OPENDRAIN     // 开漏输出
} GPIO_Mode;

void GPIO_Init(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode);
void GPIO_SetBits(sbit Pxy);
void GPIO_ResetBits(sbit Pxy);

#endif
