/*
 * Output each byte of data received from serial port
 * 
 * LED turns on if data > 0 and turns off otherwise.
 * 
 * For Micro, RXLED must be turned off. To turn off, open:
 * C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\micro
 * and set lines 33 and 34 to:
 * #define RXLED0 1
 * #define RXLED1 1
 * 
 * See http://forum.arduino.cc/index.php?topic=264569.0 for more details.
 * 
 * 
 * TODO:
 * - serial reading using interrupt.
 * - debugging mode using interrupt.
 * 
 * Kyuhwa Lee and Inaki Iturrate, 2016.
 */

int led_pin= 13; // LED connected to digital pin 13
int debug_pin1= 6; // 52 for Mega
int debug_pin2= 7; // 53 for Mega
int data= 0;
int echo_back= 0;

void setup() {
  // ports init
  pinMode(debug_pin1, INPUT); // sets the Analog Pin as Output
  pinMode(debug_pin2, INPUT); // sets the Analog Pin as Output
  pinMode(led_pin, OUTPUT); // sets the digital pin as output
  digitalWrite(debug_pin1, LOW);
  digitalWrite(debug_pin2, LOW);
  digitalWrite(led_pin, LOW);
  DDRB= B11111111; // set all bits as output
  PORTB= 0;

  // serial init
  Serial.begin(115200);

  // debugging modes
  if ( digitalRead(debug_pin1)==HIGH ) echo_back= 1;
  if ( digitalRead(debug_pin2)==HIGH ) debug_pins();
}

void debug_pins()
{
  int i;
  for (;;)
  {
    for (i=1; i < 255; i*=2)
    {
      PORTB= i;
      Serial.println(PORTB, BIN);
      digitalWrite(led_pin, HIGH);
      delay(500);
  
      PORTB= 0;
      Serial.println(PORTB, BIN);
      digitalWrite(led_pin, LOW);
      delay(500);
    }
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    // read the incoming byte
    data= Serial.read();

    Serial.println(data);

    // write data to digital port
    PORTB= data;

    // echo data back to the client if needed
    if ( echo_back == 1 ) {
      Serial.write(data);
    }

    // turn LED on if non-zero data
    if ( data > 0 )
      digitalWrite(led_pin, HIGH);  // sets the LED on
    else
      digitalWrite(led_pin, LOW);  // sets the LED off
  }
}

