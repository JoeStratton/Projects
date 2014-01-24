//Joe Stratton
//Arizona State Univeristy -- Polytechnic
//November 25, 2013
//PRogram for LCD Screen/Fridge Control
#include <genieArduino.h>      //used for LCD serial com
#include <SPI.h>               //breaks without this
#include <Ethernet.h>         //ethernet shield library
#include <HTTPClient.h>       //ethernet shield library
#include <Avviso.h>           //notify my android for api key and push

char* items[] = {0, 0, 0, 0, 0, "Steak", "Fish", "Chicken", "Beans", "Peanuts", "Apples", "Bananas", 
                "Oranges", "Lemon", "Mango", "Celery", "Lettuce", "Broccoli", "Squash", "Carrots", "Milk", 
                "Cheese", "Yogurt", "Ice Cream", "Chocolate Milk", "Protein Powder", "Candy", "Poptarts", 
                "Health Bar", "Soda"}; // Set these to actual button index
char cart[25] = {0}; // Creates global list to be used for groceries
int index = 0;
int page = 0;

byte mac[] = { xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx }; //mac address for ethernet connection


void setup() { 

  genieBegin (GENIE_SERIAL, 9600);    // Serial0 initialization of TFT screen

  delay (1500); //let the display start up

  genieAttachEventHandler(myGenieEventHandler); // Attaches myfunc when event occurs for TFT
  genieWriteObject (GENIE_OBJ_FORM, 0, 0) ;   // Select Form0 of the Display (screen 1)

  Ethernet.begin(mac); 
  Avviso.begin(NOTIFY_MY_ANDROID);                                       /*initializes android com */
  Avviso.setApiKey("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  Avviso.setApplicationName("Smart Fridge");  
}


void loop() { 
  genieDoEvents(); // TFT Changes
}

void myGenieEventHandler(void) {

  genieFrame Event; // Genie Instance
  genieDequeueEvent(&Event); // Arduino Button Event 
  
  int iterator = 1;// reset everytime this handler is used page (1-5)
  int outputPage = 0;
    if (Event.reportObject.cmd == GENIE_REPORT_EVENT) {  // TFT Action
      if (Event.reportObject.object == GENIE_OBJ_WINBUTTON) {   // Food Button
        strcat(cart, items[Event.reportObject.index]);
        index++;
        if ((items[Event.reportObject.index] > items[4]) && (items[Event.reportObject.index] < items[10])){
          page = 1;
          for (int i = 5; i < 10; i++) { 
            if (items[Event.reportObject.index] == items[i]) {
              outputPage = iterator;
            } else {
              iterator++;
            }
          }
        } else if ((items[Event.reportObject.index] > items[9]) && (items[Event.reportObject.index] < items[15])) {
          page = 2; 
          for (int i = 10; i < 15; i++) { 
            if (items[Event.reportObject.index] == items[i]) {
              outputPage = iterator;
            } else {
              iterator++;
            }
          }   
        } else if ((items[Event.reportObject.index] > items[14]) && (items[Event.reportObject.index] < items[20])) {
          page = 3;    
          for (int i = 15; i < 20; i++) { 
            if (items[Event.reportObject.index] == items[i]) {
              outputPage = iterator;
            } else {
              iterator++;
            }
          }
        } else if ((items[Event.reportObject.index] > items[19]) && (items[Event.reportObject.index] < items[25])) {
          page = 4;    
          for (int i = 20; i < 25; i++) { 
            if (items[Event.reportObject.index] == items[i]) {
              outputPage = iterator;
            } else {
              iterator++;
            }
          }
        } else if (items[Event.reportObject.index] > items[24]) {
          page = 0;    
          for (int i = 25; i < 30; i++) { 
            if (items[Event.reportObject.index] == items[i]) {
              outputPage = iterator;
            } else {
              iterator++;
            }
          }
        }
        genieWriteStr(page, "added!");
        genieWriteObject(GENIE_OBJ_STRINGS, page,  outputPage);
        delay(500);
      } 
        else if(Event.reportObject.object == GENIE_OBJ_FORM) {
        
        genieWriteObject(GENIE_OBJ_STRINGS, 0 , 0);
        genieWriteObject(GENIE_OBJ_STRINGS, 1 , 0);
        genieWriteObject(GENIE_OBJ_STRINGS, 2 , 0);
        genieWriteObject(GENIE_OBJ_STRINGS, 3 , 0);
        genieWriteObject(GENIE_OBJ_STRINGS, 4 , 0);
        genieWriteObject(GENIE_OBJ_STRINGS, 5 , index);  
      }
    }
  
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT) {  // TFT Action
    if (Event.reportObject.object == GENIE_OBJ_4DBUTTON) {  // Send Button
 
      Serial.println("Sending...");
      notifymyAndroid(cart); //send list array to myfunc
    }  
  }
}


void notifymyAndroid (char* cart){//takes in the given list
  int len = strlen(cart);  //reads the length of char array (meaning # button pushed)
  cart[len+1] = '\0';// null terminator after last char in array
  Avviso.push("List Received", cart, 0); //sends list to android with 0 priority
  Serial.print("Sent ");
  Serial.print(index);// prints for test
  Serial.print(" items to phone!");
  memset(cart, 0 , 25); // resets array to 0 so no garbage is printed!*/
  index = 0;
}
