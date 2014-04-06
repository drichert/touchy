// Adapted from https://github.com/Illutron/AdvancedTouchSensing

#define SET(x,y) (x |=(1<<y))     // Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y))) // |
#define CHK(x,y) (x & (1<<y))     // |
#define TOG(x,y) (x^=(1<<y))      // +

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

  send_all(1, freq, results);

  TOG(PORTB,0); // Toggle pin 8 after each sweep (good for scope)
}

/*
                                                 .
                                      .         ;
         .              .              ;%     ;;
           ,           ,                :;%  %;
            :         ;                   :;%;'     .,
   ,.        %;     %;            ;        %;'    ,;
     ;       ;%;  %%;        ,     %;    ;%;    ,%'
      %;       %;%;      ,  ;       %;  ;%;   ,%;'
       ;%;      %;        ;%;        % ;%;  ,%;'
        `%;.     ;%;     %;'         `;%%;.%;'
         `:;%.    ;%%. %@;        %; ;@%;%'
            `:%;.  :;bd%;          %;@%;'
              `@%:.  :;%.         ;@@%;'
                `@%.  `;@%.      ;@@%;
                  `@%%. `@%%    ;@@%;
                    ;@%. :@%%  %@@%;
                      %@bd%%%bd%%:;
                        #@%%%%%:;;
                        %@@%%%::;
                        %@@@%(o);  . '
                        %@@@o%;:(.,'
                    `.. %@@@o%::;
                       `)@@@o%::;
                        %@@(o)::;
                       .%@@@@%::;
                       ;%@@@@%::;.
                      ;%@@@@%%:;;;.
                  ...;%@@@@@%%:;;;;,..    Gilo97
*/

// Note: x/y arguments reversed from original code (was y then x)
void send(int cmd, unsigned int x, unsigned int y) {
  byte x_msb    = 0,
       x_lsb    = 0,
       y_msb    = 0,
       y_lsb    = 0,
       z        = 128, // zero-byte (10000000)
       checksum = 0;

  x_lsb = lowByte(x);
  x_msb = highByte(x);
  y_lsb = lowByte(y);
  y_msb = highByte(y);

  // Set bit 1 high
  if(x_lsb == 0) {
    x_lsb = 1;
    z    += 1;
  }

  // Set bit 2 high
  if(x_msb == 0) {
    x_msb = 1;
    z    += 2;
  }

  // Set bit 3 high
  if(y_lsb == 0) {
    y_lsb = 1;
    z    += 4;
  }

  // Set bit 4 high
  if(y_msb == 0) {
    y_msb = 1;
    z    += 8;
  }

  // Get remainder of sum of all bytes divided by 255
  checksum = (cmd + x_msb + x_lsb + y_msb + y_lsb + z) % 255;

  if(checksum !=0 ) {
   Serial.write(byte(0));   // Start byte
   Serial.write(byte(cmd)); // Command byte

   Serial.write(byte(x_msb));
   Serial.write(byte(x_lsb));
   Serial.write(byte(y_msb));
   Serial.write(byte(y_lsb));

   Serial.write(byte(z));        // Which values have a zero value
   Serial.write(byte(checksum));
  }
}

void send_all(unsigned int cmd, float arr1[], arr2[]) {
  // Tell computer data is about to be sent
  send(cmd + 1, 1, 1);
  delay(1);

  for(int ndx = 0; ndx < num_freqs; ndx++) {
    send(cmd, round(arr1[ndx]), round(arr2[ndx]));
  }

  send(cmd + 2, 1, 1);
}

