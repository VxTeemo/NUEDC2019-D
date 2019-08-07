/***************************************************************************************************
  * 文    件：BITBAND.h    
  * 作    者：longwei
  * 编写日期：2012/3/8
  * 版    本：V1.0
  * 开 发 板：stm32f4-discovery
  * 修改时间：无 
  * 编 译 器: keil.4.23
  *------------------------------------------------------------------------------------------------
  * 简    介：位带别名区的宏定义实现快速的位操作     
  *	注：位段计数公式
  *         bit_word_addr = bit_band_base + bit_word_offset 
  *       	bit_word_offset = (byte_offset x 32) + (bit_number × 4)
  *     	
  *************************************************************************************************/
/* 防止递归包含的定义-----------------------------------------------------------------------------*/
#ifndef __BITBAND_H
#define __BIIBAND_H

#ifdef __cplusplus
 extern "C" {
#endif 
/* 包含 ------------------------------------------------------------------------------------------*/
#include"stm32f4xx.h"
/*控制宏------------------------------------------------------------------------------------------*/
#define GPOIX_BSRR_EN 1 //1：使用BSRR位段设置功能 0：不使用
/*出口宏定义--------------------------------------------------------------------------------------*/
/*IO口寄存器偏量*/
#define GPIOX_ODR_OFFSET     0X0014
#define GPIOX_IDR_OFFSET     0X0010

#if GPOIX_BSRR_EN==1
#define GPIOX_BSRR_OFFSET    0X0018
#endif

/*GPIOX的各寄存器的开始地址偏量*/
#define GPIOA_BASE_OFFSET          (GPIOA_BASE - PERIPH_BASE)
#define GPIOB_BASE_OFFSET          (GPIOB_BASE - PERIPH_BASE)
#define GPIOC_BASE_OFFSET          (GPIOC_BASE - PERIPH_BASE)
#define GPIOD_BASE_OFFSET          (GPIOD_BASE - PERIPH_BASE)
#define GPIOE_BASE_OFFSET          (GPIOE_BASE - PERIPH_BASE)
#define GPIOF_BASE_OFFSET          (GPIOF_BASE - PERIPH_BASE)
#define GPIOG_BASE_OFFSET          (GPIOG_BASE - PERIPH_BASE)
#define GPIOH_BASE_OFFSET          (GPIOH_BASE - PERIPH_BASE)
#define GPIOI_BASE_OFFSET          (GPIOI_BASE - PERIPH_BASE)
/*GPIOX_ODR的位带别名区地址*/
#define GPIOA_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOA_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOB_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOB_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOC_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOC_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOD_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOD_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOE_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOE_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOF_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOF_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOG_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOG_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOH_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOH_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
#define GPIOI_ODR_OFFSET         (PERIPH_BB_BASE + ((GPIOI_BASE_OFFSET + GPIOX_ODR_OFFSET)<<5))
/*GPIOX_IDR的位带别名区地址*/
#define GPIOA_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOA_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOB_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOB_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOC_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOC_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOD_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOD_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOE_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOE_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOF_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOF_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOG_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOG_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOH_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOH_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))
#define GPIOI_IDR_OFFSET         (PERIPH_BB_BASE + ((GPIOI_BASE_OFFSET + GPIOX_IDR_OFFSET)<<5))

#if GPOIX_BSRR_EN==1
/*GPIOX_BSRR的位带别名区地址*/
#define GPIOA_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOA_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOB_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOB_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOC_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOC_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOD_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOD_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOE_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOE_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOF_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOF_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOG_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOG_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOH_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOH_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#define GPIOI_BSRR_OFFSET         (PERIPH_BB_BASE + ((GPIOI_BASE_OFFSET + GPIOX_BSRR_OFFSET)<<5))
#endif

//GPIOX的位段方式写
#define GPIOA_OUT(n)     *((__IO uint32_t *)(GPIOA_ODR_OFFSET+(n<<2)))
#define GPIOB_OUT(n)	 *((__IO uint32_t *)(GPIOB_ODR_OFFSET+(n<<2)))
#define GPIOC_OUT(n)	 *((__IO uint32_t *)(GPIOC_ODR_OFFSET+(n<<2)))
#define GPIOD_OUT(n)	 *((__IO uint32_t *)(GPIOD_ODR_OFFSET+(n<<2)))
#define GPIOE_OUT(n)	 *((__IO uint32_t *)(GPIOE_ODR_OFFSET+(n<<2)))
#define GPIOF_OUT(n)	 *((__IO uint32_t *)(GPIOF_ODR_OFFSET+(n<<2)))
#define GPIOG_OUT(n)	 *((__IO uint32_t *)(GPIOG_ODR_OFFSET+(n<<2)))
#define GPIOH_OUT(n)	 *((__IO uint32_t *)(GPIOH_ODR_OFFSET+(n<<2)))
#define GPIOI_OUT(n)	 *((__IO uint32_t *)(GPIOI_ODR_OFFSET+(n<<2)))
//GPIOX的位段方式读
#define GPIOA_IN(n)      *((__IO uint32_t *)(GPIOA_IDR_OFFSET+(n<<2)))
#define GPIOB_IN(n)	     *((__IO uint32_t *)(GPIOB_IDR_OFFSET+(n<<2)))
#define GPIOC_IN(n)	     *((__IO uint32_t *)(GPIOC_IDR_OFFSET+(n<<2)))
#define GPIOD_IN(n)	     *((__IO uint32_t *)(GPIOD_IDR_OFFSET+(n<<2)))
#define GPIOE_IN(n)	     *((__IO uint32_t *)(GPIOE_IDR_OFFSET+(n<<2)))
#define GPIOF_IN(n)	     *((__IO uint32_t *)(GPIOF_IDR_OFFSET+(n<<2)))
#define GPIOG_IN(n)	     *((__IO uint32_t *)(GPIOG_IDR_OFFSET+(n<<2)))
#define GPIOH_IN(n)	     *((__IO uint32_t *)(GPIOH_IDR_OFFSET+(n<<2)))
#define GPIOI_IN(n)	     *((__IO uint32_t *)(GPIOI_IDR_OFFSET+(n<<2)))

#if GPOIX_BSRR_EN==1
//GPIOX的位段方式RESET
#define GPIOA_RESET(n)       *((__IO uint32_t *)(GPIOA_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOB_RESET(n)	     *((__IO uint32_t *)(GPIOB_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOC_RESET(n)	     *((__IO uint32_t *)(GPIOC_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOD_RESET(n)	     *((__IO uint32_t *)(GPIOD_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOE_RESET(n)	     *((__IO uint32_t *)(GPIOE_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOF_RESET(n)	     *((__IO uint32_t *)(GPIOF_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOG_RESET(n)	     *((__IO uint32_t *)(GPIOG_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOH_RESET(n)	     *((__IO uint32_t *)(GPIOH_BSRR_OFFSET+64+(n<<2)))=1
#define GPIOI_RESET(n)	     *((__IO uint32_t *)(GPIOI_BSRR_OFFSET+64+(n<<2)))=1
//GPIOX的位段方式SET
#define GPIOA_SET(n)         *((__IO uint32_t *)(GPIOA_BSRR_OFFSET+(n<<2)))=1
#define GPIOB_SET(n)	     *((__IO uint32_t *)(GPIOB_BSRR_OFFSET+(n<<2)))=1
#define GPIOC_SET(n)	     *((__IO uint32_t *)(GPIOC_BSRR_OFFSET+(n<<2)))=1
#define GPIOD_SET(n)	     *((__IO uint32_t *)(GPIOD_BSRR_OFFSET+(n<<2)))=1
#define GPIOE_SET(n)	     *((__IO uint32_t *)(GPIOE_BSRR_OFFSET+(n<<2)))=1
#define GPIOF_SET(n)	     *((__IO uint32_t *)(GPIOF_BSRR_OFFSET+(n<<2)))=1
#define GPIOG_SET(n)	     *((__IO uint32_t *)(GPIOG_BSRR_OFFSET+(n<<2)))=1
#define GPIOH_SET(n)	     *((__IO uint32_t *)(GPIOH_BSRR_OFFSET+(n<<2)))=1
#define GPIOI_SET(n)	     *((__IO uint32_t *)(GPIOI_BSRR_OFFSET+(n<<2)))=1
#endif

#ifdef __cplusplus
}
#endif

#endif
/******************* (C) 版权 2012 longwei *****文件结束*******************************************/ 
