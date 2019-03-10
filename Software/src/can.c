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
   *  - automatic retransmission (NART)
   *  - automatic bus off management (ABOM)
   *  - transmit FIFO priority by order of request (TXFP)
   *  - disable CAN freeze while debugging
   */
  CAN1->MCR &= ~(CAN_MCR_NART  | CAN_MCR_DBF);
  CAN1->MCR |= ( CAN_MCR_AWUM | CAN_MCR_TXFP | CAN_MCR_ABOM );


  /* Set CAN bit timing
   *  - Baud rate prescaler
   *  - Resynchronisation jump width
   *  - Time segment 1
   *  - Time segment 2
   */

  /* Set baud rate prescaler to 3 */
  CAN1->BTR &= ~(CAN_BTR_BRP);
  CAN1->BTR |= ( 3 << CAN_BTR_BRP_Pos);

  /* Set resynchronisation jump width to 1 */
  CAN1->BTR |= CAN_BTR_SJW;

  /* Set Time segment 2 to 1 */
  CAN1->BTR &= ~(CAN_BTR_TS2);
  CAN1->BTR |= CAN_BTR_TS2_0;

  /* Set Time Segment 1 to 12 */
  CAN1->BTR &= ~(CAN_BTR_TS1);
  CAN1->BTR |= (CAN_BTR_TS1_3 | CAN_BTR_TS1_2);

  /* Enable LoopBack mode */
  CAN1->BTR |= CAN_BTR_LBKM;

  /* Leave Initialization mode */
  CAN1->MCR &= ~(CAN_MCR_INRQ);

  /* Wait until CAN peripheral left initialization mode */
  while(CAN1->MSR & CAN_MSR_INAK){}

  /* Set CAN filter options 
   *
   * 
   */

  /* Enable CAN filter initialization mode */
  CAN1->FMR |= CAN_FMR_FINIT;

  /* Disable CAN filter initialization mode */



}