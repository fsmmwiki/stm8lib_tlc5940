//Pin Xlat B4
//Pin Blank B5
//Pin MISO C6
//Pin SCLK C5
//Pin GSCLK C3
void TLC5940_init();
void setTLCChannel(uint8_t channel, uint16_t val);
void resetTLC();
void writeTLC(uint8_t data);
void updateTLC();
uint16_t  readdel();

#define TLC_BLANK_ON GPIOB->ODR |= (uint8_t)(GPIO_PIN_5);
#define TLC_BLANK_OFF GPIOB->ODR &= (uint8_t)(~GPIO_PIN_5);
#define TLC_XLAT_OFF GPIOB->ODR &= (uint8_t)(~GPIO_PIN_4);
#define TLC_XLAT_ON GPIOB->ODR |= (uint8_t)GPIO_PIN_4;

#define TLC_BLANK_INIT GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST); //
#define TLC_XLAT_INIT GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
#define TLC_SCLK_INIT GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);  
#define TLC_GSCLK_INIT GPIO_Init (GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);  