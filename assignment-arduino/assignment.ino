// DEBUG
#ifdef DEBUG
  #define TIMING_FACTOR 1000
// PRODUCTION
#else
  #define TIMING_FACTOR 1000
#endif

#define PB1                  1
#define PB2                  2
#define LED1                18
#define LED2                10

#define TON_1              900
#define TOFF               500
#define NUM_PULSES          17
#define IDLE_TIME         6500
#define TSYNC_ON            50
#define MODE                 2

// #define TON_1_ALT         (NUM_PULSES - 1) * (TOFF + TON_1)
// #define TOFF_ALT           500
// #define NUM_PULSES_ALT      17
// #define IDLE_TIME_ALT     6500
// #define TSYNC_ON_ALT        50


bool output_enabled = false;
bool output_selected = false;

void setup() {
  // put your setup code here, to run once:
  // Button
  pinMode(PB1, INPUT);
  pinMode(PB2, INPUT);

  // LED
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  

}

void normal_data_waveform(){
  for (int i = 0; i < NUM_PULSES; i++)
  {
    // pulse on-time
    digitalWrite(LED1, HIGH);
    delayMicroseconds((TON_1 + TSYNC_ON * (i - 1)) * TIMING_FACTOR);

    //pulse off-time
    digitalWrite(LED1, LOW);
    delayMicroseconds(TOFF * TIMING_FACTOR);
  }

  delayMicroseconds(IDLE_TIME * TIMING_FACTOR);

}

void alternative_data_waveform(){
  for (int i = NUM_PULSES; i > 0; i--)
  {
    //pulse on-time
    digitalWrite(LED1, HIGH);
    delayMicroseconds((TON_1 + TSYNC_ON * (i - 1)) * TIMING_FACTOR);

    // pulse off-time
    digitalWrite(LED1, LOW);
    delayMicroseconds(TOFF * TIMING_FACTOR);
  }

  delayMicroseconds(IDLE_TIME * TIMING_FACTOR);

}

void sync_signal(){
    // turn on the SYNC pulse
    digitalWrite(LED2, HIGH);
    delayMicroseconds(TSYNC_ON * TIMING_FACTOR);

    // turn off
    digitalWrite(LED2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  sync_signal();

  // Read the state of push buttons
  bool pb1_state = digitalRead(PB1);
  bool pb2_state = digitalRead(PB2);

  // output enable
  if(pb1_state == HIGH && output_enabled == false)
  {
    output_enabled = true;
    digitalWrite(LED1, HIGH);
  }
  else if(pb1_state == LOW && output_enabled == true)
  {
    output_enabled = false;
    digitalWrite(LED1, LOW);
  }

  // output select
  if(pb2_state == HIGH && output_selected == false)
  {
    output_selected = true;
    alternative_data_waveform();
  }
  else if(pb2_state == LOW && output_selected == true)
  {
    output_selected = false;
    normal_data_waveform();
  }
}