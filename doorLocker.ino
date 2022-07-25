#include <MatrixMini.h>
#include <MiniANG.h>
#include <MiniBTN.h>
#include <MiniDC.h>
#include <MiniDIG.h>
#include <MiniLED.h>
#include <MiniRC.h>

bool isDoorStucked;
bool isDoorClosed;

void setup() {
  Mini.begin(7.4, 1, 115200);
  Serial.begin(115200);
  Mini.M1.set(30);

  delay(100);
  Serial.println("Door Locker!");
}

bool checkDoorClosed(int grayScaleValue) {
  if (grayScaleValue < 100) {
    return true;
  }
  else {
    return false;
  }
}

bool checkDoorStucked(bool getDigitalInput) {

  // 按下亮光是0，彈起是1
  if (Mini.D1.get() == 1) {
    return false;
  } else {
    return true;
  }
}


void loop() {
  isDoorStucked = checkDoorStucked(Mini.D1.get()); 
  isDoorClosed = checkDoorClosed(Mini.A3.getANG());
  if (Mini.BTN1.get() == 1){

    if (isDoorClosed) {
      // 門要是關的，才可以開門
      Mini.RGB1.setRGB(255, 0, 0);
      Serial.println("Door Opening...");
      // 直到沒卡住之前，都讓橫桿往上抬
      while (isDoorStucked) {
        Mini.M1.set(-50);
        Serial.println("Locker is rising.");
        isDoorStucked = checkDoorStucked(Mini.D1.get()); 
        delay(100); //稍微久一點
      }
      Serial.println("Door is not locked.");

      Mini.RGB1.setRGB(0, 0, 0);
      Mini.M1.set(0);
    }
    else {
      Mini.RGB1.setRGB(0, 0, 0);
      Serial.println("Door is Opened.");
    }
  }
  
  if (Mini.BTN2.get() == 1) {
    if (!isDoorClosed) {
      // 門要是開的，才可以關門
      Mini.RGB2.setRGB(255, 0, 0);
      Serial.println("Please Close the Door...");

      // 需要手動關門，門閂才會卡上
      
      while (!isDoorClosed) {
        Serial.println("Door is not closed.");
        isDoorClosed = checkDoorClosed(Mini.A3.getANG());
      }

      while(!isDoorStucked){
        isDoorStucked = checkDoorStucked(Mini.D1.get()); 
        Serial.println("Door is not stucked.");
        Mini.M1.set(50);
      }
      Serial.println("Door is already stucked.");
      Mini.RGB2.setRGB(0, 0, 0);
    }
    else {
      // 關門但沒有鎖上 
      if (!isDoorStucked) {
        while(!isDoorStucked){
          isDoorStucked = checkDoorStucked(Mini.D1.get()); 
          Serial.println("Door is not stucked.");
          Mini.M1.set(50);
        }
        Serial.println("Door is already stucked.");
      }

      else {
        Mini.RGB2.setRGB(0, 0, 0);
        Serial.println("Door is Closed.");
      }
    }

    
  }
}
