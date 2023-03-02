#include "menu.h"

//extern volatile uint16 session_counter;//Stores the session time
//extern uint8 timeout_flag;//stores if the session is still valid or outdated

u8 ui8ComparePass(const u8* pass1,const u8* pass2,const u8 size)
{
	u8 pin_counter=0;//the offset of the character of the pin
	u8 ret_value = TRUE;
	while (pin_counter<size)//loop till the pin is fully entered
	{
		if (*(pass1+pin_counter)!=*(pass2+pin_counter))//if the two characters of the entered pin and the stored pin are not equal
		{
			ret_value= FALSE;//returns false that means the two PINs are not equal
			break;//break the loop
		}
		pin_counter++;//increase thee offset by one
	}
	return ret_value;//returns false that means the two PINs are equal since all characters of the PINs are equal
}

void vMenuOption(const u8 u8SelectedRoom,const u8 u8LoginMode)
{
	u8 u8StatusCode  = 0;//demand status code
	u8 u8TurnOnCode  = 0;//turn on the room or the device code
	u8 u8TurnOffCode = 0;//turn off the device or room
	u8 response      = DEFAULT_ACK;//the response of the slave that is sent back based on the command of the master
	u8 key_pressed   = NOT_PRESSED;//the key that is entered by the user
	
	do
	{
		LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		
		/*********** ****************Set the commands to be sent to the slave based on  the sent room or device ******************************/
		switch(u8SelectedRoom)
		{
			case ROOM1_MENU:
			u8StatusCode = ROOM1_STATUS;
			u8TurnOnCode = ROOM1_TURN_ON;
			u8TurnOffCode = ROOM1_TURN_OFF;
			LCD_vidDisplayString("Room1 S:");
			break;
			case ROOM2_MENU:
			u8StatusCode = ROOM2_STATUS;
			u8TurnOnCode = ROOM2_TURN_ON;
			u8TurnOffCode = ROOM2_TURN_OFF;
			LCD_vidDisplayString("Room2 S:");
			break;
			case ROOM3_MENU:
			u8StatusCode = ROOM3_STATUS;
			u8TurnOnCode = ROOM3_TURN_ON;
			u8TurnOffCode = ROOM3_TURN_OFF;
			LCD_vidDisplayString("Room3 S:");
			break;
			case ROOM4_MENU:
			u8StatusCode = ROOM4_STATUS;
			u8TurnOnCode = ROOM4_TURN_ON;
			u8TurnOffCode = ROOM4_TURN_OFF;
			LCD_vidDisplayString("Room4 S:");
			break;
			case TV_MENU:
			u8StatusCode = TV_STATUS;
			u8TurnOnCode = TV_TURN_ON;
			u8TurnOffCode = TV_TURN_OFF;
			LCD_vidDisplayString("TV S:");
			break;
			case AIRCOND_CTRL_MENU:
			u8StatusCode = AIR_COND_STATUS;
			u8TurnOnCode = AIR_COND_TURN_ON;
			u8TurnOffCode = AIR_COND_TURN_OFF;
			LCD_vidDisplayString("Air Cond. S:");
			break;
		}
		/****************************************************************************************************/
		
		SPI_u8SendReceive(u8StatusCode);//demand the status from the slave
		_delay_ms(100);//Halt the system for the given time in (ms)
		response = SPI_u8SendReceive(DEMAND_RESPONSE);//the master micro controller asks for the status
		if(response == ON_STATUS)//if the response from the slave was on status
		{
			LCD_vidDisplayString("ON");//print the status on
		}
		else//if the response from the slave was off status
		{
			LCD_vidDisplayString("OFF");//print the status off
		}

		LCD_vidMoveCursor(2,1);//move the cursor to the start of the second line
		LCD_vidDisplayString("1-On 2-Off 0-RET");//print the sub menu
		
		/*******************************************************************************/
		
		key_pressed = u8GetKeyPressed(u8LoginMode);//wait for the user till key is pressed or the time is out
		_delay_ms(100);//to avoid the duplication of the pressed key
		
		/*there is no need to take any action in case of the user pressed 0(RET) key
		breaking the loop will be enough since it will be handled in the main*/
		
		if (key_pressed == '1')
		{
			SPI_u8SendReceive(u8TurnOnCode);//Send turn on signal from master to slave
		}
		else if (key_pressed == '2')
		{
			SPI_u8SendReceive(u8TurnOffCode);//Send turn off signal from master to slave
		}
		else if( (key_pressed != NOT_PRESSED) && (key_pressed != '0') )//show wrong input message if the user entered non numeric value
		{
			LCD_vidClearScreen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
			LCD_vidDisplayString("Wrong input");//print error message
			_delay_ms(500);//Halt the system for the given time in (ms)
		}
	} while ( ( (key_pressed < '0') || (key_pressed > '2') ) /*&& (timeout_flag == FALSE)*/);//break the loop in case of valid key or time is out
}

u8 u8GetKeyPressed(const u8 u8LoginMode)
{
	u8 key_pressed = NOT_PRESSED;
	while (key_pressed == NOT_PRESSED)//repeat till the user press any key
	{
		if ( (/*session_counter >= ADMIN_TIMEOUT  && */ u8LoginMode == ADMIN) || ( /*session_counter >= GUEST_TIMEOUT && */ u8LoginMode == GUEST ))//check for timeout
		{
			//timeout_flag = TRUE;//set timeout flag to true
			break;//break the loop that wait for input from the user
		}
		
		key_pressed = KPD_u8GetPressedKey();//if the user pressed any button in keypad save the value in key_pressed
	}
	return key_pressed;
}