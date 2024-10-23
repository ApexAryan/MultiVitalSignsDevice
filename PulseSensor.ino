#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2

//  Variables
int pulsePin = 0;
int blinkPin = 13;
int fadePin = 5;
int fadeRate = 0;

volatile int BPM;
volatile int Signal;
volatile int IBI = 600;
volatile boolean Pulse = false;
volatile boolean QS = false;

static int outputType = SERIAL_PLOTTER;


void setup(){
  pinMode(blinkPin,OUTPUT);
  pinMode(fadePin,OUTPUT);
  Serial.begin(115200);
  interruptSetup(); 
}


void loop(){
    serialOutput() ;
  if (QS == true){            
        fadeRate = 255;                          
        serialOutputWhenBeatHappens();  
        QS = false;
  ledFadeToBeat();                      
  delay(20);                             
}


void ledFadeToBeat(){
    fadeRate -= 15;                        
    fadeRate = constrain(fadeRate,0,255);  
    analogWrite(fadePin,fadeRate);      
  }
