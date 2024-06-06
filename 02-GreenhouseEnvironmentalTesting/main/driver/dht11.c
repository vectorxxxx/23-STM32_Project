#include "dht11.h"

extern TIM_HandleTypeDef htim1;

DHT11_DATA DHT11_Data;

/**
  * @brief 微秒级延时函数
  * @param  微妙数
  * @retval 无
  */
void Delay_us(u8 us)
{
    u32 differ = 0xffff - us - 5;
    __HAL_TIM_SET_COUNTER(&htim1, differ);
    HAL_TIM_Base_Start(&htim1);
    
    while (differ < 0xffff - 5)
    {
        differ = __HAL_TIM_GET_COUNTER(&htim1);
    }
    HAL_TIM_Base_Stop(&htim1);
}

/**
  * @brief 输出模式
  * @param  flag 设置电平 
  * @retval 无
  */
void DHT11_OUTPUT(u8 flag)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = DATA_GPIO_Pin;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DATA_GPIO_Port, &GPIO_InitStruct);
    
    if (flag == 1)
    {
        DATA_SET();
    }
    else 
    {
        DATA_RESET(); 
    }
}

/**
  * @brief 输入模式
  * @param  无
  * @retval 读取到的电平
  */
u8 DHT11_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = DATA_GPIO_Pin;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DATA_GPIO_Port, &GPIO_InitStruct);
     
    return DATA_READ() == GPIO_PIN_SET ? 1 : 0;
}

/**
  * @brief 读取一个字节数据
  * @param  无
  * @retval 返回读取到的数据
  */
u8 DHT11_ReadByte(void)
{
    u8 ReadData = 0;
    u8 temp = 0;
    u8 retry = 0;
    u8 i = 0;
    
    for (i = 0; i < 8; i++)
    {
        // 读取低电平
        while (DATA_READ() == GPIO_PIN_RESET && retry < 100)
        {
            Delay_us(1);
            retry++;
        }
        retry = 0;
        // 因为高电平的长短决定了数据位是0还是1，高电平26us-28us为0，70us为1，这里选在 26us-28us ~ 70us 之间即可判断
        Delay_us(40);
        
        // 如果在 40us 之后为高电平，说明接收到的数据为 1
        if (DATA_READ() == GPIO_PIN_SET)
        {
            temp = 1;
        }
        // 如果在 40us 之后为低电平，说明接收到的数据为 0
        else 
        {
            temp = 0;
        }
        
        // 如果仍为高电平，则等待高电平结束，否则跳过
        while (DATA_READ() == GPIO_PIN_SET && retry < 100)
        {
            Delay_us(1);
            retry++;
        }
        retry = 0;
        
        // 因为数据是高位先出，这里左移后进行或运算
        ReadData <<= 1;
        ReadData |= temp;
    }
    
    return ReadData;
}

/**
  * @brief 读取数据
  * @param  无
  * @retval 读取状态位
  */
u8 DHT11_Read(void)
{
    u8 retry = 0;
    u8 i = 0;
    
    // 总线空闲状态为高电平,主机把总线拉低等待DHT11响应,主机把总线拉低必须大于18毫秒,保证DHT11能检测到起始信号。
    DHT11_OUTPUT(0);
    HAL_Delay(18);
    DATA_SET();

    // 主机发送开始信号结束后,延时等待20-40us后, 读取DHT11的响应信号
    Delay_us(20);
    DHT11_INPUT();
    Delay_us(20);
    
    if (DATA_READ() == GPIO_PIN_RESET)
    {
        // 总线为低电平,说明DHT11发送响应信号
        while(DATA_READ() == GPIO_PIN_RESET && retry < 100)
        {
            Delay_us(1);
            retry++;
        }
        retry = 0;
        // DHT11发送响应信号后,再把总线拉高80us,准备发送数据
        while(DATA_READ() == GPIO_PIN_SET && retry < 100)
        {
            Delay_us(1);
            retry++;
        }
        retry = 0;
        
        for (i = 0; i < 5; i++)
        {
            DHT11_Data.DATA[i] = DHT11_ReadByte();
        }
        // 当最后一bit数据传送完毕后，DHT11拉低总线50us,随后总线由上拉电阻拉高进入空闲状态。
        Delay_us(50);
    }
    
    // 数据传送正确时校验和数据等于“ 8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据” 所得结果的末8位
    u8 checksum = DHT11_Data.DATA[0] + DHT11_Data.DATA[1] + DHT11_Data.DATA[2] + DHT11_Data.DATA[3];
    if (checksum == DHT11_Data.DATA[4])
    {
        DHT11_Data.humidity = DHT11_Data.DATA[0];
        DHT11_Data.temperature = DHT11_Data.DATA[2];
        return 1;
    }
    else 
    {
        return 0;
    }
}

void DHT11_Task(void)
{
    if (DHT11_Read() == 1)
    {
        DHT11_Data.index++;
        if (DHT11_Data.index >= 128)
        {
            DHT11_Data.index = 0;
        }
    }
}
