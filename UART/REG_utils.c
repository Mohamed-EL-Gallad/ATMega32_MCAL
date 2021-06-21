/*
 * REG_utils.c
 *
 *  Created on: 20/06/2021
 *  Author:Mohamed EL-Gallad
 */
#include "STD_types.h"
#include "REG_utils.h"

void SetRegisterBit(u8 Register,u8 BitNo)
{
	Register |=(1<<BitNo);
}

void ClearRegisterBit(u8 Register,u8 BitNo)
{
	Register &= ~(1<<BitNo);
}

u8 GetRegisterBit(u8 const Register, u8 BitNo)
{
	return ((Register & (1<<BitNo))>>BitNo);;
}
