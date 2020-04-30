; make my life easy


; constants used as rhs operands of "<<" operator
DESC_BASE_OFFSET1       equ         16          ; length 16
DESC_BASE_OFFSET2       equ         0           ; length 8
DESC_BASE_OFFSET3       equ         24          ; length 8
DESC_AVL_OFFSET         equ         20


; constants used as operands of "||" operator
DESC_TYPE_DATA_RD       equ         0x00 << 0x08 ;  Read-Only
DESC_TYPE_DATA_RDA      equ         0x01 << 0x08 ;  Read-Only, accessed
DESC_TYPE_DATA_RDWR     equ         0x02 << 0x08 ;  Read/Write
DESC_TYPE_DATA_RDWRA    equ         0x03 << 0x08 ;  Read/Write, accessed
DESC_TYPE_DATA_RDEXPD   equ         0x04 << 0x08 ;  Read-Only, expand-down
DESC_TYPE_DATA_RDEXPDA  equ         0x05 << 0x08 ;  Read-Only, expand-down, accessed
DESC_TYPE_DATA_RDWREXPD equ         0x06 << 0x08 ;  Read/Write, expand-down
DESC_TYPE_DATA_RDWREXPDA equ        0x07 << 0x08 ;  Read/Write, expand-down, accessed
DESC_TYPE_CODE_EX       equ         0x08 << 0x08 ;  Execute-Only
DESC_TYPE_CODE_EXA      equ         0x09 << 0x08 ;  Execute-Only, accessed
DESC_TYPE_CODE_EXRD     equ         0x0A << 0x08 ;  Execute/Read
DESC_TYPE_CODE_EXRDA    equ         0x0B << 0x08 ;  Execute/Read, accessed
DESC_TYPE_CODE_EXC      equ         0x0C << 0x08 ;  Execute-Only, conforming
DESC_TYPE_CODE_EXCA     equ         0x0D << 0x08 ;  Execute-Only, conforming, accessed
DESC_TYPE_CODE_EXRDC    equ         0x0E << 0x08 ;  Execute/Read, conforming
DESC_TYPE_CODE_EXRDCA   equ         0x0F << 0x08 ;  Execute/Read, conforming, accessed


DESC_S_SYS              equ         (0) << 0xc
DESC_S_USR              equ         (1) << 0xc

DESC_DPL_0              equ         (00b) << 0xd
DESC_DPL_1              equ         (01b) << 0xd
DESC_DPL_2              equ         (10b) << 0xd
DESC_DPL_3              equ         (11b) << 0xd

DESC_P_CLS              equ         (0) << 0xf
DESC_P_SET              equ         (1) << 0xf

DESC_L_CLS              equ         (0) << 0x15
DESC_L_SET              equ         (1) << 0x15

DESC_DB_D               equ         (0) << 0x16
DESC_DB_B               equ         (1) << 0x16

DESC_G_1B               equ         (0) << 0x17
DESC_G_4K               equ         (1) << 0x17
