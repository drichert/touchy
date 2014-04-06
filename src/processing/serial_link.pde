import processing.serial.*;

int port = 2, port_selected = 2;

// Global vars

int x, y, cmd;
boolean error = true;

boolean update_graph = true;
int line_graph, error_count = 0, num_received = 0;

// Local vars

boolean[] data_received = { false, false, false };
float[] 
