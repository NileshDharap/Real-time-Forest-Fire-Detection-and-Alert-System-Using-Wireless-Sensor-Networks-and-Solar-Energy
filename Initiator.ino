
#include <ESP8266WiFi.h>
#include <espnow.h>


// Integer for identification (make unique for each transmitter)
int ident = 1;

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD8, 0xF1, 0x5B, 0x13, 0x15, 0x48};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  bool fire;
  int ID;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(A0,INPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  long int flame;

  if ((millis() - lastTime) > timerDelay) {
    
  // Read flame sensor 
    flame = analogRead(A0);
    delay(10);

    if(flame == 1024){
      Serial.println("No Fire ");
    }
    else{
      Serial.println("Fire!");
      myData.fire = true;
      myData.ID = ident;
      // Send message via ESP-NOW
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
    
    delay(1000);
    // Add to structured data object
  lastTime = millis();
 }
}
