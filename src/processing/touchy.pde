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

int rxCount; 

rxCount = 0;

void serialEvent(Serial port) {
  var inByte;

  while(port.available() > 0) {
    inByte = port.read();

    if(inByte == 0) rxCount = 0;

    if(inByte > 255) {
      println(" inByte = " + inByte);
      exit();
    }

    buffer[rxCount] = inByte;
  }
}
