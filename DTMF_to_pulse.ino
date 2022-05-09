/*
Convert output of DTMF converter to pulses for telephone exchange.

for Look Mum No Computer
https://www.patreon.com/posts/im-aware-this-66205770

https://microcontrollerslab.com/mt8870-dtmf-decoder-module-pinout-interfacing-with-arduino-features/#Pin_Configuration

*/

#define PULSE_PIN D7
#define MT8870_STD D2
#define MAX_PULSES 100

const int PULSE_LEN_MS = 50;
const int DIAL_BUFFER_LEN = 32;
const int DIAL_DONE_TIMEOUT_MS = 2000;

char g_dial_buffer[DIAL_BUFFER_LEN] = "";

int buffer_position = 0;

volatile unsigned long last_dialed_time = 0;

#define stq_pin 3 //for nano, can only use D2 or D3 for interrupt pins.
#define q1_pin 4
#define q2_pin 5
#define q3_pin 6
#define q4_pin 7

// Declare functions
void dtmf_interrupt();
void hang_up();
char read_dtmf_inputs();
void clear_buffer();
void pulse(int);


void setup() {
  // Attach interrupt to the MT8870 StD pin
  attachInterrupt(digitalPinToInterrupt(stq_pin), dtmf_interrupt, FALLING);


  pinMode(stq_pin, INPUT); // connect to Std pin
  pinMode(q4_pin, INPUT); // connect to Q4 pin
  pinMode(q3_pin, INPUT); // connect to Q3 pin
  pinMode(q2_pin, INPUT); // connect to Q2 pin
  pinMode(q1_pin, INPUT); // connect to Q1 pin

  // On bootup set exchange pin high
  hang_up();
}

void loop() {
  unsigned long now = millis();

  if (last_dialed_time == 1) {
    last_dialed_time = now;

    // DTMF character ready
    g_dial_buffer[buffer_position] = read_dtmf_inputs();
    buffer_position++;
  }

  // If enough time has elapsed since last DTMF, send buffer via pulses
  if ((now - last_dialed_time) > DIAL_DONE_TIMEOUT_MS) {
    pulse_exchange(g_dial_buffer, buffer_position);
    
    buffer_position = 0;
    clear_buffer();
  }
}


void pulse_exchange(char buf[], int idx) {
  for(int i = 0; i < idx; i++) {
    switch (buf[idx]) {
      case '#':
        // TODO: how to pulse dial this
        pulse(11);
        break;
      case '*':
        // TODO: how to pulse dial this
        pulse(12);        
        break;
      case '0':
        pulse(10);
        break;
      default:
        int count = buf[idx] - '0';
        pulse(count);
        break;
  }
}

void pulse(int num) {
  if (num <= 0 || num > MAX_PULSES) {
    Serial.print("I don't want to pulse ");
    Serial.print(num);
    Serial.print(" times. (Out of Range)");
    return;
  }

  for (int i = 0; i < num; i++) {
    digitalWrite(PULSE_PIN, HIGH);
    delay(PULSE_LEN_MS);
    digitalWrite(PULSE_PIN,LOW);
    delay(PULSE_LEN_MS);
  }
}

void clear_buffer() {
  for (int i = 0; i < DIAL_BUFFER_LEN; i++) {
    g_dial_buffer[i] = 0;
  }
  g_dial_buffer[0] = '\0';
}

void hang_up(){
  digitalWrite(PULSE_PIN, HIGH);
}

void dtmf_interrupt() {
  // Set flag, avoid function calls in interrupt
  last_dialed_time = 1;
}

char read_dtmf_inputs() {
  Serial.println("Hello, I'm in read_dtmf_inputs()!");
  Serial.println();

  uint8_t number_pressed;
  delay(250);

  // checks q1,q2,q3,q4 to see what number is pressed.
  number_pressed = ( 0x00 | (digitalRead(7)<<0) | (digitalRead(6)<<1) | (digitalRead(5)<<2) | (digitalRead(4)<<3) );
  switch (number_pressed)
  {
    case 0x01:
      Serial.println("Button Pressed =  1");
      return '1';
      break;
    case 0x02:
      Serial.println("Button Pressed =  2");
      return '2';
      break;
    case 0x03:
      Serial.println("Button Pressed =  3");
      return '3';
      break;
    case 0x04:
    Serial.println("Button Pressed =  4");
      return '4';
      break;
    case 0x05:
      Serial.println("Button Pressed =  5");
      return '5';
      break;
    case 0x06:
      Serial.println("Button Pressed =  6");
      return '6';
      break;
    case 0x07:
      Serial.println("Button Pressed =  7");
      return '7';
      break;
    case 0x08:
      Serial.println("Button Pressed =  8");
      return '8';
      break;
    case 0x09:
      Serial.println("Button Pressed =  9");
      return '9';
      break;
    case 0x0A:
      Serial.println("Button Pressed =  0");
      return '0';
      break;
    case 0x0B:
      Serial.println("Button Pressed =  *");
      return '*';
      break;
    case 0x0C:
      Serial.println("Button Pressed =  #");
      return '#';
      break;
  }
}

