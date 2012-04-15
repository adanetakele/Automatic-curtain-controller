// ADC0-- LDR
//ADC1-- reference

/*
pc4 enable motor
pc5 m1 cw 
pc3 m2 cw
PC2 M2 CCW
PD0 M1 CCW

PB3---MODE SELECT----0=>AUTO 1=> MANUAL
PB4---OPEN
PB5--CLOSE
*/
#define F_CPU 1000000UL
#include<avr/io.h>
#include<util/delay.h>

void ADC_Init()
{
  //select AVCC reference voltage , left alignment of data and ADC0
  ADMUX=((1<<REFS0)|(1<<ADLAR));

  //enable ADC, set prescaler to divide the clock by 128 and auto triggering mode
 ADCSRA=((1<<ADEN)|(1<<ADFR)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0));


}



int ADC_Read()
{
  //start conversion
  ADCSRA|=(1<<ADSC);
   _delay_ms(10);

  //return the value of ADCH
  return ADCH;
}

uint8_t ADC0()
{
  ADMUX&=(~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)));   // SELECT ADC0
 _delay_ms(5);
  return ADC_Read();
}
  
uint8_t ADC1()
{
  ADMUX|=(1<<MUX0);   // SELECT ADC1
 _delay_ms(5);
  return ADC_Read();
}



main()
{
 
//make these pins outputs 
DDRC|=((1<<DDC2)|(1<<DDC3)|(1<<DDC4)|(1<<DDC5));
DDRD|=(1<<DDD0);

//USART_Init();
//uint8_t data0, data1;

ADC_Init();


while(1)
{
/*
*/
if(PINB&(1<<PB3))//PD0 high****************** manual mode

{
  
  PORTC&=(~((1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)));
		  PORTD&=(~(1<<PD0));

	if(PINB&(1<<PB4)) //PB4 high

		{
			 PORTC&=(~(1<<PC2));
                        PORTD&=(~(1<<PD0));

			PORTC|=((1<<PC4)|(1<<PC3)|(1<<PC5));  // = ***************open window
		_delay_ms(100);
		 PORTC&=(~((1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)));
		  PORTD&=(~(1<<PD0));
		}

	if(PINB&(1<<PB5)) //PB5 high
		{
			  PORTC&=(~((1<<PC3)|(1<<PC5)));
			PORTD|=(1<<PD0);
			PORTC|=((1<<PC2)|(1<<PC4));
			_delay_ms(100);
		 PORTC&=(~((1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)));
		  PORTD&=(~(1<<PD0));
		}

//break;

}
//while(~(PINB&(1<<PB3)))           //k****************automatic mode
else
{

//while(1)
//{
/*
		_delay_ms(100);
		ADMUX=((1<<REFS0)|(1<<ADLAR)|(0<<MUX0));
		_delay_ms(40);
		data0=ADC_Read();
		

		_delay_ms(100);
		ADMUX=((1<<REFS0)|(1<<ADLAR)|(1<<MUX0));
		_delay_ms(40);
		data1=ADC_Read();
	
*/
		while(ADC0()<(ADC1()-0x15))
			{
			  //USART_putstring("ADC0 low");
			  //USARTWriteChar();
			PORTC&=(~(1<<PC2));
                        PORTD&=(~(1<<PD0));

			PORTC|=((1<<PC4)|(1<<PC3)|(1<<PC5));  // = ***************open window
			break;
//			
			}

		while(ADC0()>(ADC1()+0x15))  //CLOSE WINDOW
			{
//			  USART_putstring("ADC0 HIGH\r\n");
			PORTC&=(~((1<<PC3)|(1<<PC5)));
			PORTD|=(1<<PD0);
			PORTC|=((1<<PC2)|(1<<PC4));
				break;
			}
		while(((ADC1()-0X15)<ADC0())&(ADC0()<(ADC1()+0x15)))
		{
		  PORTC&=(~((1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)));
		  PORTD&=(~(1<<PD0));
		  break;
		}
  
//	    break;

}
}

		}	
