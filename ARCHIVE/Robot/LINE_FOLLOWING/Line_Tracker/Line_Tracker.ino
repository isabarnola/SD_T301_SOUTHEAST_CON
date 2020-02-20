
/* Line Follower robort - 5 sensors included/ direct port manipulation For Arduino Mega2560 created by: Ali Shwaiheen March 2018*/

#include <avr/io.h> // header file to include input/output port
int  main(void)   //main function
{
unsigned char z;
DDRA = 0x00;  //set PORTA as input port  (DDRA = 0x00 DDRA=0b00000000) same command
DDRB = 0xFF;  //set PORTB as outputport

while(1) // create infinite loop(repeat forver)

{
    z=PINA; // read port A
    switch (z) // make decision
{
case (28): // Fully left 90 degrees
{
PORTB=0x09 ; // wirte High to PB0 & PB3
break;
}
case (30):// Fully left 90 degrees
{
PORTB=0x09;
break;
}
case (7):// Fully right 90 degrees
{
PORTB=0X06; // wirte High to PB1 & PB2
break;
}
case (15):// Fully right 90 degrees
{
PORTB=0x06;
break;
}
case (8):// Slightly left. 
{
PORTB=0x02; // wirte High to PB1
break;
}
case (12):// Slightly left.
{
PORTB=0x02;
break;
}
case (16):// Slightly left.
{
PORTB=0x02;
break;
}
case (24):// Slightly left.
{
PORTB=0x02;
break;
}
case (1):// Slightly Right.
{
PORTB=0x08;// wirte High to PB3
break;
}
case (2): // Slightly Right.
{
PORTB=0x08;
break;
}
case (3):// Slightly Right.
{
PORTB=0x08;
break;
}
case (6):// Slightly Right.
{
PORTB=0x08;
break;
}
case (4): // Straight
{
PORTB=0x0A; // wirte High to PB1 & PB3
break;
}
case (0):// U turn go back to the line
{
PORTB=0x09; // 09 is in hex numbers
break;
}
case (31)://STOP 
{
PORTB=0x00; // Write Low to PB0 to PB7
break;
}
}
}
return 0;
}
