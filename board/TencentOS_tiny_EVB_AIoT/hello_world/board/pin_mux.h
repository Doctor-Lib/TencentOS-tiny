/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK 0x04U /*!< GPIO3 and GPIO8 share same IO MUX function, GPIO_MUX3 selects one GPIO function: affected bits mask */

/* GPIO_AD_B0_13 (coord L14), UART1_RXD */
/* Routed pin properties */
#define BOARD_INITPINS_UART1_RXD_PERIPHERAL                              LPUART1   /*!< Peripheral name */
#define BOARD_INITPINS_UART1_RXD_SIGNAL                                       RX   /*!< Signal name */

/* GPIO_AD_B0_12 (coord K14), UART1_TXD */
/* Routed pin properties */
#define BOARD_INITPINS_UART1_TXD_PERIPHERAL                              LPUART1   /*!< Peripheral name */
#define BOARD_INITPINS_UART1_TXD_SIGNAL                                       TX   /*!< Signal name */

/* GPIO_SD_B1_02 (coord M3), FlexSPI_D1_B */
/* Routed pin properties */
#define BOARD_INITPINS_FlexSPI_D1_B_PERIPHERAL                             GPIO3   /*!< Peripheral name */
#define BOARD_INITPINS_FlexSPI_D1_B_SIGNAL                               gpio_io   /*!< Signal name */
#define BOARD_INITPINS_FlexSPI_D1_B_CHANNEL                                   2U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_FlexSPI_D1_B_GPIO                                   GPIO3   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_FlexSPI_D1_B_GPIO_PIN                                  2U   /*!< GPIO pin number */
#define BOARD_INITPINS_FlexSPI_D1_B_GPIO_PIN_MASK                     (1U << 2U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_FlexSPI_D1_B_PORT                                   GPIO3   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_FlexSPI_D1_B_PIN                                       2U   /*!< PORT pin number */
#define BOARD_INITPINS_FlexSPI_D1_B_PIN_MASK                          (1U << 2U)   /*!< PORT pin mask */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
