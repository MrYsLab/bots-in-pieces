#define MAX_BUFFER_SIZE 20

// Output of memory usage:
// Sketch uses 1682 bytes (5%) of program storage space. Maximum is 32256 bytes.
// Global variables use 200 bytes (9%) of dynamic memory, leaving 1848 bytes for local variables. Maximum is 2048 bytes.

// The data and types we wish to send across the link
int count = 0;
byte mtype = 1;
byte device = 44;
byte the_register = 232;
int big_data = 2048;

// prebuild the packet


size_t n ;
char *working_buffer;
char working_buffer[] ;

void setup() {
  Serial.begin(115200);
  working_buffer = {highByte(count), lowByte(count), mtype, device, the_register, highByte(big_data),
                           lowByte(big_data), 1, 2, 3, 4, 5, 6
                          };

  n = sizeof(working_buffer);
  the_buffer[0] = n;
  for (int i = 0; i < n; i++) {
    the_buffer[i+1] = working_buffer[i];
  }
  delay(5000);
}

void loop() {
  // clear the the_buffer
  for (int i = 0; i < 1000; i++)
  {
    Serial.write(the_buffer, n + 1);
    // bump up the counter and adjust the count field in the packet
    count = i;
    the_buffer[1] = highByte(count);
    the_buffer[2] = lowByte(count);
  }
}