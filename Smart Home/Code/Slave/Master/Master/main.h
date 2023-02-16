#ifndef MAIN_H_
#define MAIN_H_
		/************************************************************************/
		/*                  include library needed                              */
		/************************************************************************/
#include "Types.h"
		/************************************************************************/
		/*                         Rooms status address                         */
		/************************************************************************/
#define ROOM1_STATUS    0x11
#define ROOM2_STATUS    0x12
#define ROOM3_STATUS    0x13
#define ROOM4_STATUS    0x14
#define ROOM5_STATUS    0x15
#define ROOM6_STATUS    0x16
#define AIR_STATUS		0x17
		/************************************************************************/
		/*                         Rooms status on address                      */
		/************************************************************************/
#define ROOM1_TURN_ON    0x21
#define ROOM2_TURN_ON    0x22
#define ROOM3_TURN_ON    0x23
#define ROOM4_TURN_ON    0x24
#define ROOM5_TURN_ON    0x25
#define ROOM6_TURN_ON    0x26
#define AIR_TURN_ON	     0x27
		/************************************************************************/
		/*                         Rooms status oFF address                     */
		/************************************************************************/
#define ROOM1_TURN_OFF    0x31
#define ROOM2_TURN_OFF    0x32
#define ROOM3_TURN_OFF    0x33
#define ROOM4_TURN_OFF    0x34
#define ROOM5_TURN_OFF    0x35
#define ROOM6_TURN_OFF    0x36
#define AIR_TURN_OFF      0x37
		/************************************************************************/
		/*                         response check address                       */
		/************************************************************************/
#define DEFAULT_ACK    0xFF
#define DEMAND_RESPONSE 0xFF
		/************************************************************************/
		/*                         Status Check                                 */
		/************************************************************************/
#define ON_STATUS   0x01
#define OFF_STATUS  0x00
		/************************************************************************/
		/*                         MENU Address                                 */
		/************************************************************************/
#define MAIN_MENU            1
#define ROOM1_MENU           3
#define ROOM2_MENU           4
#define ROOM3_MENU           5
#define ROOM4_MENU           6
#define ROOM5_MENU           7
#define ROOM6_MENU           8
#define AIR_MENU             9
		/************************************************************************/
		/* function name: u8RecieveByte											*/
		/* input: void (no input)												*/
		/* description: void function that control start the system				*/
		/************************************************************************/
void start_control();
void devices(void);

#endif
