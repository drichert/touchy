import processing.serial.*;

//int port = 2, port_selected = 2;
//
//// Global vars
//
//int x, y, cmd;
//boolean error = true;
//
//boolean update_graph = true;
//int line_graph, error_count = 0, num_received = 0;
//
//// Local vars
//
//boolean[] data_received = { false, false, false };
//float[]

int rxCount          = 0,
    bufferSize       = 8,
    messagesReceived = 0;

boolean error;

int[] buffer = new int[bufferSize];

void serialEvent(Serial port) {
  int inByte, checksum = 0;

  boolean dataReceived;

  while(port.available() > 0) {
    inByte = port.read();

    if(inByte == 0) rxCount = 0;

    if(inByte > 255) {
      println(" inByte = " + inByte);
      exit();
    }

    buffer[rxCount++] = inByte;

    if(rxCount > bufferSize) {
      rxCount = 0;

      messagesReceived++;

      for(int n = 0; n < bufferSize -1; n++) {
        checksum += buffer[n];
      }
      checksum %= 255;

      if(checksum == buffer[bufferSize - 1]) {
        error        = false;
        dataReceived = true;
      }
      else {
        error        = true;
        dataReceived = false;
      }

      if(!error) {
        int x, y, zeroByte, xLSB, xMSB, yLSB, yMSB, cmd;

        cmd      = buffer[1];
        xMSB     = buffer[2];
        xLSB     = buffer[3];
        yMSB     = buffer[4];
        yLSB     = buffer[5];
        zeroByte = buffer[6];

        // Adjust for 0-values
        if((zeroByte & 1) == 1) xLSB = 0;
        if((zeroByte & 2) == 2) xMSB = 0;
        if((zeroByte & 4) == 4) yLSB = 0;
        if((zeroByte & 8) == 8) yMSB = 0;

        // Combine bytes to make 16-bit ints
        x = xMSB << 8 | xLSB;
        y = yMSB << 8 | yLSB:

        switch(cmd) {
          case 1:
            break;
          case 2:
            break;
          case 3:
            break;
          case 4:
            break;
          case 5:
            break;
          case 6:
            break;
          case 20:
            break;
          case 21:
            break;
        }
      }
    }
  }
}
