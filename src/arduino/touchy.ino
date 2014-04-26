// Adapted from https://github.com/Illutron/AdvancedTouchSensing

#define SET(x,y) (x |=(1<<y))     // Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y))) // |
#define CHK(x,y) (x & (1<<y))     // |
#define TOG(x,y) (x^=(1<<y))      // +

#define NUMFREQS 200

float freqs[NUMFREQS];
float vals[NUMFREQS];

void _send(int index, float value) {
  // index,value,
  Serial.print(index);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
}

void sendReadings(float freqs[], float vals[]) {
  for(int i = 0; i < NUMFREQS; i++) {
    _send(freqs[i], vals[i]);
  }

  // Terminate with ASCII Line Feed
  Serial.write(10);

  TOG(PORTB, 0);
}

void getReadings() {
  for(int i = 0; i < NUMFREQS; i++) {
    int val = analogRead(0);

    CLR(TCCR1B,0); // Stop generator

    TCNT1 = 0;     // Reload new frequency
    ICR1  = i;     // |
    OCR1A = i / 2; // +

    SET(TCCR1B, 0); // Restart generator

    // Filter results
    vals[i] = vals[i] * 0.5 + (float)(val) * 0.5;

    freqs[i] = i;
  }
}

void setupGenerator() {
  TCCR1A = 0b10000010; // Set up frequency generator
  TCCR1B = 0b00011001; // +
  ICR1   = 110;
  OCR1A  = 55;

  pinMode(9, OUTPUT);  // Signal generator pin
  pinMode(8, OUTPUT);  // Sync (test) pin
}

void initVals() {
  for(int i = 0; i < NUMFREQS; i++) {
    vals[i] = 0;
  }
}

void setup() {
  setupGenerator();
  initVals();

  Serial.begin(115200);
}

void loop() {
  //_send(random(1,20), random(20, 30));

  // Assigns to freqs[] and vals[]
  getReadings();
  sendReadings(freqs, vals);

  delay(20);
}
