import processing.serial.*;

Serial port;

int numFreqs = 200;

int[] ndxs   = new int[numFreqs];
float[] vals = new float[numFreqs];

// ASCII Line Feed
int lf = 10;

void setup() {
  size(400, 400);
  background(255);
  stroke(30);

  port = new Serial(this, Serial.list()[0], 115200);
  port.clear();
}

void draw() {
  // Index for popluating ndxs (frequency indexes)
  // and vals (sensor data)
  int _ndx = 0;

  while(port.available() > 0) {
    String buffer = port.readStringUntil(lf);

    if(buffer != null) {
      //ndxs = new int[numFreqs];
      //vals = new float[numFreqs];

      String[] bufferVals = split(buffer, ",");

      for(int i = 0; i < bufferVals.length; i++) {
        if(match(bufferVals[i], "^[0-9.]+$") != null) {
          if(i % 2 == 0) {
            _ndx = i / 2;

            //println("!!!" + bufferVals[i] + "!!!");
            ndxs[_ndx] = int(bufferVals[i]);
          }
          else {
            vals[_ndx] = Float.parseFloat(bufferVals[i]);
          }
        }
      }
    }
  }

  background(255);
  for(int i = 0; i < numFreqs; i++) {
    int x = width / numFreqs * ndxs[i];
    float y = vals[i];

    //point(width / numFreqs * ndxs[i], vals[i]);
    line(x, 0, x, y);
  }
}
