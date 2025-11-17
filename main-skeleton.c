#define ETHERNET_WEBSERVER_LOGLEVEL       3
#include <WebServer_WT32_ETH01.h>
#include <FastLED.h>
#include "AccelStepper.h" 
#define ARTNET_PORT 6454
#define NUM_LEDS 120
#define LED_PIN 12
#define ARTNET_BUFFER_SIZE 530
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  // MAC address
IPAddress myIP(192, 168, 0, 110);
IPAddress myGW(192, 168, 0, 1);
IPAddress mySN(255, 255, 255, 0);
IPAddress myDNS(8, 8, 8, 8);
WiFiUDP Udp;
CRGB leds[NUM_LEDS];
#define ARTNET_BUFFER_SIZE 530
uint8_t artnetBuffer[ARTNET_BUFFER_SIZE];
AccelStepper stepperX(1, 4, 2);
#define home_switch 15 // Pin 9 connected to Home Switch (MicroSwitch)
// Stepper Travel Variables
int TravelX;  // Used to store the X value entered in the Serial Monitor
int buffere;
int buffere1;
int speed;
int der1;
int der2;
int der3;
int derTop=0;
         int sag1;
         int sag2;
         int sag3;
            int sol1;
             int sol2;
              int sol3;
            int aci;
           int reset;

           //int rasgele;
int move_finished=1;  // Used to check if move is completed
long initial_homing=-1;  // Used to Home Stepper at startup
int say=0;
int sag=0;
int sol=0;
//int aci=0;
int rasgele;
int yonrest=0;
int previousValue = 0;  // Önceki değer (ilk başta 0 olarak ayarlanır)
int threshold = 2;  // Analog okuma için değişim eşik değeri (isteğe bağlı)
int val=0;

int previous = 0;
int long newval = 0;   
int acihiz=0;
int acihizT=0;
void setup() {
  

  
  WT32_ETH01_onEvent();
 //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
 ETH.begin();
  ETH.config(myIP, myGW, mySN, myDNS);
  //WT32_ETH01_waitForConnect();
  Udp.begin(ARTNET_PORT);
  //Led Configuration
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  memset(leds, 0, NUM_LEDS * sizeof(CRGB));
  FastLED.show();
   pinMode(home_switch, INPUT_PULLUP);
  stepperX.setCurrentPosition(0);    // Set the current position as zero for now
  stepperX.setMaxSpeed(9000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(9000.0);  // Set Acceleration of Stepper
  stepperX.setSpeed(5000.0);
  initial_homing = 1;
  stepperX.setCurrentPosition(0);
  xTaskCreatePinnedToCore (
    loop2,     // Function to implement the task
    "loop2",   // Name of the task
    1000,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );
}

// the loop function runs over and over again forever
void loop() {
// Check for incoming Art-Net packets
  int packetSize = Udp.parsePacket();

 //delay(50); 


  if (packetSize > 0) {
      if (packetSize <= 530) { // Ensure the packet is not larger than the Art-Net maximum size
      Udp.read(artnetBuffer, packetSize);

      // Check if it's an Art-Net packet
      if (memcmp(artnetBuffer, "Art-Net", 8) == 0) {
        uint16_t opcode = artnetBuffer[9] << 8 | artnetBuffer[8];

        // Handle different Art-Net opcodes
        if (opcode == 0x5000) { // ArtDMX opcode
          uint16_t universe = artnetBuffer[15] << 8 | artnetBuffer[14];
          uint16_t length = artnetBuffer[17] << 8 | artnetBuffer[16];
          uint8_t sequence = artnetBuffer[18];
          uint8_t* data = &artnetBuffer[19];
          if (universe == 0) {
            // Update the LED colors
            //if (length >= NUM_LEDS * 3) {
              for (int i = 1; i < NUM_LEDS; i++) {
                leds[i].r = data[i * 3];
                leds[i].g = data[i * 3 + 1];
                leds[i].b = data[i * 3 + 2];
              }
              FastLED.show();
           // }
          }
         if (universe == 1) {
             sag1=(data[1]);//sag dön
            sol1=(data[2]);// sol dön
            rasgele=(data[3]);
            der1=(data[4]*6);
            der2=(data[5]*6);
            der3=(data[6]*6);
            acihiz=(data[7]);
           // derTop=der1+der2+der3;
            buffere=sag1;
            buffere1=sol1;
             

             //sag2=(data[4]);
             //sag3=(data[3]);
             //sol2=(data[5]);
             //sol3=(data[6]);
           // buffere=sag1+sag2+sag3;
           // buffere1=sol1+sol2+sol3;
            //buffere=sag1;
            //buffere1=sol1;
//der1=(data[8]*3);
//der2=(data[9]*3);
//der3=(data[10]*3);

int currentValue=der1+der2+der3;;
  if (abs(currentValue - previousValue) >= threshold) {
derTop=currentValue;
    previousValue = currentValue;  // Önceki değeri güncelle
  }
            }
        }
      }
    }
  }
//else eklenip paket kontrolü yapılacak !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
// the loop2 function also runs forver but as a parallel task
void loop2 (void* pvParameters) {
     pinMode(home_switch, INPUT_PULLUP); 
  // delay(5);  // Wait for EasyDriver wake up
resttt();
  while (1) {
sag=(buffere);
sol=(buffere1);
val=derTop;
  if (sag<2&&sol<2&&val<2&&rasgele<2)
  {
motor_ret();
  }
   if (sag>2&&sol<2&&val<2&&rasgele<2)
  {
yonrest=3;
   move_finished=0;
  say=say+5;
    stepperX.setMaxSpeed(sag*30);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(sag*25);  // Set Acceleration of Stepper
  stepperX.move(say);  // Set new moveto position of Stepper
//stepperX.setSpeed(sag*10); 
 stepperX.run();
  }
   if (sag<2&&sol>2&&val<2&&rasgele<2)
    {
   yonrest=4;
  say=say-5;
  stepperX.setMaxSpeed(sol*30);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(sol*25);  // Set Acceleration of Stepper
  stepperX.move(say);  // Set new moveto position of Stepper
  stepperX.run();
    }

if (sag<2&&sol<2&&rasgele<2)
    {
    delay(50);
  stepperX.setMaxSpeed(mapReverse(acihiz,0,255,9000,500));      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(mapReverse(acihiz,0,255,9000,500));  // Set Acceleration of Stepper
  //stepperX.setMaxSpeed(9000-(acihiz*9000/255));      // Set Max Speed of Stepper (Faster for regular movements)
  //stepperX.setAcceleration(9000-(acihiz*9000/255));  // Set Acceleration of Stepper
stepperX.runToNewPosition(derTop);
       stepperX.stop();
 // }

    }
//////////////////////////////////////////////
 if (sag<2&&sol<2&&val<2&&rasgele>2)
    {
resttt();
    }
////////////////////////////////////////
  }
}
             int mapReverse(int value, int minInput, int maxInput, int minOutput, int maxOutput)
 {
  return minOutput + (maxOutput - minOutput) * (value - minInput) / (maxInput - minInput);
}
void motor_ret()
{
   say=0;
   yonrest=0;
resetlemeSag();
stepperX.stop();
stepperX.setCurrentPosition(0);
}
void resttt()
{
 stepperX.setMaxSpeed(9000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(8000.0);  // Set Acceleration of Stepper
  stepperX.setCurrentPosition(0);  // Set the current position as zero for now
  stepperX.setMaxSpeed(9000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(8000.0);  // Set Acceleration of Stepper
  initial_homing=1;
 while (!digitalRead(home_switch)) { // Make the Stepper move CW until the switch is deactivated
    stepperX.moveTo(initial_homing);  
    stepperX.run();
    initial_homing++;
      delay(1);
  }
  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(9000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(8000.0);  // Set Acceleration of Stepper
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void resetlemeSag()
{

 stepperX.stop();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void resetlemeSol()
{
  stepperX.stop();

}



void karisik()
{
  stepperX.moveTo(3200);
  while (stepperX.currentPosition() != 300) // Full speed up to 300
    stepperX.run();
  stepperX.stop(); // Stop as fast as possible: sets new target
  stepperX.runToPosition(); 
  // Now stopped after quickstop

  // Now go backwards
  stepperX.moveTo(-3200);
  while (stepperX.currentPosition() != 0) // Full speed basck to 0
    stepperX.run();
  stepperX.stop(); // Stop as fast as possible: sets new target
  stepperX.runToPosition(); 
  // Now stopped after quickstop
}
