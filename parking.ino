#include <Servo.h>
#include <Wire.h>

#define PCF8574N_addr	0x20

#define	Open	85			//Posiciones Servos
#define Close	5

#define max_sens	30

#define S1			A0		//Sensores a entradas analogicas
#define S2			A1
#define S3			A2
#define S4			A3
#define S5			A4
#define S6			A5
#define Button_In	  	11	//Botones
#define Button_Out		12
#define Led_G_In	  	10
#define Led_R_In	  	9
#define Led_G_Out	  	8
#define Led_R_Out	  	7


Servo S_In;			//Instanciamos los servos de entrada y salida
Servo S_Out;


int car_cap;			//Capacidad del parking
int Enter_State = 0;	//Estado de las barreras 0 = cerrado 1 = abierto
int Exit_State = 0;



void setup()
{
	pinMode(S1, INPUT);				//Config pines I/O
	pinMode(S2, INPUT);
	pinMode(S3, INPUT);
	pinMode(S4, INPUT);
	pinMode(S5, INPUT);
	pinMode(S6, INPUT);
	pinMode(Button_In, INPUT);
	pinMode(Button_Out, INPUT);
	pinMode(Led_G_In, OUTPUT);
	pinMode(Led_G_Out, OUTPUT);
	pinMode(Led_R_In, OUTPUT);
	pinMode(Led_R_Out, OUTPUT);

	S_In.attach(5);			//Servo entrada a pin 5 
	S_Out.attach(6);		//Servo salida a pin 6

	Wire.begin();			//Inicializamos comunicacion I2C
	Serial.begin(9600);		//Inicializamos comunicacion Serie

	digitalWrite(Led_R_In, HIGH);			//Activamos semaforos en rojo
	digitalWrite(Led_R_Out, HIGH);

	S_Out.write(Close);
	S_In.write(Close);
	delay(500);
	
	


}


void loop()
{
	Exit();
}

void Barrier(int pos, int servo)		//Servo In (1), Servo Out (2)
{
	if(servo == 1)
	{
		S_In.write(pos);
		delay(20);
	} 
	else if(servo == 2)
	{
		S_Out.write(pos);
		delay(20);
	}
}

void display(int num)									//Control de display 7 segmentos con expansor I2C PCF8574N
{
	Wire.beginTransmission(PCF8574N_addr);
	switch(num){										//	   Bit: | 7 | 6  | 5 | 4 | 3 | 2 | 1 | 0 |
		case 0:											// Segment: | a | b  | c | d | e | f | g |   |
			Wire.write(B00000011);						//  Pin IO: | 1 | 10 | 8 | 5 | 4 | 2 | 3 |   |
		break;

		case 1:
			Wire.write(B10011111);
		break;

		case 2:
			Wire.write(B00100101);
		break;

		case 3:
			Wire.write(B00001101);
		break;

		case 4:
			Wire.write(B10011001);
		break;
	}
	Wire.endTransmission();
}

int park_num(void)
{
	int park_cap = 0;
	int sensor = 0;
	int lim = 30;						//limite de sensado del CYN70

	sensor = analogRead(S3);
	if(sensor <= lim)
	{
		park_cap++;
	}

	sensor = analogRead(S4);
	if(sensor <= lim)
	{
		park_cap++;
	}

	sensor = analogRead(S5);
	if(sensor <= lim)
	{
		park_cap++;
	}

	sensor = analogRead(S6);
	if(sensor <= lim)
	{
		park_cap++;
	}

	return park_cap;


}

void debug(int data)
{
	Serial.println(data);
}


void Exit(void)					//En pruebas
{
	int puls;
	puls = digitalRead(Button_Out);
	delay(200);
	if(puls == LOW)
	{
		if(Exit_State == 0)
		{
			digitalWrite(Led_R_Out, LOW);
			digitalWrite(Led_G_Out, HIGH);
			S_Out.write(Open);
			Exit_State = 1;
		}
		else if(Exit_State == 1)
		{
			digitalWrite(Led_R_Out, HIGH);
			digitalWrite(Led_G_Out, LOW);
			S_Out.write(Close);
			Exit_State = 0;
		}

		
	}

}

void Enter(void)					//En pruebas
{
	int puls;
	puls = digitalRead(Button_Out);
	delay(200);
	if(puls == LOW)
	{
		if(Enter_State == 0)
		{
			digitalWrite(Led_R_Out, LOW);
			digitalWrite(Led_G_Out, HIGH);
			S_Out.write(Open);
			Enter_State = 1;
		}
		else if(Enter_State == 1)
		{
			digitalWrite(Led_R_Out, HIGH);
			digitalWrite(Led_G_Out, LOW);
			S_Out.write(Close);
			Enter_State = 0;
		}

		
	}

}