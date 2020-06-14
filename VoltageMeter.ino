#define BAUD_RATE 9600 // baud rate of serial monitor
#define TONE_PIN 3    // pin used for interrupt tone

int32_t timeMillis;    // variable to keep track of time (in milliseconds) of voltage measurements
int32_t samplePeriod;  // time (in milliseconds) between measurements

double timeSecs;

bool sending;

String comdata;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(BAUD_RATE);
  // set time counter to 0:
  timeMillis = 0;
  // set the sample period:
  samplePeriod = 100;
  // set sending bool:
  sending = false;
  // make TONE_PIN output:
  pinMode(TONE_PIN, OUTPUT);
  // generate tone for timing interrupt:
  tone(TONE_PIN, 1000);
  // attach the interrupt:
  attachInterrupt(digitalPinToInterrupt(TONE_PIN), interruptServiceRoutine, RISING);
  Serial.println("Time (seconds),Voltage (5/1024 volts)");
}

void loop() {
  //read string from serial monitor
  if (Serial.available() > 0) {
    comdata = "";
    while (Serial.available() > 0) {
      comdata += char(Serial.read());
      delay(2);
    }
    if (comdata == "s") {
      sending = !sending;
    }
  }
}

void interruptServiceRoutine() {
  if (sending) { // check if user has given request to start sending
    if (timeMillis % samplePeriod == 0) { // check if it's time to print
      // read the input on analog pin 0:
      int sensorValue = analogRead(A1);
      // calculate & print out the time:
      timeSecs = timeMillis / 1000.0;
      Serial.print(timeSecs, 1);
      // print "," to separate tim and value:
      Serial.print(",");
      // print the value read:
      Serial.println(sensorValue);
    }
  }
  if ((timeMillis != 0) || (sending)) {
    timeMillis += 1;
  }
}


