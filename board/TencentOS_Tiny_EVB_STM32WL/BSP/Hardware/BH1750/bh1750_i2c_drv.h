/**
 * @Copyright 			(c) 2019,mculover666 All rights reserved	
 * @filename  			bh1750_i2c_drv.h
 * @breif				Drive BH1750 based on iic1 commucation interface
 * @version
 *            			v1.0    ��ɻ�����������              mculover666    2019/7/15
 * @note            	��ֲ˵�����ǳ���Ҫ����	
 *						1. bh1750_i2c_drv.h�ļ��е�BH1750��д��ַ���ɣ�
 * 						2. ������������ҪSTM32CubeMX���ɵ�I2C��ʼ���ļ�i2c.h��i2c.c֧��;
 * 						3. ÿ��д��ָ��֮����Ҫ��ʱ�ٶ�ȡ���ݣ����ڵ��ڸ�ģʽ�Ĳ���ʱ�䣩��
 */
#ifndef	_BH1750_I2C_DRV_H_
#define	_BH1750_I2C_DRV_H_

#include "stm32wlxx_hal.h"

#define	BH1750_ADDR_WRITE	0x46	//01000110
#define	BH1750_ADDR_READ	0x47	//01000111

typedef enum
{
		POWER_OFF_CMD				=	0x00,	//�ϵ磺�޼���״̬
		POWER_ON_CMD				=	0x01,	//ͨ�磺�ȴ�����ָ��
		RESET_REGISTER				=	0x07,	//�������ּĴ������ڶϵ�״̬�²������ã�
		CONT_H_MODE					=	0x10,	//����H�ֱ���ģʽ����11x�ֱ����¿�ʼ����������ʱ��120ms
		CONT_H_MODE2				=	0x11,	//����H�ֱ���ģʽ2����0.51x�ֱ����¿�ʼ����������ʱ��120ms
		CONT_L_MODE					=	0x13,	//����L�ֱ���ģʽ����411�ֱ����¿�ʼ����������ʱ��16ms
		ONCE_H_MODE					=	0x20,	//һ�θ߷ֱ���ģʽ����11x�ֱ����¿�ʼ����������ʱ��120ms���������Զ�����Ϊ�ϵ�ģʽ
		ONCE_H_MODE2				=	0x21,	//һ�θ߷ֱ���ģʽ2����0.51x�ֱ����¿�ʼ����������ʱ��120ms���������Զ�����Ϊ�ϵ�ģʽ
		ONCE_L_MODE					=	0x23	//һ�εͷֱ���ģʽ����411x�ֱ����¿�ʼ����������ʱ��16ms���������Զ�����Ϊ�ϵ�ģʽ
} BH1750_MODE;

int bh1750_read_data_once(uint16_t *lux);

#endif	/* _AT24C02_I2C_DRV_H_ */
