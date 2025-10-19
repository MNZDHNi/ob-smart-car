#include "stc32g.h"
#include "stc32f12k_gpio.h"

/**
  * @brief  GPIO初始化
  * @param  port: GPIO端口
  * @param  pin: GPIO引脚
  * @param  mode: GPIO工作模式
  * @retval 无
  */

void GPIO_Init(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode)
{
    unsigned char *pM0 = NULL, *pM1 = NULL;
    
    /* 获取模式配置寄存器地址 */
    switch(port) {
        case GPIO_P0: pM0 = &P0M0; pM1 = &P0M1; break;
        case GPIO_P1: pM0 = &P1M0; pM1 = &P1M1; break;
        case GPIO_P2: pM0 = &P2M0; pM1 = &P2M1; break;
        case GPIO_P3: pM0 = &P3M0; pM1 = &P3M1; break;
        case GPIO_P4: pM0 = &P4M0; pM1 = &P4M1; break;
        case GPIO_P5: pM0 = &P5M0; pM1 = &P5M1; break;
        case GPIO_P6: pM0 = &P6M0; pM1 = &P6M1; break;
        case GPIO_P7: pM0 = &P7M0; pM1 = &P7M1; break;
        default: return;
    }

    switch(mode) {
        case GPIO_MODE_QUASI:    // 准双向口: M1=0, M0=0
            *pM1 &= ~pin;
            *pM0 &= ~pin;
            break;
            
        case GPIO_MODE_PUSHPULL: // 推挽输出: M1=0, M0=1
            *pM1 &= ~pin;
            *pM0 |= pin;
            break;
            
        case GPIO_MODE_INPUT:    // 高阻输入: M1=1, M0=0
            *pM1 |= pin;
            *pM0 &= ~pin;
            break;
            
        case GPIO_MODE_OPENDRAIN: // 开漏输出: M1=1, M0=1
            *pM1 |= pin;
            *pM0 |= pin;
            break;
            
        default:
            break;
    }
}

void GPIO_SetBits(sbit Pxy)
{
    Pxy = 1;
}

void GPIO_ResetBits(sbit Pxy)
{
    Pxy = 0;
}
