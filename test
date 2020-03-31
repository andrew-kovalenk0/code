#include"stm32f407xx.h"

uint8_t connection_timer = 0;
uint8_t rfid_buffer[33];
uint8_t crc_buffer = 0;
uint8_t indicator_blinking = 0;
uint8_t command_buffer[11];
uint8_t crc = 0;
uint8_t crc_summ = 0;
uint8_t temperature_1 = 0;
uint8_t temperature_2 = 0;
uint8_t black_box[5];
uint8_t power_1 = 0;
uint8_t power_2 = 0;
uint8_t emitter_1_timer_to_send = 0;
uint8_t emitter_2_timer_to_send = 0;
uint16_t emitter_1_timer = 0;
uint16_t emitter_2_timer = 0;
uint16_t black_box_time[2];
uint16_t status_word[30];
uint16_t generation_parametrs[8];
uint16_t adc_buffer[60];
uint16_t adc_value[6];
uint16_t analog_errors = 0;
uint16_t digital_errors = 0;
uint16_t rfid_timer = 0;
uint16_t timer_ready_state = 0;
uint16_t impulse_counter = 0;
uint32_t flags = 0;

void init_all()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_DMA2EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_USART3EN | RCC_APB1ENR_DACEN | RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_I2C2EN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_SYSCFGEN | RCC_APB2ENR_ADC1EN;

	GPIOA->MODER |= GPIO_MODER_MODER2_0;
	GPIOB->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
	GPIOD->MODER |= GPIO_MODER_MODER10_0 | GPIO_MODER_MODER13_0;

	SysTick_Config(1600000);

	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PC | SYSCFG_EXTICR2_EXTI7_PC;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC | SYSCFG_EXTICR3_EXTI9_PC | SYSCFG_EXTICR3_EXTI10_PA | SYSCFG_EXTICR3_EXTI11_PA;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PA;
	EXTI->IMR |= EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12;
	EXTI->FTSR |= EXTI_FTSR_TR6 | EXTI_FTSR_TR7 | EXTI_FTSR_TR8 | EXTI_FTSR_TR9 | EXTI_FTSR_TR10 | EXTI_FTSR_TR11 | EXTI_FTSR_TR12;
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	__enable_irq();

	GPIOA->MODER |= GPIO_MODER_MODER15_1;
	GPIOA->AFR[1] |= GPIO_AFRH_AFRH7_0;
	TIM2->PSC = 400;
	TIM2->ARR = 200;
	TIM2->CCR1 = 0;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CR1 |= TIM_CR1_CEN;

	GPIOD->MODER |= GPIO_MODER_MODER12_1;
	GPIOD->AFR[1] |= GPIO_AFRH_AFRH4_1;
	TIM4->PSC = 400;
	TIM4->ARR = 200;
	TIM4->CCR1 = 0;
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM4->CCER |= TIM_CCER_CC1E;
	TIM4->CR1 |= TIM_CR1_CEN;

	GPIOA->MODER |= GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7;
	GPIOC->MODER |= GPIO_MODER_MODER1 | GPIO_MODER_MODER2;
	ADC1->SQR1 |= ADC_SQR1_L_1 | ADC_SQR1_L_2;
	ADC1->SQR3 |= ADC_SQR3_SQ2_0 | ADC_SQR3_SQ3_2 | ADC_SQR3_SQ3_1 | ADC_SQR3_SQ4_2 | ADC_SQR3_SQ4_1 | ADC_SQR3_SQ4_0 | ADC_SQR3_SQ5_3 | ADC_SQR3_SQ5_1 | ADC_SQR3_SQ5_0 | ADC_SQR3_SQ6_3 | ADC_SQR3_SQ6_2;
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_ADON;

	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
	DMA2_Stream0->CR  |= DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0 | DMA_SxCR_MINC | DMA_SxCR_CIRC;
	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
	DMA2_Stream0->M0AR = (uint32_t)&adc_buffer;
	DMA2_Stream0->NDTR = 60;
   	DMA2_Stream0->CR |= DMA_SxCR_EN;

	GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_0  | GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_0;
	USART1->CR1 |= USART_CR1_OVER8;
	USART1->BRR = 0x116;
	USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

	GPIOD->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1;
	GPIOD->AFR[0] |= GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_1 | GPIO_AFRL_AFRL5_0  | GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_0;
	USART2->CR1 |= USART_CR1_OVER8;
	USART2->BRR = 0x116;
	USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

	GPIOD->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
	GPIOD->AFR[1] |= GPIO_AFRH_AFRH0_2 | GPIO_AFRH_AFRH0_1 | GPIO_AFRH_AFRH0_0 | GPIO_AFRH_AFRH1_2 | GPIO_AFRH_AFRH1_1 | GPIO_AFRH_AFRH1_0;
	USART3->CR1 |= USART_CR1_OVER8;
	USART3->BRR = 0x116;
	USART3->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1 | DAC_SWTRIGR_SWTRIG2;
	DAC->DHR12R1 = 0;
	DAC->DHR12R2 = 0;
	DAC->CR |= DAC_CR_EN1 | DAC_CR_EN2;

	GPIOB->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_11;
	GPIOB->AFR[1] |= (0x04<<2*4);
	GPIOB->AFR[1] |= (0x04<<3*4);
	I2C2->CR1 &= ~I2C_CR1_SMBUS;
	I2C2->CR2 &= ~I2C_CR2_FREQ;
	I2C2->CR2 |= 42;
	I2C2->CCR &= ~(I2C_CCR_FS | I2C_CCR_DUTY);
	I2C2->CCR |= 208;
	I2C2->TRISE = 42;
	I2C2->CR1 |= I2C_CR1_PE;

	// ВТ-педаль
}

void SysTick_Handler(void)
{
//	if((flags & 0x2) != 0 || (flags & 0x4) != 0 )
//		++connection_timer;
	if((flags & 0x8) != 0)
	{
		++emitter_1_timer_to_send;
		++emitter_1_timer;
	}
	if((flags & 0x10) != 0)
	{
		++emitter_2_timer_to_send;
		++emitter_2_timer;
	}
	if((flags & 0x4) != 0)
		++timer_ready_state;
	if((flags & 0x200) != 0)
		++rfid_timer;
	if((flags & 0x40) != 0)
	{
		++indicator_blinking;
		if(indicator_blinking >= 5 && (flags & 0x80) == 0)
		{
			GPIOB->BSRR |= GPIO_BSRR_BS_13;
			flags |= 0x80;
			indicator_blinking = 0;
		}
		if(indicator_blinking >= 5 && (flags & 0x80) != 0)
		{
			GPIOB->BSRR |= GPIO_BSRR_BR_13;
			flags &= ~0x80;
			indicator_blinking = 0;
		}
	}
	if((flags & 0x8) != 0 && generation_parametrs[6] == 0x2)
	{
		++impulse_counter;
		if((flags & 0x80) != 0 && impulse_counter == generation_parametrs[0])
		{
			GPIOB->BSRR |= GPIO_BSRR_BR_15;
			flags &= ~0x80;
			impulse_counter = 0;
		}
		if((flags & 0x80) == 0 && impulse_counter == generation_parametrs[2])
		{
			GPIOB->BSRR |= GPIO_BSRR_BS_15;
			flags |= 0x80;
			impulse_counter = 0;
		}
	}
    if((flags & 0x10) != 0 && generation_parametrs[7] == 0x2)
	{
		++impulse_counter;
		if((flags & 0x80) != 0 && impulse_counter == generation_parametrs[2])
		{
			GPIOD->BSRR |= GPIO_BSRR_BR_10;
			flags &= ~0x80;
		}
		if((flags & 0x80) == 0 && impulse_counter == generation_parametrs[4])
		{
			GPIOD->BSRR |= GPIO_BSRR_BS_10;
			flags |= 0x80;
		}
	}
}

void flash_write(uint8_t reg_addr1, uint8_t reg_addr2, uint8_t data)
{
    I2C2->CR1 |= I2C_CR1_START;
	while(!(I2C2->SR1 & I2C_SR1_SB));
	(void) I2C2->SR1;
	I2C2->DR = 0xA0;
	while(!(I2C2->SR1 & I2C_SR1_ADDR));
	(void) I2C2->SR1;
	(void) I2C2->SR2;
	I2C2->DR = reg_addr1;
	while(!(I2C2->SR1 & I2C_SR1_TXE));
	(void) I2C2->SR1;
	I2C2->DR = reg_addr2;
	while(!(I2C2->SR1 & I2C_SR1_TXE));
	(void) I2C2->SR1;
	I2C2->DR = data;
	while(!(I2C2->SR1 & I2C_SR1_BTF));
	I2C2->CR1 |= I2C_CR1_STOP;
}

uint8_t flash_read(uint8_t reg_addr1, uint8_t reg_addr2)
{
	uint8_t data;
	I2C2->CR1 |= I2C_CR1_START;
	while(!(I2C2->SR1 & I2C_SR1_SB));
	(void) I2C2->SR1;
	I2C2->DR = 0xA0;
	while(!(I2C2->SR1 & I2C_SR1_ADDR));
	(void) I2C2->SR1;
	(void) I2C2->SR2;
	I2C2->DR = reg_addr1;
	while(!(I2C2->SR1 & I2C_SR1_TXE));
	(void) I2C2->SR1;
	I2C2->DR = reg_addr2;
	while(!(I2C2->SR1 & I2C_SR1_TXE));
	I2C2->CR1 |= I2C_CR1_STOP;
	I2C2->CR1 |= I2C_CR1_START;
	while(!(I2C2->SR1 & I2C_SR1_SB));
	(void) I2C2->SR1;
	I2C2->DR = 0xA1;
	while(!(I2C2->SR1 & I2C_SR1_ADDR));
	(void) I2C2->SR1;
	(void) I2C2->SR2;
	I2C2->CR1 &= ~I2C_CR1_ACK;
	while(!(I2C2->SR1 & I2C_SR1_RXNE));
	data = I2C2->DR;
	I2C2->CR1 |= I2C_CR1_STOP;
	return data;
}

void transmit_value(void *ad, int length, int usart)
{
	if(usart == 1)
	{
		for(int i = length - 1; i >= 0; --i)
		{
			while((USART1->SR & USART_SR_TXE) == 0);
			USART1->DR = *((char*)ad + i);
			crc_buffer += *((char*)ad + i);
		}
	}
	if(usart == 2)
	{
		for(int i = length - 1; i >= 0; --i)
		{
			while((USART2->SR & USART_SR_TXE) == 0);
			USART2->DR = *((char*)ad + i);
			crc_buffer += *((char*)ad + i);
		}
	}
	if(usart == 3)
	{
		for(int i = length - 1; i >= 0; --i)
		{
			while((USART3->SR & USART_SR_TXE) == 0);
			USART3->DR = *((char*)ad + i);
			crc_buffer += *((char*)ad + i);
		}
	}
}

void read_value(void *ad, int length)
{
	for(int i = length - 1; i >= 0; --i)
	{
		while((USART3->SR & USART_SR_RXNE) == 0);
		*((char*)ad + i) = USART3->DR;
		crc_summ += USART3->DR;
	}
}

void set_flag(int flag_number, int value)
{
	switch(flag_number)
	{
		case 1:
		{
			if(value != 0)
				status_word[9] |= 0x1;
			else
				status_word[9] &= ~0x1;
			break;
		}
		case 2:
		{
			if(value != 0)
				status_word[9] |= 0x2;
			else
				status_word[9] &= ~0x2;
			break;
		}
		case 3:
		{
			if(value != 0)
				status_word[9] |= 0x4;
			else
				status_word[9] &= ~0x4;
			break;
		}
		case 4:
		{
			if(value != 0)
				status_word[9] |= 0x8;
			else
				status_word[9] &= ~0x8;
			break;
		}
		case 5:
		{
			if(value != 0)
				status_word[9] |= 0x10;
			else
				status_word[9] &= ~0x10;
			break;
		}
		case 6:
		{
			if(value != 0)
				status_word[9] |= 0x20;
			else
				status_word[9] &= ~0x20;
			break;
		}
		case 7:
		{
			if(value != 0)
				status_word[9] |= 0x40;
			else
				status_word[9] &= ~0x40;
			break;
		}
		case 8:
		{
			if(value != 0)
				status_word[9] |= 0x80;
			else
				status_word[9] &= ~0x80;
			break;
		}
		case 9:
		{
			if(value != 0)
				status_word[9] |= 0x100;
			else
				status_word[9] &= ~0x100;
			break;
		}
		case 10:
		{
			if(value != 0)
				status_word[9] |= 0x200;
			else
				status_word[9] &= ~0x200;
			break;
		}
	}
	flash_write(0, 18, *((char*)&status_word[9] + 0));
	for(int i = 0; i <= 7500; ++i);
	flash_write(0, 19, *((char*)&status_word[9] + 1));
	for(int i = 0; i <= 7500; ++i);
}

void processing_adc_value()
{
	for(int i = 0; i < 10; ++i)
	{
		DMA2_Stream0->CR &= ~DMA_SxCR_EN;
		DMA2_Stream0->M0AR = (uint32_t)&adc_buffer[i * 6];
	   	DMA2_Stream0->CR |= DMA_SxCR_EN;
		ADC1->CR2 |= ADC_CR2_SWSTART;
		for(int j = 0; j < 1000; ++j);
	}
	for(int i = 0; i < 6; ++i)
		adc_value[i] = 0;
	for(int i = 0; i < 6; ++i)
		for(int j = 0; j < 10; ++j)
			adc_value[i] += adc_buffer[i+j*6];
	for(int i = 0; i < 6; ++i)
		adc_value[i] /= 10;
	adc_value[1] = 0x0900;
	adc_value[3] = 0x0900;
	adc_value[5] = 0x0b00;
}

void analog_emergency_situations_check()
{
	processing_adc_value();
	if(adc_value[0] >= 0x0e00)
		analog_errors |= 0x1;
	if(adc_value[1] >= 0x0e00)
		analog_errors |= 0x2;
	if(adc_value[2] >= 0x0e00)
		analog_errors |= 0x4;
	if(adc_value[3] >= 0x0e00)
		analog_errors |= 0x8;

	if(adc_value[4] <= 0x0a00)
		analog_errors |= 0x80;
	if(adc_value[5] <=  0x0a00)
		analog_errors |= 0x80;
	if(adc_value[4] > 0x0a00)
		analog_errors &= ~0x80;
	if(adc_value[5] >  0x0a00)
		analog_errors &= ~0x80;
	if(adc_value[4] >= 0x0c00)
	{
		GPIOD->BSRR |= GPIO_BSRR_BS_13;
		if(adc_value[4] >= 0x0e00)
			analog_errors |= 0x40;
		if(adc_value[4] <= 0x0d80)
			analog_errors &= ~0x40;
		if(adc_value[4] >= 0x0f00)
			analog_errors |= 0x100;
	}
	if(adc_value[5] >= 0x0c00)
	{
		GPIOD->BSRR |= GPIO_BSRR_BS_13;
		if(adc_value[5] >= 0x0e00)
			analog_errors |= 0x80;
		if(adc_value[5] <= 0x0d80)
			analog_errors &= ~0x80;
		if(adc_value[5] >= 0x0f00)
			analog_errors |= 0x200;
	}
	if(adc_value[4] <= (0x0c00 - 0x0500))// && adc_value[5] <= 0x0c00 - 0x100)
		GPIOD->BSRR |= GPIO_BSRR_BR_13;
/*	if(adc_value[4] >= status_word[4])
	{
		GPIOD->BSRR |= GPIO_BSRR_BS_13;
		if(adc_value[4] >= status_word[6])
			analog_errors |= 0x40;
		if(adc_value[4] <= status_word[7])
			analog_errors &= ~0x40;
		if(adc_value[4] >= status_word[5])
			analog_errors |= 0x100;
	}
	if(adc_value[5] >= 2000)
	{
		GPIOD->BSRR |= GPIO_BSRR_BS_13;
		if(adc_value[5] >= status_word[6])
			analog_errors |= 0x80;
		if(adc_value[5] <= status_word[7])
			analog_errors &= ~0x80;
		if(adc_value[5] >= status_word[5])
			analog_errors |= 0x200;
	}
	if(adc_value[4] <= (status_word[4] - 0xa1) && adc_value[5] <= status_word[4] - 0xa1)
		GPIOD->BSRR |= GPIO_BSRR_BR_13;
*/}

void clear_command_buffer()
{
	for(int i = 0; i < 11; ++i)
		command_buffer[i] = 0;
}

void read_command(int length)
{
	crc_summ = 0;
	for(int i = 0; i < length; ++i)
	{
		while((USART3->SR & USART_SR_RXNE) == 0);
		command_buffer[i] = USART3->DR;
		crc_summ +=command_buffer[i];
	}
	while((USART3->SR & USART_SR_RXNE) == 0);
	crc = USART3->DR;
	if(crc_summ != crc)
		flags |= 0x1;
	crc_summ = 0;
}

void errors_check()
{
	analog_emergency_situations_check();
//	if((GPIOD->IDR & 0x4000) == 0 && (flags & 0x4) == 0)
//		digital_errors |= 0x8;
//	if((GPIOD->IDR & 0x8000) == 0 && (flags & 0x4) == 0)
//		digital_errors |= 0x10;
}

void error_handling()
{
	begin1:
	read_command(2);
	if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf1)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x05;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0xf1;
		transmit_value(&analog_errors, 2, 3);
		transmit_value(&digital_errors,2, 3);
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = temperature_1;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = temperature_2;
		crc_buffer = 0xf6 + temperature_1 + temperature_2;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = crc_buffer;
		crc_buffer = 0;
		analog_errors = 0;
		digital_errors = 0;
		begin:
		read_command(3);
		while(1)
		{
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf2)
			{
				if(command_buffer[2] == 0x0A)
					goto end;
				if(command_buffer[2] == 0x00)
				{
					__disable_irq();
					GPIOB->BSRR |= GPIO_BSRR_BS_14;
					GPIOB->BSRR |= GPIO_BSRR_BR_12;
					GPIOB->BSRR |= GPIO_BSRR_BR_13;
					GPIOB->BSRR |= GPIO_BSRR_BR_14;
					DAC->DHR12R1 = 0;
					DAC->DHR12R2 = 0;
					flags &= ~0x4;
					flags &= ~0x20;
					flags &= ~0x40;
					flags &= ~0x80;
					TIM3->CCR2 = 0;
					TIM3->CCR3 = 0;
				}
			}
			goto begin;
		}
	}
	clear_command_buffer();
	goto begin1;
	end:
	clear_command_buffer();
}

void set_duty_cycle(int duty_cycle)
{
	switch(duty_cycle)
	{
		case 0:
		{
			TIM2->CCR1 = 0;
			TIM4->CCR1 = 0;
			break;
		}
		case 25:
		{
			TIM2->CCR1 = 50;
			TIM4->CCR1 = 50;
			break;
		}
		case 50:
		{
			TIM2->CCR1 = 100;
			TIM4->CCR1 = 100;
			break;
		}
		case 75:
		{
			TIM2->CCR1 = 150;
			TIM4->CCR1 = 150;
			break;
		}
		case 100:
		{
			TIM2->CCR1 = 200;
			TIM4->CCR1 = 200;
			break;
		}
	}
}

void connection_check()
{
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xf0;
	errors_check();
	if(analog_errors != 0 || digital_errors != 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x0c;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x01;
		error_handling();
	}
	if((flags & 0x1) == 0 && analog_errors == 0 && digital_errors == 0)
	{
		while ((USART3->SR & USART_SR_TXE)==0);
		USART3->DR = 0x0A;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0xFF;
	}
	if((flags & 0x1) != 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x0B;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x00;
		flags &= ~0x1;
	}
	clear_command_buffer();
}

void connection_check_in_ready()
{
	errors_check();
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xf8;
	crc_buffer = 0x05 + 0xf8;
	if((flags & 0x8) == 0 && (flags & 0x10) == 0 && analog_errors == 0 && digital_errors == 0 && (flags & 0x1) == 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x00;
	}
	if((flags & 0x8) != 0 && analog_errors == 0 && digital_errors == 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x0A;
		crc_buffer += 0x0A;
	}
	if((flags & 0x10) != 0 && analog_errors == 0 && digital_errors == 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x0B;
		crc_buffer += 0x0B;
	}
	if(analog_errors != 0 || digital_errors != 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		crc_buffer += 0x0C;
		USART3->DR = 0x0C;
	}
	if((flags & 0x1) != 0 && analog_errors == 0 && digital_errors == 0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x0D;
		crc_buffer += 0x0D;
		flags &= ~0x1;
	}
	if((status_word[9] & 0x80) == 0)
	{
		if(adc_value[2] > 0 && adc_value[2] <= 330)
			power_1 = (int)((adc_value[2] - 25) / 10.3); // * status_word[26];
		if(adc_value[2] > 330 && adc_value[2] <= 660)
			power_1 = (int)((adc_value[2] - 20) / 11); // * status_word[26];
		if(adc_value[2] > 660 && adc_value[2] <= 990)
			power_1 = (int)((adc_value[2] + 260) / 15); // * status_word[26];
		if(adc_value[2] > 990 && adc_value[2] <= 1320)
			power_1 = (int)((adc_value[2] + 630) / 19.5); // * status_word[26];
	}
	if((status_word[9] & 0x80) != 0)
	{
		if(adc_value[0] > 0 && adc_value[0] <= 300)
			power_1 = (int)((adc_value[0] - 55) / 8.5); // * status_word[28];
		if(adc_value[0] > 300 && adc_value[0] <= 600)
			power_1 = (int)((adc_value[0] - 20) / 9.8); // * status_word[28];
		if(adc_value[0] > 600 && adc_value[0] <= 900)
			power_1 = (int)((adc_value[0] + 125) / 12); // * status_word[28];
		if(adc_value[0] > 900 && adc_value[0] <= 1200)
			power_1 = (int)((adc_value[0] + 300) / 14); // * status_word[28];
	}
	if((status_word[9] & 0x80) != 0)
	{
		if(adc_value[1] > 0 && adc_value[1] <= 300)
			power_2 = (int)((adc_value[1] - 70) / 4.5); // * status_word[29];
		if(adc_value[1] > 300 && adc_value[1] <= 600)
			power_2 = (int)((adc_value[1] - 40) / 5); // * status_word[29];
		if(adc_value[1] > 600 && adc_value[1] <= 900)
			power_2 = (int)((adc_value[1] + 100) / 6.3); // * status_word[29];
		if(adc_value[1] > 900 && adc_value[1] <= 1200)
			power_2 = (int)((adc_value[1] + 90) / 6.3); // * status_word[29];
	}
	power_1 = generation_parametrs[4];
	power_2 = generation_parametrs[5];
	if(((flags & 0x8) != 0 || (flags & 0x10) != 0) && analog_errors == 0 && digital_errors == 0)
	{
		if((flags & 0x8) != 0 && analog_errors == 0 && digital_errors == 0)
		{
			while ((USART3->SR & USART_SR_TXE) == 0);
			USART3->DR = 0;
			transmit_value(&emitter_1_timer_to_send, 1, 3);
			while ((USART3->SR & USART_SR_TXE) == 0);
			USART3->DR = 0;
			if(generation_parametrs[6] != 0x2)
				transmit_value(&power_1, 1, 3);
			else
			{
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0;
			}
		}
		if((flags & 0x10) != 0 && analog_errors == 0 && digital_errors == 0)
		{
			while ((USART3->SR & USART_SR_TXE) == 0);
			USART3->DR = 0;
			transmit_value(&emitter_2_timer_to_send, 1, 3);
			while ((USART3->SR & USART_SR_TXE) == 0);
			USART3->DR = 0;
			if(generation_parametrs[6] != 0x2)
				transmit_value(&power_2, 1, 3);
			else
			{
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0;
			}
		}
	}
	else
		for(int i = 0; i < 4; ++i)
		{
			while ((USART3->SR & USART_SR_TXE) == 0);
			USART3->DR = 0;
		}
	processing_adc_value();
	for(int i = 0; i < 6; ++i)
	{
		transmit_value(&adc_value[i], 2, 3);
	}
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = crc_buffer;
	clear_command_buffer();
	crc_buffer = 0;
	if(analog_errors != 0 || digital_errors != 0)
	{
		error_handling();
	}
	emitter_1_timer_to_send = 0;
	emitter_2_timer_to_send = 0;
}

void chanel_1_generation()
{
	flags &= ~0x40;
	if(generation_parametrs[4] > 0 && generation_parametrs[4] <= 25)
		DAC->DHR12R1 = (generation_parametrs[4] * 26 + 410); // * status_word[10];
	if(generation_parametrs[4] > 25 && generation_parametrs[4] <= 50)
		DAC->DHR12R1 = (generation_parametrs[4] * 29 + 330); // * status_word[11];
	if(generation_parametrs[4] > 50 && generation_parametrs[4] <= 75)
		DAC->DHR12R1 = (generation_parametrs[4] * 36 + 10); // * status_word[12];
	if(generation_parametrs[4] > 75 && generation_parametrs[4] <= 100)
		DAC->DHR12R1 = (generation_parametrs[4] * 40 - 260); // * status_word[13];
	// корректировка температурными коэффициентами
	GPIOB->BSRR |= GPIO_BSRR_BS_13;
	GPIOB->BSRR |= GPIO_BSRR_BS_14;
	flags |= 0x8;
	GPIOB->BSRR |= GPIO_BSRR_BS_15;
	while((GPIOD->IDR & 0x4000) == 0 && (flags & 0x800) == 0)
	{
		if(emitter_1_timer >= 600 && (status_word[9] & 0x40) != 0)
			flags |= 0x800;
/*		if(connection_timer > 20)
		{
			__disable_irq();
			GPIOB->BSRR |= GPIO_BSRR_BS_14;
			GPIOB->BSRR |= GPIO_BSRR_BR_13;
			GPIOB->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_10;
			DAC->DHR12R1 = 0;
			DAC->DHR12R2 = 0;
			flags &= ~0x4;
			flags &= ~0x20;
			flags &= ~0x40;
			flags &= ~0x80;
			TIM2->CCR1 = 0;
			TIM4->CCR1 = 0;
			while(1);
		}
*/		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ += command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			while((USART3->SR & USART_SR_RXNE) == 0);
			crc = USART3->DR;
			if(crc_summ != crc)
				flags |= 0x1;
			crc_summ = 0;
			errors_check();
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf8)
			{
				connection_check_in_ready();
				connection_timer = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfe)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[3];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				set_flag(command_buffer[2], command_buffer[3]);
			}
			clear_command_buffer();
		}
	}
	DAC->DHR12R1 = 0;
	GPIOB->BSRR |= GPIO_BSRR_BR_13;
	flags &= ~ 0x8;
	flags &= ~ 0x80;
	GPIOB->BSRR |= GPIO_BSRR_BR_14;
	GPIOB->BSRR |= GPIO_BSRR_BR_15;
	flags |= 0x40;
	black_box_time[0] += emitter_1_timer;
	for(int k = 0; k <= 7500; ++k);
	flash_write(0, 81, *((char*)&black_box_time[0]));
	for(int k = 0; k <= 7500; ++k);
	flash_write(0, 82, *((char*)&black_box_time[0]) + 1);
	emitter_1_timer = 0;
}

void chanel_2_generation()
{
	flags &= ~0x40;
	if(generation_parametrs[5] > 0 && generation_parametrs[5] < 50)
		DAC->DHR12R2 = generation_parametrs[5] * 15 + 470; // * status_word[14];
	if(generation_parametrs[5] > 50 && generation_parametrs[5] < 100)
		DAC->DHR12R2 = generation_parametrs[5] * 17 + 330; // * status_word[15];
	if(generation_parametrs[5] > 100 && generation_parametrs[5] < 150)
		DAC->DHR12R2 = generation_parametrs[5] * 20; // * status_word[16];
	if(generation_parametrs[5] > 150 && generation_parametrs[5] < 200)
		DAC->DHR12R2 = generation_parametrs[5] * 21 - 150; // * status_word[17];
	GPIOB->BSRR |= GPIO_BSRR_BS_13;
	GPIOB->BSRR |= GPIO_BSRR_BS_14;
	flags |= 0x10;
	GPIOD->BSRR |= GPIO_BSRR_BS_10;
	while((GPIOD->IDR & 0x8000) == 0 && (flags & 0x800) == 0)
	{
		if(emitter_1_timer >= 600 && (status_word[9] & 0x40) != 0)
			flags |= 0x800;
/*		if(connection_timer > 20)
		{
			__disable_irq();
			GPIOB->BSRR |= GPIO_BSRR_BS_14;
			GPIOB->BSRR |= GPIO_BSRR_BR_13;
			GPIOB->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_10;
			DAC->DHR12R1 = 0;
			DAC->DHR12R2 = 0;
			flags &= ~0x4;
			flags &= ~0x20;
			flags &= ~0x40;
			flags &= ~0x80;
			TIM2->CCR1 = 0;
			TIM4->CCR1 = 0;
			while(1);
		}
*/
		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ += command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			while((USART3->SR & USART_SR_RXNE) == 0);
			crc = USART3->DR;
			if(crc_summ != crc)
				flags |= 0x1;
			crc_summ = 0;
			errors_check();
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf8)
			{
				connection_check_in_ready();
				connection_timer = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfe)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[3];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				set_flag(command_buffer[2], command_buffer[3]);
			}
			clear_command_buffer();
		}
	}
	DAC->DHR12R2 = 0;
	GPIOB->BSRR |= GPIO_BSRR_BR_13;
	flags &= ~ 0x10;
	flags &= ~ 0x80;
	GPIOB->BSRR |= GPIO_BSRR_BR_14;
	GPIOB->BSRR |= GPIO_BSRR_BR_15;
	flags |= 0x40;
	black_box_time[1] += emitter_2_timer;
	for(int k = 0; k <= 7500; ++k);
	flash_write(0, 83, *((char*)&black_box_time[1]));
	for(int k = 0; k <= 7500; ++k);
	flash_write(0, 84, *((char*)&black_box_time[1]) + 1);
	emitter_2_timer = 0;
}

void ready_state()
{
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xf6;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xfb;
	flags |= 0x4;
	flags |= 0x40;
	flags &= ~ 0x800;
	while(timer_ready_state <= 72000 && (flags & 0x20) == 0 && (flags & 0x100) == 0)
	{
		if((GPIOD->IDR & 0x4000) == 0 && (flags & 0x800) == 0)
			chanel_1_generation();
		if((GPIOD->IDR & 0x8000) == 0 && (flags & 0x800) == 0)
			chanel_2_generation();
//		опрос ВТ-педали
/*		if(connection_timer > 20)
		{
			__disable_irq();
			GPIOB->BSRR |= GPIO_BSRR_BS_14;
			GPIOB->BSRR |= GPIO_BSRR_BR_13;
			GPIOB->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_10;
			DAC->DHR12R1 = 0;
			DAC->DHR12R2 = 0;
			flags &= ~0x4;
			flags &= ~0x20;
			flags &= ~0x40;
			flags &= ~0x80;
			TIM2->CCR1 = 0;
			TIM4->CCR1 = 0;
			while(1);
		}
*/		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			crc_summ = 0;
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ += command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf4)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				set_duty_cycle(command_buffer[2]);
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf7)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				flags |= 0x20;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf8)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				connection_timer = 0;
				connection_check_in_ready();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf9)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				if(command_buffer[2] == 0x0A)
				{
					read_value(&generation_parametrs[4], 2);
				}
				if(command_buffer[2] == 0x0B)
				{
					read_value(&generation_parametrs[5], 2);
				}
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xf9;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xfe;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf6)
			{
				for(int i = 0; i < 8; ++i)
				{
					read_value(&generation_parametrs[i], 2);
				}
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xee)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0x02)
					flags |= 0x100;
			}
			crc_buffer = 0;
			clear_command_buffer();
		}
	}
	timer_ready_state = 0;
	flags &= ~0x4;
	flags &= ~0x20;
	flags &= ~0x40;
	flags &= ~0x80;
	GPIOB->BSRR |= GPIO_BSRR_BR_13;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xf7;
	while ((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xfc;
}

void service_menu_control()
{
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xfa;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x01;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x00;
	while((flags & 0x100) == 0)
	{
		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			clear_command_buffer();
			crc_buffer = 0;
			crc_summ = 0;
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ += command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfb)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				while((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xfb;
				crc_buffer = 0;
				processing_adc_value();
				for(int i = 0; i < 6; ++i)
				{
					transmit_value(&adc_value[i], 2, 3);
				}
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = crc_buffer;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xee)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0x01)
					flags |= 0x100;
			}
			clear_command_buffer();
			crc_buffer = 0;
		}
	}
	flags &= ~0x100;
}

void service_menu_generation()
{
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xfa;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x02;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x01;
	while((flags & 0x100) == 0)
		ready_state();
	flags &= ~0x100;
}

void send_start_rfid_message()
{
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x31;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x39;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x33;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x34;
}

void service_menu_tag()
{
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xfa;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x03;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x02;
	while((flags & 0x100) == 0)
	{
		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ +=command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf5)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				flags |= 0x200;
				begin_rfid:
				if((flags & 0x400) == 0)
				{
					for(int i = 0; i < 10000; ++i);
					for(int i = 0; i < 33; ++i)
						rfid_buffer[i] = 0;
					send_start_rfid_message();
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x41;
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x30;
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x30;
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x31;
					for(int i = 0; i < 4; ++i)
					{
						while((USART1->SR & USART_SR_TXE) == 0);
						USART1->DR = 0x30;
					}
					for(int i = 0; i < 16; ++i)
					{
						while((USART1->SR & USART_SR_RXNE) == 0);
						rfid_buffer[i] = USART1->DR;
					}
					if(rfid_buffer[14] == 0x0d && rfid_buffer[15] == 0x0a)
					{
						if(rfid_timer >= 300)
							flags |= 0x400;
						goto begin_rfid;
					}
					for(int i = 16; i < 33; ++i)
					{
						while((USART1->SR & USART_SR_RXNE) == 0);
						rfid_buffer[i] = USART1->DR;
					}
					while ((USART3->SR & USART_SR_TXE) == 0);
					USART3->DR = 0x05;
					while ((USART3->SR & USART_SR_TXE) == 0);
					USART3->DR = 0xf5;
					for(int i = 15; i < 25; ++i)
					{
						while((USART3->SR & USART_SR_TXE) == 0);
						USART3->DR = rfid_buffer[i];
					}
					crc_buffer += 0xfa;
					while ((USART3->SR & USART_SR_TXE) == 0);
					USART3->DR = crc_buffer;
					crc_buffer = 0;
				}
				rfid_timer = 0;
				flags &= ~0x200;
				flags &= ~0x400;
				clear_command_buffer();
				for(int i = 0; i < 33; ++i)
					rfid_buffer[i] = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xee)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[1] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0x03)
					flags |= 0x100;
				clear_command_buffer();
			}
		}
	}
	flags &= ~0x100;
}

void service_menu()
{
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x05;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xfa;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0x00;
	while((USART3->SR & USART_SR_TXE) == 0);
	USART3->DR = 0xff;
	while((flags & 0x100) == 0)
	{
		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			clear_command_buffer();
			crc_buffer = 0;
			crc_summ = 0;
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ += command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfa)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0x01)
				{
					service_menu_control();
				}
				if(command_buffer[2] == 0x02)
				{
					service_menu_generation();
				}
				if(command_buffer[2] == 0x03)
				{
					service_menu_tag();
				}
				service_menu();
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf4)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				set_duty_cycle(command_buffer[2]);
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfc)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0x1)
				{
					GPIOB->BSRR |= GPIO_BSRR_BR_13;
					GPIOB->BSRR |= GPIO_BSRR_BR_14;
				}
				if(command_buffer[2] == 0x2)
				{
					GPIOB->BSRR |= GPIO_BSRR_BS_13;
					GPIOB->BSRR |= GPIO_BSRR_BR_14;
				}
				if(command_buffer[2] == 0x3)
				{
					GPIOB->BSRR |= GPIO_BSRR_BS_13;
					GPIOB->BSRR |= GPIO_BSRR_BS_14;
				}
				if(command_buffer[2] == 0x4)
				{
					GPIOB->BSRR |= GPIO_BSRR_BR_13;
					GPIOB->BSRR |= GPIO_BSRR_BS_14;
				}
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfd)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0)
				{
					GPIOD->BSRR |= GPIO_BSRR_BR_13;
				}
				if(command_buffer[2] == 0x1)
				{
					GPIOD->BSRR |= GPIO_BSRR_BS_13;
				}
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfe)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[3] = USART3->DR;
				crc_summ += command_buffer[3];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				set_flag(command_buffer[2], command_buffer[3]);
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xff)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xff;
				transmit_value(&status_word[9], 2, 3);
				crc_buffer += 0x04;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = crc_buffer;
				crc_buffer = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xe0)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xe0;
				for(int i = 0; i < 30; ++i)
				{
					transmit_value(&status_word[i], 2, 3);
				}
				crc_buffer += 0xe5;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = crc_buffer;
				crc_buffer = 0;
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xe1)
			{
				for(int i = 0; i < 30; ++i)
				{
					read_value(&status_word[i],2);
				}
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				for(int i = 0; i < 30; ++i)
				{
					for(int j = 0; j < 2; ++j)
					{
						for(int k = 0; k <= 7500; ++k);
						flash_write(0, i * 2 + j, *((char*)&status_word[i] + j));
					}
				}
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xe1;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xe6;
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xe2)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xe2;
				transmit_value(&black_box, 5, 3);
				transmit_value(&black_box_time[0], 2, 3);
				transmit_value(&black_box_time[1], 2, 3);
				crc_buffer += 0xe7;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = crc_buffer;
				crc_buffer = 0;
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xe3)
			{
				read_value(&black_box,5);
				read_value(&black_box_time[0],2);
				read_value(&black_box_time[0],2);
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				for(int i = 60; i < 65; ++i)
				{
						for(int k = 0; k <= 7500; ++k);
						flash_write(0, i, *((char*)&black_box[i - 76]));
				}
				for(int i = 0; i < 2; ++i)
				{
						for(int k = 0; k <= 7500; ++k);
						flash_write(0, i * 2 + 65, *((char*)&black_box_time[i]));
						for(int k = 0; k <= 7500; ++k);
						flash_write(0, i * 2 + 66, *((char*)&black_box_time[i]) + 1);
				}
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0x05;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xe3;
				while ((USART3->SR & USART_SR_TXE) == 0);
				USART3->DR = 0xe8;
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xee)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[1] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				if(command_buffer[2] == 0x00)
					flags |= 0x100;
				clear_command_buffer();

			}
		}
	}
	flags &= ~0x100;
}

void input_generation_parameters_state()
{
	while(1)
	{
/*		flags |= 0x2;
		if(connection_timer > 40)
		{
			__disable_irq();
			GPIOB->BSRR |= GPIO_BSRR_BS_14;
			GPIOB->BSRR |= GPIO_BSRR_BR_13;
			GPIOB->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_10;
			DAC->DHR12R1 = 0;
			DAC->DHR12R2 = 0;
			flags &= ~0x4;
			flags &= ~0x20;
			flags &= ~0x40;
			flags &= ~0x80;
			TIM2->CCR1 = 0;
			TIM4->CCR1 = 0;
			while(1);
		}
*/		if((USART3->SR & USART_SR_RXNE) != 0)
		{
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[0] = USART3->DR;
			crc_summ +=command_buffer[0];
			while((USART3->SR & USART_SR_RXNE) == 0);
			command_buffer[1] = USART3->DR;
			crc_summ += command_buffer[1];
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf0)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				connection_timer = 0;
				connection_check();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf4)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				set_duty_cycle(command_buffer[2]);
				clear_command_buffer();
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf5)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				flags &= ~0x20;
				flags |= 0x200;
				begin_rfid:
				if((flags & 0x400) == 0)
				{
					for(int i = 0; i < 10000; ++i);
					for(int i = 0; i < 33; ++i)
						rfid_buffer[i] = 0;
					send_start_rfid_message();
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x41;
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x30;
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x30;
					while((USART1->SR & USART_SR_TXE) == 0);
					USART1->DR = 0x31;
					for(int i = 0; i < 4; ++i)
					{
						while((USART1->SR & USART_SR_TXE) == 0);
						USART1->DR = 0x30;
					}
					for(int i = 0; i < 16; ++i)
					{
						while((USART1->SR & USART_SR_RXNE) == 0);
						rfid_buffer[i] = USART1->DR;
					}
					if(rfid_buffer[14] == 0x0d && rfid_buffer[15] == 0x0a)
					{
						if(rfid_timer >= 300)
							flags |= 0x400;
						goto begin_rfid;
					}
					for(int i = 16; i < 33; ++i)
					{
						while((USART1->SR & USART_SR_RXNE) == 0);
						rfid_buffer[i] = USART1->DR;
					}
					read_command(2);
					if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf5)
					{
						while ((USART3->SR & USART_SR_TXE) == 0);
						USART3->DR = 0x05;
						while ((USART3->SR & USART_SR_TXE) == 0);
						USART3->DR = 0xf5;
						for(int i = 15; i < 25; ++i)
						{
							while((USART3->SR & USART_SR_TXE) == 0);
							USART3->DR = rfid_buffer[i];
							crc_buffer += rfid_buffer[i];
						}
						crc_buffer += 0xfa;
						while ((USART3->SR & USART_SR_TXE) == 0);
						USART3->DR = crc_buffer;
						crc_buffer = 0;
					}
				}
				rfid_timer = 0;
				flags &= ~0x200;
				flags &= ~0x400;
				clear_command_buffer();
				connection_timer = 0;
				for(int i = 0; i < 33; ++i)
					rfid_buffer[i] = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf6)
			{
				for(int i = 0; i < 8; ++i)
				{
					read_value(&generation_parametrs[i], 2);
				}
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				clear_command_buffer();
				flags &= ~0x2;
				connection_timer = 0;
				ready_state();
				clear_command_buffer();
				connection_timer = 0;
			}
			if(command_buffer[0] == 0x04 && command_buffer[1] == 0xfa)
			{
				while((USART3->SR & USART_SR_RXNE) == 0);
				command_buffer[2] = USART3->DR;
				crc_summ += command_buffer[2];
				while((USART3->SR & USART_SR_RXNE) == 0);
				crc = USART3->DR;
				if(crc_summ != crc)
					flags |= 0x1;
				crc_summ = 0;
				flags &= ~0x20;
				if(command_buffer[2] == 0x00)
					service_menu();
				clear_command_buffer();
				connection_timer = 0;
			}
			clear_command_buffer();
		}
	}
}

void EXTI9_5_IRQHandler()
{
	for(int i = 0; i <= 1000; ++i);
	if (EXTI->PR & (1<<6))
	{
		if((status_word[9] & 0x20) == 0)
			digital_errors |= 0x2;
		EXTI->PR |= EXTI_PR_PR6;
	}
	if (EXTI->PR & (1<<7))
	{
		if((status_word[9] & 0x10) == 0)
			digital_errors |= 0x1;
		EXTI->PR |= EXTI_PR_PR7;
	}
	if (EXTI->PR & (1<<8))
	{
		digital_errors |= 0x40;
		++black_box[2];
		flash_write(0, 78, black_box[2]);
		EXTI->PR |= EXTI_PR_PR8;
	}
	if (EXTI->PR & (1<<9))
	{
		digital_errors |= 0x100;
		++black_box[4];
		flash_write(0, 80, black_box[4]);
		EXTI->PR |= EXTI_PR_PR9;
	}
}

void EXTI15_10_IRQHandler()
{
	for(int i = 0; i <= 1000; ++i);
	if (EXTI->PR & (1<<10))
	{
		digital_errors |= 0x20;
		++black_box[1];
		flash_write(0, 77,black_box[1]);
		EXTI->PR |= EXTI_PR_PR10;
	}
	if (EXTI->PR & (1<<11))
	{
		digital_errors |= 0x80;
		++black_box[3];
		flash_write(0, 79,black_box[3]);
		EXTI->PR |= EXTI_PR_PR11;
	}
	if (EXTI->PR & (1<<12))
	{
		digital_errors |= 0x4;
		EXTI->PR |= EXTI_PR_PR12;
	}
}

int main(void)
{
	init_all();
	// 1 rfid message
	for(int i = 0; i <= 10000; ++i);
	send_start_rfid_message();
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x41;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x46;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x46;
	for(int i = 0; i < 4; ++i)
	{
		while((USART1->SR & USART_SR_TXE) == 0);
		USART1->DR = 0x30;
	}
	for(int i = 0; i < 2; ++i)
	{
		while((USART1->SR & USART_SR_RXNE) == 0);
		rfid_buffer[i] = USART1->DR;
	}
	for(int i = 0; i <= 10000; ++i);
	// 2 rfid message
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x31;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x43;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x33;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x34;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x31;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x32;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x31;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x31;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x39;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x30;
	for(int i = 0; i < 27; ++i)
	{
		while((USART1->SR & USART_SR_RXNE) == 0);
		rfid_buffer[i] = USART1->DR;
	}
	for(int i = 0; i <= 10000; ++i);
	// 3 rfid message
	send_start_rfid_message();
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x46;
	for(int i = 0; i < 7; ++i)
	{
		while((USART1->SR & USART_SR_TXE) == 0);
		USART1->DR = 0x30;
	}
	for(int i = 0; i < 12; ++i)
	{
		while((USART1->SR & USART_SR_RXNE) == 0);
		rfid_buffer[i] = USART1->DR;
	}
	for(int i = 0; i <= 10000; ++i);
	// 4 rfid message
	send_start_rfid_message();
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x46;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x31;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x46;
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = 0x46;
	for(int i = 0; i < 4; ++i)
	{
		while((USART1->SR & USART_SR_TXE) == 0);
		USART1->DR = 0x30;
	}
	for(int i = 0; i < 14; ++i)
	{
		while((USART1->SR & USART_SR_RXNE) == 0);
		rfid_buffer[i] = USART1->DR;
	}
	for(int i = 0; i < 33; ++i)
		rfid_buffer[i] = 0;
/*	if((GPIOC->IDR & 0x40) == 0)
		digital_errors |= 0x2;
	if((GPIOC->IDR & 0x80) == 0)
		digital_errors |= 0x1;
//	if((GPIOC->IDR & 0x100) == 0)
//		digital_errors |= 0x40;
	if((GPIOC->IDR & 0x200) == 0)
		digital_errors |= 0x100;
	if((GPIOA->IDR & 0x400) == 0)
		digital_errors |= 0x20;
	if((GPIOA->IDR & 0x800) == 0)
		digital_errors |= 0x80;
	if((GPIOA->IDR & 0x1000) == 0)
		digital_errors |= 0x4;
	if((GPIOD->IDR & 0x4000) == 0)
		digital_errors |= 0x8;
//	if((GPIOD->IDR & 0x8000) == 0)
//		digital_errors |= 0x10;
*/	//flash
	for(int i = 0; i < 30; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			for(int k = 0; k <= 7500; ++k);
			*((char*)&status_word[i] + j) = flash_read(0, i * 2 + j);
		}
	}
	for(int i = 60; i < 65; ++i)
	{
			for(int k = 0; k <= 7500; ++k);
			*((char*)&black_box[i - 76]) = flash_read(0, i);
	}
	for(int i = 0; i < 2; ++i)
	{
			for(int k = 0; k <= 7500; ++k);
			*((char*)&black_box_time[i]) = flash_read(0, i * 2 + 65);
			for(int k = 0; k <= 7500; ++k);
			*((char*)&black_box_time[i] + 1) = flash_read(0, i * 2 + 66);

	}
	// main
/*	flags |= 0x2;
	crc_summ = 0;
	while((USART3->SR & USART_SR_RXNE) == 0)
	{
		if(connection_timer > 50)
		{
			__disable_irq();
			GPIOB->BSRR |= GPIO_BSRR_BS_14;
			GPIOB->BSRR |= GPIO_BSRR_BR_13;
			GPIOB->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_10;
			DAC->DHR12R1 = 0;
			DAC->DHR12R2 = 0;
			flags &= ~0x4;
			flags &= ~0x20;
			flags &= ~0x40;
			flags &= ~0x80;
			TIM2->CCR1 = 0;
			TIM4->CCR1 = 0;
			error_begin_1:
			read_command(2);
			if(command_buffer[0] != 0x04 && command_buffer[1] != 0xf0)
				goto error_begin_1;
			GPIOB->BSRR |= GPIO_BSRR_BR_14;
		}
	}
	command_buffer[0] = USART3->DR;
	crc_summ +=command_buffer[0];
	while((USART3->SR & USART_SR_RXNE) == 0);
	command_buffer[1] = USART3->DR;
	crc_summ +=command_buffer[1];
	while((USART3->SR & USART_SR_RXNE) == 0);
	crc = USART3->DR;
	if(crc_summ != crc)
		flags |= 0x1;
	crc_summ = 0;
	flags &= ~0x2;
	connection_timer = 0;
*/
	read_command(2);
	if(command_buffer[0] == 0x04 && command_buffer[1] == 0xf0)
	{
		connection_check();
	}
	clear_command_buffer();
 	read_command(2);
	if(command_buffer[0] == 0x04 && command_buffer[1] == 0xe0)
	{
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0x05;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = 0xe0;
		for(int i = 0; i < 30; ++i)
		{
			transmit_value(&status_word[i], 2, 3);
		}
		crc_buffer += 0xe5;
		while ((USART3->SR & USART_SR_TXE) == 0);
		USART3->DR = crc_buffer;
		crc_buffer = 0;
		clear_command_buffer();
	}
	clear_command_buffer();
	input_generation_parameters_state();
}
