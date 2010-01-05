// Définition de types standards
// u32 u16 u8 
// s32 s16 s8
// Si ce n'est pas déjà fait par d'autres .h



// On vérifie si trampoline à déjà défini 
//  les standards dans
//   tpl_os_std_types.h
#ifndef TPL_OS_STD_TYPES_H
#ifndef __STM32F10x_TYPE_H

#define TPL_OS_STD_TYPES_H

/**
 * @typedef u8
 *
 * 8 bits unsigned number
 */
typedef unsigned char   u8;

/**
 * @typedef s8
 *
 * 8 bits signed number
 */
typedef signed char     s8;

/**
 * @typedef u16
 *
 * 16 bits unsigned number
 */
typedef unsigned short  u16;

/**
 * @typedef s16
 *
 * 16 bits signed number
 */
typedef signed short    s16;

/**
 * @typedef u32
 *
 * 32 bits unsigned number
 */
typedef unsigned int   u32;

/**
 * @typedef s32
 *
 * 32 bits signed number
 */
typedef signed int     s32;

/**
 * @typedef u64
 *
 * 64 bits unsigned number
 */
typedef unsigned long   u64;

/**
 * @typedef s64
 *
 * 64 bits signed number
 */
typedef signed long     s64;

#endif /* trampoline */
#endif

// On vérifie si stl32f10x lib  à déjà défini 
//  les standards dans
//   stm32f10x_type.h
#ifndef	__STM32F10x_TYPE_H
#define	__STM32F10x_TYPE_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* stm */


