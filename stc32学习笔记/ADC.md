# 一、STC32G12K128 ADC特性

## 主要技术参数

|参数|规格|
|---|---|
|**分辨率**|12位（0-4095）|
|**转换通道**|15个外部通道 + 内部通道|
|**转换速度**|最高30万次/秒（3.3V系统）|
|**参考电压**|内部1.19V/2.4V/外部Vref|
|**触发方式**|软件触发/定时器触发/PWM触发|

## ADC通道分配

| 通道   | 引脚        | 特殊功能      |
| ---- | --------- | --------- |
| 0-7  | P1.0-P1.7 | 通用ADC输入   |
| 8-14 | P0.0-P0.6 | 通用ADC输入   |
| 15   | 内部        | 1.19V基准电压 |
| 16   | 内部        | 温度传感器     |
| 17   | 内部        | GND       |

# 二、ADC工作原理

## 转换过程

| 模拟输入 | 采样保持 | 逐次逼近 | 数字输出  |
| ---- | ---- | ---- | ----- |
| 电压信号 | 保持稳定 | 比较判断 | 12位数字 |

### 分辨率与量化

**12位分辨率**意味着：

- 可以区分 2¹² = 4096 个不同的电平
- 量化精度 = 参考电压 / 4096
- 例如：3.3V参考电压时，精度 = 3.3V / 4096 ≈ 0.8mV

# 四、ADC相关寄存器详解

## 1. ADC控制寄存器（ADC_CONTR）

|位|7|6|5|4|3|2|1|0|
|---|---|---|---|---|---|---|---|---|
|功能|ADC_POWER|ADC_START|ADC_FLAG|SPEED1|SPEED0|-|-|-|

- **ADC_POWER**：ADC电源控制（1=开启，0=关闭）
- **ADC_START**：启动转换（写1启动，转换完成自动清零）
- **ADC_FLAG**：转换完成标志（1=转换完成，需软件清零）
- **SPEED[1:0]**：转换速度控制

## 2. ADC配置寄存器（ADC_CFG）

|位|7|6|5|4|3|2|1|0|
|---|---|---|---|---|---|---|---|---|
|功能|-|-|-|RESFMT|-|-|-|-|

- **RESFMT**：结果对齐方式（0=右对齐，1=左对齐）

## 3. 通道选择寄存器（ADC_CHS）

| 位   | 7   | 6   | 5   | 4   | 3    | 2    | 1    | 0    |
| --- | --- | --- | --- | --- | ---- | ---- | ---- | ---- |
| 功能  | -   | -   | -   | -   | CHS4 | CHS3 | CHS2 | CHS1 |

- **CHS[4:0]**：选择ADC通道（0-17）

## 4. 结果寄存器（ADC_RES, ADC_RESL）

- **ADC_RES**：转换结果高8位
- **ADC_RESL**：转换结果低4位

**数据对齐方式**：

```c
// 右对齐（RESFMT=0）
result = (ADC_RES << 4) | ADC_RESL;  // 12位结果

// 左对齐（RESFMT=1）  
result = (ADC_RES << 8) | ADC_RESL;  // 高8位在ADC_RES
```

## 5. 采样时间控制（ADCTIM）

控制采样保持时间，影响转换精度。

# 五、ADC配置与使用

## 基本配置步骤

1. **开启ADC电源**
2. **配置ADC参数**（速度、对齐方式等）
3. **选择ADC通道**
4. **启动转换**
5. **等待转换完成**
6. **读取转换结果**
7. **关闭ADC电源**（可选，省电）

## 查询方式ADC读取

```c
#include "stc32g.h"

// ADC初始化函数
void ADC_Init(void)
{
    // 1. 开启ADC电源
    ADC_CONTR = 0x80;  // ADC_POWER = 1
    
    // 2. 配置ADC参数
    ADC_CFG = 0x00;    // 右对齐，默认设置
    
    // 3. 设置采样时间（可选，根据需求调整）
    ADCTIM = 0x3F;     // 默认采样时间
    
    // 延时等待ADC稳定
    Delay_ms(1);
}

// 读取指定通道的ADC值（12位，0-4095）
unsigned int ADC_Read(unsigned char channel)
{
    unsigned int result;
    
    // 1. 选择ADC通道
    ADC_CHS = channel;  // 设置要读取的通道
    
    // 2. 启动转换
    ADC_CONTR |= 0x40;  // ADC_START = 1
    
    // 3. 等待转换完成
    while (!(ADC_CONTR & 0x20));  // 等待ADC_FLAG=1
    
    // 4. 清除转换标志
    ADC_CONTR &= ~0x20;  // ADC_FLAG = 0
    
    // 5. 读取转换结果（右对齐）
    result = (ADC_RES << 4) | ADC_RESL;
    
    return result;
}

// 将ADC值转换为电压值（单位：mV）
unsigned int ADC_To_Voltage(unsigned int adc_value, unsigned int ref_voltage)
{
    // 电压(mV) = (ADC值 × 参考电压(mV)) / 4096
    return (adc_value * ref_voltage) / 4096;
}

void main()
{
    unsigned int adc_value, voltage;
    
    // 初始化
    ADC_Init();
    
    while(1)
    {
        // 读取通道0的ADC值
        adc_value = ADC_Read(0);
        
        // 转换为电压值（假设参考电压3.3V）
        voltage = ADC_To_Voltage(adc_value, 3300);
        
        // 可以在这里处理或显示电压值
        printf("ADC值: %d, 电压: %d mV\r\n", adc_value, voltage);
        
        Delay_ms(1000);
    }
}
```

## 中断方式ADC读取

```c
#include "stc32g.h"

// 全局变量存储ADC结果
volatile unsigned int adc_result = 0;
volatile bit adc_ready = 0;

// ADC初始化（中断方式）
void ADC_Init_Interrupt(void)
{
    // 1. 开启ADC电源
    ADC_CONTR = 0x80;  // ADC_POWER = 1
    
    // 2. 配置ADC参数
    ADC_CFG = 0x00;    // 右对齐
    
    // 3. 设置采样时间
    ADCTIM = 0x3F;
    
    // 4. 开启ADC中断
    EADC = 1;          // 使能ADC中断
    EA = 1;            // 开启总中断
    
    Delay_ms(1);
}

// 启动ADC转换（中断方式）
void ADC_Start_Conversion(unsigned char channel)
{
    ADC_CHS = channel;      // 选择通道
    ADC_CONTR |= 0x40;     // 启动转换
}

// ADC中断服务函数
void ADC_ISR() interrupt 5
{
    // 读取转换结果
    adc_result = (ADC_RES << 4) | ADC_RESL;
    adc_ready = 1;         // 设置数据就绪标志
    
    // 清除中断标志（ADC_FLAG）
    ADC_CONTR &= ~0x20;
}

void main()
{
    ADC_Init_Interrupt();
    
    // 启动第一次转换
    ADC_Start_Conversion(0);
    
    while(1)
    {
        if(adc_ready)
        {
            adc_ready = 0;
            
            // 处理ADC数据
            unsigned int voltage = ADC_To_Voltage(adc_result, 3300);
            printf("电压: %d mV\r\n", voltage);
            
            // 启动下一次转换
            ADC_Start_Conversion(0);
        }
        
        // 主循环可以执行其他任务
        Delay_ms(100);
    }
}
```
