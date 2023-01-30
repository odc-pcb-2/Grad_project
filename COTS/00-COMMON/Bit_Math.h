#ifndef _COMMON_BIT_MATH_H_
#define _COMMON_BIT_MATH_H_

// ****_BIT --> it is the Name of function
// reg --> register name
// bit_num --> the bit number that i want to change the value from 1 to 0 or form 0 to 1

#define SET_BIT(reg, bit_num) reg |= (1 << bit_num)

#define CLR_BIT(reg, bit_num) reg &= (~(1 << bit_num))

#define TOG_BIT(reg, bit_num) reg ^= (1<<bit_num)

#define GET_BIT(reg, bit_num) ((reg&(1<<bit_num))>>bit_num) //(1 & (reg >> bit_num))

#endif
