/*
 * REG_utils.h
 *
 *  Created on: 20/06/2021
 *  Author: Mohamed EL-Gallad
 */

#ifndef REG_UTILS_H_
#define REG_UTILS_H_

#define SetRegisterBit(Register,BitNo)     ((Register) |=(1<<(BitNo)))
#define ClearRegisterBit(Register,BitNo)   ((Register) &= ~(1<<(BitNo)))
#define GetRegisterBit(Register,BitNo)     (((Register) & (1<<(BitNo)))>>(BitNo))


#endif /* REG_UTILS_H_ */
