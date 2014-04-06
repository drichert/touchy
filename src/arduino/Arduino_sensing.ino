// Adapted from https://github.com/Illutron/AdvancedTouchSensing

#define SET(x,y) (x |=(1<<y))     // Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y))) // |
#define CHK(x,y) (x & (1<<y))     // |
#define TOG(x,y) (x^=(1<<y))      // +

#define N 160  // Number of frequencies

int   num_freqs = 160;
float results[num_freqs];
float freq[num_freqs];

void setup() {
  TCCR1A = 0b10000010; // Set up frequency generator
  TCCR1B = 0b00011001; // +
  ICR1   = 110;
  OCR1A  = 55;

  pinMode(9, OUTPUT);  // Signal generator pin
  pinMode(8, OUTPUT);  // Sync (test) pin

  Serial.begin(115200);

  // Init results
  for(int i = 0; i < num_freqs; i++) {
    results[i] = 0;
  }
}

void loop() {
  unsigned int d;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v = analogRead(0); // Read response signal

    CLR(TCCR1B,0);  // Stop generator

    TCNT1 = 0;     // Reload new frequency
    ICR1  = d;     // |
    OCR1A = d / 2; // +

    SET(TCCR1B,0); // Restart generator

    // Filter results
    results[d] = results[d] * 0.5 + (float)(v) * 0.5;

    freq[d] = d;
  }

  PlottArray(1,freq,results);

  TOG(PORTB,0); // Toggle pin 8 after each sweep (good for scope)
}
