/*
Convert output of DTMF converter to pulses for telephone exchange.

for Look Mum No Computer
https://www.patreon.com/posts/im-aware-this-66205770

*/

#define PULSE_PIN A7
#define MAX_PULSES 100

const int PULSE_LEN_MS = 50;
const int DIAL_BUFFER_LEN = 32;
const int DIAL_DONE_TIMEOUT_MS = 2000;

char g_dial_buffer[DIAL_BUFFER_LEN] = "";

int buffer_position = 0;
unsigned long last_dialed_time = 0;


void setup() {
  // setPin for DTMF chip as inputs
  // initialize interrupt service routine for change detection of DTMF?
}

void loop() {
  now = time.millis();

  // If enough time has elapsed since last DTMF, send buffer via pulses
  if ((now - last_dialed_time) > DIAL_DONE_TIOMEOUT_MS) {
    pulse_exchange(g_dial_buffer, buffer_position);
  }

}

void pulse_exchange(char[] buf, int idx) {
  for(int i = 0; i < idx; i++) {
    switch buf[idx]:
      case '0':
        pulse(10);
      case '#':
        // something
      case '*':
        // something
      default:
        int count = atoi(buf[idx]);
        if (count > 0 && count < MAX_PULSES)
          pulse(count);
  }
}

void pulse(int num) {
  for (int i = 0; i < num; i++) {
    digitalWrite(PULSE_PIN, HIGH);
    delay(PULSE_LEN_MS);
    digitalWrite(PULSE_PIN,LOW);
    delay(PULSE_LEN_MS);
  }
}
