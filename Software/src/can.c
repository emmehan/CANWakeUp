/**************************************************************************//**
 * @file     can.c
 * @brief    Implementation file of the CAN module.
 * @version  V1.0
 * @date     05.03.2019
 ******************************************************************************/

/*
Application for interfacing a TJA1050 CAN transceiver with a STM32F103C8T6 MCU.
Copyright (C) 2019  Jonas Heim
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "can.h"

/**
  \brief    Initializes the can peripherals.
  \details  Enabled clock for CAN CAN1.
            Enable automatic wake up mode on CAN message reception.
            Disable automatic retransmission.
            Set transmit FIFO priority to order of request.
            Set bit timing to 500kbit/s
 */
void can_init(void)
{
	/* Enable clock for CAN peripheral CAN1 */
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

	/* Set CAN initialization mode */
  CAN1->MCR |= CAN_MCR_INRQ;

  /* Wait until CAN peripheral is in initialization mode */
  while(!(CAN1->MSR & CAN_MSR_INAK)){}

  /* Set up CAN bit timing */

  /* Set CAN options
   *  - automatic wake up mode (AWUM)
   *  - no automatic retransmission (NART)
   *  - transmit FIFO priority by order of request (TXFP)
   */
  CAN1->MCR |= ( CAN_MCR_AWUM | CAN_MCR_NART | CAN_MCR_TXFP );

  /* Set CAN bit timing
   * 
   * 
   */

  /* Leave Initialization mode */
  CAN1->MCR &= ~(CAN_MCR_INRQ);

  /* Wait until CAN peripheral left initialization mode */
  while(CAN1->MSR & CAN_MSR_INAK){}

}