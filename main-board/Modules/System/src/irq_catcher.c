#if 1 /* ON/OFF */

#define __weak   __attribute__((weak))

void CatchUnhandledIrq()
{
	int proceed = 0;

	while (!proceed); // Set breakpoint here
}

//__weak void Reset_Handler(void)                 {CatchUnhandledIrq();}
__weak void NMI_Handler(void)                   {CatchUnhandledIrq();}
__weak void HardFault_Handler(void)             {CatchUnhandledIrq();}
__weak void MemManage_Handler(void)             {CatchUnhandledIrq();}
__weak void BusFault_Handler(void)              {CatchUnhandledIrq();}
__weak void UsageFault_Handler(void)            {CatchUnhandledIrq();}
__weak void SVC_Handler(void)                   {CatchUnhandledIrq();}
__weak void DebugMon_Handler(void)              {CatchUnhandledIrq();}
__weak void PendSV_Handler(void)                {CatchUnhandledIrq();}
__weak void SysTick_Handler(void)               {CatchUnhandledIrq();}

__weak void WWDG_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void PVD_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void TAMP_STAMP_IRQHandler(void)         {CatchUnhandledIrq();}
__weak void RTC_WKUP_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void FLASH_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void RCC_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void EXTI_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void EXTI1_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void EXTI2_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void EXTI3_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void EXTI4_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void DMA1_Stream_IRQHandler(void)        {CatchUnhandledIrq();}
__weak void DMA1_Stream1_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA1_Stream2_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA1_Stream3_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA1_Stream4_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA1_Stream5_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA1_Stream6_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void ADC_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void CAN1_TX_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void CAN1_RX_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void CAN1_RX1_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void CAN1_SCE_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void EXTI9_5_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void TIM1_BRK_TIM9_IRQHandler(void)      {CatchUnhandledIrq();}
__weak void TIM1_UP_TIM1_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void TIM1_TRG_COM_TIM11_IRQHandler(void) {CatchUnhandledIrq();}
__weak void TIM1_CC_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void TIM2_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void TIM3_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void TIM4_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void I2C1_EV_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void I2C1_ER_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void I2C2_EV_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void I2C2_ER_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void SPI1_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void SPI2_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void USART1_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void USART2_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void USART3_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void EXTI15_1_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void RTC_Alarm_IRQHandler(void)          {CatchUnhandledIrq();}
__weak void OTG_FS_WKUP_IRQHandler(void)        {CatchUnhandledIrq();}
__weak void TIM8_BRK_TIM12_IRQHandler(void)     {CatchUnhandledIrq();}
__weak void TIM8_UP_TIM13_IRQHandler(void)      {CatchUnhandledIrq();}
__weak void TIM8_TRG_COM_TIM14_IRQHandler(void) {CatchUnhandledIrq();}
__weak void TIM8_CC_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void DMA1_Stream7_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void FMC_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void SDIO_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void TIM5_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void SPI3_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void UART4_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void UART5_IRQHandler(void)              {CatchUnhandledIrq();}
__weak void TIM6_DAC_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void TIM7_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void DMA2_Stream_IRQHandler(void)        {CatchUnhandledIrq();}
__weak void DMA2_Stream1_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA2_Stream2_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA2_Stream3_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA2_Stream4_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void CAN2_TX_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void CAN2_RX_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void CAN2_RX1_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void CAN2_SCE_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void OTG_FS_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void DMA2_Stream5_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA2_Stream6_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void DMA2_Stream7_IRQHandler(void)       {CatchUnhandledIrq();}
__weak void USART6_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void I2C3_EV_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void I2C3_ER_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void OTG_HS_EP1_OUT_IRQHandler(void)     {CatchUnhandledIrq();}
__weak void OTG_HS_EP1_IN_IRQHandler(void)      {CatchUnhandledIrq();}
__weak void OTG_HS_WKUP_IRQHandler(void)        {CatchUnhandledIrq();}
__weak void OTG_HS_IRQHandler(void)             {CatchUnhandledIrq();}
__weak void DCMI_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void FPU_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void SPI4_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void SAI1_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void SAI2_IRQHandler(void)               {CatchUnhandledIrq();}
__weak void QuadSPI_IRQHandler(void)            {CatchUnhandledIrq();}
__weak void CEC_IRQHandler(void)                {CatchUnhandledIrq();}
__weak void SPDIF_RX_IRQHandler(void)           {CatchUnhandledIrq();}
__weak void FMPI2C1_Event_IRQHandler(void)      {CatchUnhandledIrq();}
__weak void FMPI2C1_Error_IRQHandler(void)      {CatchUnhandledIrq();}

#endif
