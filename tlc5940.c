#include "stm8s.h"
#include "tlc5940.h"
uint16_t leds[15];
extern uint16_t clkCnt;
extern uint16_t currentDelay;
void TLC5940_init()
{
  /*****Init Pin *****/
  TLC_BLANK_INIT
  TLC_XLAT_INIT
  TLC_SCLK_INIT
  TLC_GSCLK_INIT
  
  /*****SPI Init******/
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_32 ,SPI_MODE_MASTER,SPI_CLOCKPOLARITY_LOW,SPI_CLOCKPHASE_1EDGE,SPI_DATADIRECTION_1LINE_TX,SPI_NSS_SOFT,0);
  
  
  /****Timer Init ****/
  TIM1_TimeBaseInit(71,TIM1_COUNTERMODE_UP,3,0); //Настраиванем таймер
  TIM1_InternalClockConfig(); //Внутрений источник тактирования
  TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); //Включить прерывание
  TIM1_ARRPreloadConfig(ENABLE);  
  TIM1_OC3Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,(u16)2,TIM1_OCPOLARITY_HIGH,TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_RESET,TIM1_OCNIDLESTATE_SET); //Настроить ШИМ
  TIM1_Cmd(ENABLE); //Включить таймер
  TIM1_CtrlPWMOutputs(ENABLE); //Включить ШИМ
  
  /** Включить SPI **/
  SPI_Cmd(ENABLE);
  
  /** Отключить ПИН **/
  TLC_XLAT_OFF //xlat off
  TLC_BLANK_OFF //blank off
     
}
/**** Установить значение ШИМ ****/
void setTLCChannel(uint8_t channel, uint16_t val)
{
    leds[channel] = (val & 0x0FFF);
}

/** Сбросить настройки **/
void resetTLC()
{
    uint8_t i;
    for (i = 0; i < 16; i++)
    {
        leds[i] = 0x0000;
    }
    updateTLC();
}

//***Записать в микросхему байт**//
void writeTLC(uint8_t data)
{
    SPI->DR=data; //Запись данных
    while ((SPI->SR & (uint8_t)SPI_FLAG_TXE) == (uint8_t)RESET); //Ждём
}
void updateTLC()
{ 
    int8_t i;
    //Ожидание конца цикла
    while (clkCnt);
    //Выключить таймер
    TIM1_Cmd(DISABLE);
    TLC_BLANK_ON //Включить BLANK
    delay(1);
    //Отправляем Данные 
    for (i = 15; i >= 0; i-=2)
    {
        uint8_t send1 = 0;
        uint8_t send = leds[i] >> 4;
        writeTLC(send);

        send = (leds[i] & 0x000F);
        send <<= 4;
        send1 = (leds[i-1]) >> 8;

        send |= send1;
        writeTLC(send);

        send = leds[i-1];
        writeTLC(send);
    }
    
    //Дёргаем XLAT
    TLC_XLAT_ON //xlat on
    delay(1);
    TLC_XLAT_OFF //xlat off
    
    //Выулючаем BLANK
    TLC_BLANK_OFF //blank off
    delay(1);
    clkCnt = 0;
    
    //Включаем Таймер
    TIM1_Cmd(ENABLE);
    

}


/*** Handler stm8s_it.c ***/
/*
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
    TIM1->SR1 = (uint8_t)(~(uint8_t)TIM1_IT_UPDATE); //Сбрасываем таймер
    clkCnt++;
    if (clkCnt == 4095)
    {
        TLC_BLANK_ON
        return;
    }
    else if (clkCnt == 4096)
    {
        TLC_BLANK_OFF
        clkCnt = 0;
    }
    
}
*/