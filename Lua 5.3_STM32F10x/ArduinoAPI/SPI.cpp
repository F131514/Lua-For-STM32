#include "SPI.h"

SPIClass::SPIClass(SPI_TypeDef* _SPIx)
{
	SPIx = _SPIx;
}

void SPIClass::SPI_Settings(	SPI_TypeDef* SPIx,
															uint16_t SPI_Mode_x,
															uint16_t SPI_DataSize_x,
															uint16_t SPI_MODEx,
															uint16_t SPI_NSS_x,
															uint16_t SPI_BaudRatePrescaler_x,
															uint16_t SPI_FirstBit_x)
{
	u16 SPI_CPOL_x,SPI_CPHA_x;
	SPI_Cmd(SPIx,DISABLE);
	
	switch(SPI_MODEx)
	{
		case 0:
			SPI_CPOL_x = SPI_CPOL_Low;
			SPI_CPHA_x = SPI_CPHA_1Edge;
			break;
		case 1:
			SPI_CPOL_x = SPI_CPOL_Low;
			SPI_CPHA_x = SPI_CPHA_2Edge;
			break;
		case 2:
			SPI_CPOL_x = SPI_CPOL_High;
			SPI_CPHA_x = SPI_CPHA_1Edge;
			break;
		case 3:
			SPI_CPOL_x = SPI_CPOL_High;
			SPI_CPHA_x = SPI_CPHA_2Edge;
			break;
	}

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_x;		//����SPI����ģʽ:(SPI_Mode_Master,SPI_Mode_Slave)
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_x;		//����SPI�����ݴ�С:SPI���ͽ���xλ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_x;		//ѡ���˴���ʱ�ӵ���̬
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_x;	//���ݲ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_x;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����(SPI_NSS_Soft,SPI_NSS_Hard)
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_x;		//���岨����Ԥ��Ƶ��ֵ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_x;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ(SPI_FirstBit_MSB,SPI_FirstBit_LSB)
	SPI_InitStructure.SPI_CRCPolynomial = 3;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPIx, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPIx, ENABLE); //ʹ��SPI����
}

void SPIClass::begin(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	switch((int)SPIx)
	{
		case (int)SPI1:
			RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			break;
		case (int)SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
			RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			break;
	}
	
	SPI_Settings(	SPIx,
								SPI_Mode_Master,
								SPI_DataSize_8b,
								SPI_MODE0,
								SPI_NSS_Soft,
								SPI_BaudRatePrescaler_16,
								SPI_FirstBit_MSB);
}

void SPIClass::begin(uint32_t clock,uint16_t dataOrder,uint16_t dataMode)
{	
	begin();
	setClock(clock);
	setBitOrder(dataOrder);
	setDataMode(dataMode);
}

void SPIClass::beginSlave(void)
{
  begin();
	SPI_Settings(	SPIx,
								SPI_Mode_Slave,
								SPI_DataSize_8b,
								SPI_MODE0,
								SPI_NSS_Hard,
								SPI_BaudRatePrescaler_16,
								SPI_FirstBit_MSB);
}

void SPIClass::end(void)
{
  SPI_Cmd(SPIx, DISABLE);
}

void SPIClass::setClock(uint16_t clock)
{
	uint16_t SPI_BaudRatePrescaler_x;
	uint16_t clock_div = 72000000 / clock;
			 if(clock_div <= 2)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_2;
	else if(clock_div <= 4)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_4;
	else if(clock_div <= 8)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_8;
	else if(clock_div <= 16)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_16;
	else if(clock_div <= 32)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_32;
	else if(clock_div <= 64)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_64;
	else if(clock_div <= 128)SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_128;
	else SPI_BaudRatePrescaler_x = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_x;
	SPI_Init(SPIx, &SPI_InitStructure);
}

void SPIClass::setBitOrder(uint16_t bitOrder)
{
	if(bitOrder)SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//MSBFIRST 1
	else SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_Init(SPIx, &SPI_InitStructure);
}

/*	Victor Perez. Added to test changing datasize from 8 to 16 bit modes on the fly.
*	Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
*	
*/
void SPIClass::setDataSize(uint16_t datasize)
{
	SPI_InitStructure.SPI_DataSize = datasize;
	SPI_Init(SPIx, &SPI_InitStructure);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
/* Notes.  As far as I can tell, the AVR numbers for dataMode appear to match the numbers required by the STM32

From the AVR doc http://www.atmel.com/images/doc2585.pdf section 2.4

SPI Mode 	CPOL 	CPHA 	Shift SCK-edge 	Capture SCK-edge
0 			0 		0 		Falling 		Rising
1 			0 		1 		Rising 			Falling
2 			1 		0 		Rising 			Falling
3 			1 		1 		Falling 		Rising
 
 
On the STM32 it appears to be

bit 1 - CPOL : Clock polarity
    (This bit should not be changed when communication is ongoing)
    0 : CLK to 0 when idle
    1 : CLK to 1 when idle
 
bit 0 - CPHA : Clock phase
    (This bit should not be changed when communication is ongoing)
    0 : The first clock transition is the first data capture edge
    1 : The second clock transition is the first data capture edge
 
If someone finds this is not the case or sees a logic error with this let me know ;-) 
 */
	uint16_t SPI_CPOL_x,SPI_CPHA_x;
	SPI_Cmd(SPIx,DISABLE);
	
	switch(dataMode)
	{
		case 0:
			SPI_CPOL_x = SPI_CPOL_Low;
			SPI_CPHA_x = SPI_CPHA_1Edge;
			break;
		case 1:
			SPI_CPOL_x = SPI_CPOL_Low;
			SPI_CPHA_x = SPI_CPHA_2Edge;
			break;
		case 2:
			SPI_CPOL_x = SPI_CPOL_High;
			SPI_CPHA_x = SPI_CPHA_1Edge;
			break;
		case 3:
			SPI_CPOL_x = SPI_CPOL_High;
			SPI_CPHA_x = SPI_CPHA_2Edge;
			break;
	}
	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_x;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_x;
	SPI_Init(SPIx, &SPI_InitStructure);
}

void SPIClass::beginTransaction(SPISettings settings)
{
	//SPISettings(settings.clock, settings.bitOrder, settings.dataMode);
	//setDataSize(settings.dataSize);
}

void SPIClass::beginTransactionSlave(void)
{
	beginSlave();
}

void SPIClass::endTransaction(void)
{
	//SPI_Cmd(SPIx, DISABLE);
}


uint8_t SPIClass::read(void) 
{
    uint8_t buf[1];
    this->read(buf, 1);
    return buf[0];
}

void SPIClass::read(uint8_t *buf, uint32_t len)
{
  uint32_t rxed = 0;
	
	while (rxed < len)
	{
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		buf[rxed++] = (uint8_t)SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����	
	}
}

void SPIClass::write(uint16_t data)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ		  
	SPI_I2S_SendData(SPIx, data); //ͨ������SPIx����һ������
}

void SPIClass::write(const uint8_t *data, uint32_t length)
{
	uint32_t txed = 0;
	while (txed < length)
	{
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		SPI_I2S_SendData(SPIx, data[txed]);
		txed++;
		//txed += spi_tx(_currentSetting->spi_d, data + txed, length - txed);
	}
}

uint16_t SPIClass::transfer16(uint16_t wr_data) const
{
	uint16_t rd_data;		
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  
	SPI_I2S_SendData(SPIx, wr_data); //ͨ������SPIx����һ������

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ		
	rd_data = SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����	
	return rd_data;
}

uint8_t SPIClass::transfer(uint8_t wr_data) const
{
	uint8_t rd_data;		
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ		  
	SPI_I2S_SendData(SPIx, wr_data); //ͨ������SPIx����һ������

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ	
	rd_data = SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����
	return rd_data;
}

uint8_t SPIClass::send(uint8_t data)
{
	uint8_t buf[] = {data};
	return this->send(buf, 1);
}

uint8_t SPIClass::send(uint8_t *buf, uint32_t len)
{
	uint32_t txed = 0;
	uint8_t ret = 0;
	while (txed < len)
	{
			this->write(buf[txed++]);
			ret = this->read();
	}
	return ret;
}

uint8_t SPIClass::recv(void)
{
    return this->read();
}

SPIClass SPI(SPI1);//SCK-PA5 MISO-PA6 MOSI-PA7
SPIClass SPI_2(SPI2);//SCK-PB13 MISO-PB14 MOSI-PB15
