#if 1 /* ON/OFF */

#define __weak   __attribute__((weak))

void CatchUnhandledIrq()
{
	int proceed = 0;

	while (!proceed); // Set breakpoint here
}

//__weak void Reset_Handler(void)                   {CatchUnhandledIrq();}
__weak void NMI_Handler(void)                     {CatchUnhandledIrq();}
__weak void HardFault_Handler(void)               {CatchUnhandledIrq();}
__weak void SVC_Handler(void)                     {CatchUnhandledIrq();}
__weak void PendSV_Handler(void)                  {CatchUnhandledIrq();}
__weak void SysTick_Handler(void)                 {CatchUnhandledIrq();}

__weak void WWDG_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void PVD_IRQHandler(void)                  {CatchUnhandledIrq();}
__weak void RTC_IRQHandler(void)                  {CatchUnhandledIrq();}
__weak void FLASH_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void RCC_IRQHandler(void)                  {CatchUnhandledIrq();}
__weak void EXTI0_1_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void EXTI2_3_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void EXTI4_15_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void DMA1_CH1_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void DMA1_CH2_3_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void DMA1_CH4_5_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void ADC_IRQHandler(void)                  {CatchUnhandledIrq();}
__weak void TIM1_BRK_UP_TRG_COM_IRQHandler(void)  {CatchUnhandledIrq();}
__weak void TIM1_CC_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void TIM3_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void TIM6_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void TIM14_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void TIM15_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void TIM16_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void TIM17_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void I2C1_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void I2C2_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void SPI1_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void SPI2_IRQHandler(void)                 {CatchUnhandledIrq();}
__weak void USART1_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void USART2_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void USART3_4_5_6_IRQHandler(void)         {CatchUnhandledIrq();}
__weak void USB_IRQHandler(void)                  {CatchUnhandledIrq();}

#endif
