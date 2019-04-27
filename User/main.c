#include "1986be9x_config.h"
#include "1986BE9x.h"
#include "1986BE9x_uart.h"
#include "1986BE9x_port.h"
#include "1986BE9x_rst_clk.h"
#include "1986BE9x_it.h"
#include "mlt_lcd.h"
#include "MilFlash.h"

void Led_init (void)
{
  RST_CLK->PER_CLOCK |=(1<<23);                   //Включаем тактирование порта C.
	PORTC->OE |= 1;                                 //Порт - выход.
	PORTC->ANALOG |= 1;                             //Порт - цифоровой. 
	PORTC->PWR |= 1;                                //Порт - медленный режим.
}

#define CLKSOURCE (1<<2)                          //Указывает источник синхросигнала: 0 - LSI, 1 - HCLK.
#define TCKINT    (1<<1)                          //Разрешает запрос на прерывание от системного таймера.
#define ENABLE    (1<<0)                          //Разрешает работу таймера.
void Init_SysTick (void)                          //Прерывание раз в милли секунду. 
{
	SysTick->LOAD |= (8000000/1000)-1;              
	SysTick->CTRL |= CLKSOURCE|TCKINT|ENABLE;
}

volatile uint32_t Delay_dec = 0;
void SysTick_Handler (void)
{
	if (Delay_dec) Delay_dec--;
}

void Delay_ms (uint32_t Delay_ms_Data)
{
	Delay_dec = Delay_ms_Data;
	while (Delay_dec) {};
}

int main (void)
{
	Init_SysTick();                                 //Инициализируем системный таймер. 
	Led_init();                                     //Инициализируем ножку 0 порта C для светодиода. 
	
  while (1)
	{
		PORTC->RXTX |= 1;
		Delay_ms (1000);
		PORTC->RXTX = 0;
	  Delay_ms (1000);
	}
}
