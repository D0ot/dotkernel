#ifndef _GDT_HELPER_H_
#define _GDT_HELPER_H

// only 2 items for low 32 bits
#define DESC_LIMIT_0_15                         0
#define DESC_BASE_0_15                          16

// for high 32 bits
#define DESC_BASE_16_23                         0
#define DESC_LIMIT_16_19                        16
#define DESC_BASE_24_31                         24

#define DESC_AVL_OFFSET                         20

// constants used as operands of "||" operator
#define DESC_TYPE_DATA_RD                       0x00 << 0x08        //  Read-Only
#define DESC_TYPE_DATA_RDA                      0x01 << 0x08        //  Read-Only, accessed
#define DESC_TYPE_DATA_RDWR                     0x02 << 0x08        //  Read/Write
#define DESC_TYPE_DATA_RDWRA                    0x03 << 0x08        //  Read/Write, accessed
#define DESC_TYPE_DATA_RDEXPD                   0x04 << 0x08        //  Read-Only, expand-down
#define DESC_TYPE_DATA_RDEXPDA                  0x05 << 0x08        //  Read-Only, expand-down, accessed
#define DESC_TYPE_DATA_RDWREXPD                 0x06 << 0x08        //  Read/Write, expand-down
#define DESC_TYPE_DATA_RDWREXPDA                0x07 << 0x08        //  Read/Write, expand-down, accessed
#define DESC_TYPE_CODE_EX                       0x08 << 0x08        //  Execute-Only
#define DESC_TYPE_CODE_EXA                      0x09 << 0x08        //  Execute-Only, accessed
#define DESC_TYPE_CODE_EXRD                     0x0A << 0x08        //  Execute/Read
#define DESC_TYPE_CODE_EXRDA                    0x0B << 0x08        //  Execute/Read, accessed
#define DESC_TYPE_CODE_EXC                      0x0C << 0x08        //  Execute-Only, conforming
#define DESC_TYPE_CODE_EXCA                     0x0D << 0x08        //  Execute-Only, conforming, accessed
#define DESC_TYPE_CODE_EXRDC                    0x0E << 0x08        //  Execute/Read, conforming
#define DESC_TYPE_CODE_EXRDCA                   0x0F << 0x08        //  Execute/Read, conforming, accessed
#define DESC_S_SYS                              (0) << 0xc
#define DESC_S_USR                              (1) << 0xc
#define DESC_DPL_0                              (00b) << 0xd
#define DESC_DPL_1                              (01b) << 0xd
#define DESC_DPL_2                              (10b) << 0xd
#define DESC_DPL_3                              (11b) << 0xd
#define DESC_P_CLS                              (0) << 0xf
#define DESC_P_SET                              (1) << 0xf
#define DESC_L_CLS                              (0) << 0x15
#define DESC_L_SET                              (1) << 0x15
#define DESC_DB_32                              (0) << 0x16
#define DESC_DB_16                              (1) << 0x16
#define DESC_G_1B                               (0) << 0x17
#define DESC_G_4K                               (1) << 0x1

#endif