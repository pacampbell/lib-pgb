#ifndef PGB_CPU_PRIVATE_LR35902_APPENDIX_H
#define PGB_CPU_PRIVATE_LR35902_APPENDIX_H

#define LR35902_OPCODE_ADC_A_REG_FIELD_ADDEND                               2:0
#define LR35902_OPCODE_LD_REG_REG_FIELD_SRC                                 2:0
#define LR35902_OPCODE_LD_REG_REG_FIELD_DST                                 5:3
#define LR35902_OPCODE_LD_REG16_UINT16_DST                                  2:0

#define LR35902_OPCODE_FIELD_X                                              7:6
#define LR35902_OPCODE_FIELD_Y                                              5:3
#define LR35902_OPCODE_FIELD_Z                                              2:0
#define LR35902_OPCODE_FIELD_P                                              5:4
#define LR35902_OPCODE_FIELD_Q                                              3:3

/*
d = int8
n = uint8
nn = uint16
tab[x] tabled value x
*/


/*
#define LR35902_OPCODE_FIELD_X_CLASS_LD                                     1
#define LR35902_OPCODE_FIELD_X_CLASS_ARITHMETIC                             2
#define LR35902_OPCODE_FIELD_X_CLASS_CONTROL                                3
#define LR35902_OPCODE_CLASS_MEM                                            7:6
#define LR35902_OPCODE_CLASS_MEM_LD                                         0x1

#define LR35902_OPCODE_CLASS_ARITHMETIC                                     7:4
#define LR35902_OPCODE_CLASS_ARITHMETIC_ADD                                 8
#define LR35902_OPCODE_CLASS_ARITHMETIC_SUB                                 9

#define LR35902_OPCODE_CLASS_BITWISE_ARITHMETIC                             7:3
#define LR35902_OPCODE_CLASS_BITWISE_ARITHMETIC_AND                         20
#define LR35902_OPCODE_CLASS_BITWISE_ARITHMETIC_XOR                         21
#define LR35902_OPCODE_CLASS_BITWISE_ARITHMETIC_OR                          22
#define LR35902_OPCODE_CLASS_BITWISE_ARITHMETIC_CP                          23
*/


#endif /* PGB_CPU_PRIVATE_LR35902_APPENDIX_H */
