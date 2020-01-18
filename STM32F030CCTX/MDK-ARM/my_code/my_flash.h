#ifndef MY_FLASH_H
#define	MY_FLASH_H

#include "stm32f0xx_hal.h"
#include "my_global.h"
#define  SW_RESET()	       NVIC_SystemReset()   //系统复位

#define STM32F030CCTX_FLASH         //256K      128 * 2K

//如果使用STM32F030C6T6,请修改下面的宏定义
//#define STM32F030C8T6_FLASH    //flash = 64k
//#define STM32F030C6T6_FLASH    //flash = 32k

#define STM32_FLASH_BASE        0x08000000
#define STM32_FLASH_SIZE        256
#define STM32_FLASH_SECTOR_SIZE 2048
#define FLASH_WAITETIME         5000

#ifdef STM32F030C8T6_FLASH
/*FLASH存储区*/
#define FLASH_ONE_START_ADDR   ((uint32_t)0x0800f000)   /* FLASH第1页 1K字节0x0800f000~0x0800f3ff */
/*FLASH备份区*/
#define FLASH_TWO_START_ADDR   ((uint32_t)0x0800f400)   /* FLASH第2页 1K字节0x0800f400~0x0800f7ff */
#endif
#ifdef STM32F030C6T6_FLASH
/*FLASH存储区*/	 
#define FLASH_ONE_START_ADDR   ((uint32_t)0x08007000)   /* FLASH第1页 */
/*FLASH备份区*/
#define FLASH_TWO_START_ADDR   ((uint32_t)0x08007400)   /* FLASH第2页 */
#endif

#ifdef STM32F030CCTX_FLASH
#define FLASH_ONE_START_ADDR    ((uint32_t)0x0803F000)   //page 126
#define FLASH_TWO_START_ADDR    ((uint32_t)0x0803F800)   //page 127
#endif

#define ADDR_SIZE          4

u8 FlashSaveOnePage(SystemParameter_TypeDef SSPT);
u8 FlashSaveTwoPage(SystemParameter_TypeDef SSPT);
u8 LoadOnePage(SystemParameter_TypeDef *SSPT);
u8 LoadTwoPage(SystemParameter_TypeDef *SSPT);
HAL_StatusTypeDef DataBackup(void);
void Flash_Check(void);
void Flash_XorCRC(SystemParameter_TypeDef *SSPT);
extern void FLASH_PageErase(uint32_t PageAddress);



u8 stmFlash_write(uint32_t writeAddr, uint16_t *pBuf, uint16_t num);
void stmFlash_read(u32 readAddr, u16 *pbuf, u16 num);

#endif

