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
int debug_pin1= 52; // can be set to LOW using jumper pin
int debug_pin2= 53; // can be set to LOW using jumper pin
int data= 0;

void setup() {
  // ports init
  DDRB= B11111111; // set all bits as output
  PORTB= 0;
  pinMode(led_pin, OUTPUT); // sets the digital pin as output
  digitalWrite(led_pin, LOW);
  pinMode(debug_pin1, INPUT); // sets the Analog Pin as Output
  pinMode(debug_pin2, INPUT); // sets the Analog Pin as Output
  digitalWrite(debug_pin1, HIGH);
  digitalWrite(debug_pin2, HIGH);

  // serial init
  Serial.begin(115200);

  ///////////////////////////////////////////////////
  //if ( digitalRead(debug_pin2)==LOW ) debug_pins();
  ///////////////////////////////////////////////////
  //debug_pins(); return;
  ///////////////////////////////////////////////////
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

    // write data to digital port
    PORTB= data;

    // echo data back to the client if needed
    if ( digitalRead(debug_pin1)==LOW ) {
      Serial.write(data);
    }

    // turn LED on if non-zero data
    if ( data > 0 )
      digitalWrite(led_pin, HIGH);  // sets the LED on
    else
      digitalWrite(led_pin, LOW);  // sets the LED off
  }

  // enter pin testing mode
  if ( digitalRead(debug_pin2)==LOW ) {
    debug_pins();
  }
}

