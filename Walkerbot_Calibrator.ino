#include <Servo.h>
#include <tuple>
#include <map>
// Name: Walkerbot_Control
// Author: David Ledbetter
// Date Created: 8/24/2021
// Last Edit: 8/25/2021
//
// I've created classes that allow simultaneous servo control
// and continuous motor movement to a state rather than
// snapping to the position at default speed.
// Uses raw c++ methods to keep memory usage low.

#include <string>
using namespace std;

byte MAX = 8;
typedef tuple<char*, byte, byte> move_tuple;

class Smart_Servo {
  private:
    byte current_value;
    byte destination;
    char* nom;
    Servo ser;
  public:
    Smart_Servo(Servo &s, char* nom);
    void move_(byte dest);
    void arraymove_(byte dest);
    byte value();
    char* getName();
    byte Destination(byte initial);
    ~Smart_Servo();
};

Smart_Servo::Smart_Servo(Servo &s, char* nom) {
  this->ser = s;
  this->nom = nom;
  current_value = (nom[2] == 'L') ? 180 : 0;
  destination = 0;
}

byte Smart_Servo::Destination(byte initial) {
  byte dest = initial;
  // Limits 'dest' to between 0 and 55 degrees
  dest = (dest < 0) ? dest = 0 : dest = dest;
  dest = (dest > 55) ? dest = 55 : dest = dest;

  // If servo is on the left side the rnage of motion is flipped
  if (nom[2] == 'L') {
    dest = 180 - dest;
  }
  return dest;
}

void Smart_Servo::move_(byte dest) {
  // this move function is tuned specifically for Walkerbot, but
  // the presets can be changed for any project.

  dest = Destination(dest);

  // executes for right side motors increasing and left side motors decreasing
  if (dest > current_value) {
    for (byte i = current_value; i < dest; i += 1) {
      ser.write(i);
      current_value = i;
    }
  }
  // executes for left side motors increasing and right side motors decreasing
  if (dest < current_value) {
    for (byte i = current_value; i > dest; i -= 1) {
      ser.write(i);
      current_value = i;
    }
  }

}

void Smart_Servo::arraymove_(byte dest) {
  // this move function is tuned specifically for Walkerbot, but
  // the presets can be changed for any project.

  dest = Destination(dest);

  // executes for right side motors increasing and left side motors decreasing
  if (dest > current_value) {
    for (byte i = current_value; i < dest; i += 1) {
      ser.write(i);
      current_value = i;
    }
  }
  // executes for left side motors increasing and right side motors decreasing
  if (dest < current_value) {
    for (byte i = current_value; i > dest; i -= 1) {
      ser.write(i);
      current_value = i;
    }
  }

}

byte Smart_Servo::value() {
  return current_value;
}

char* Smart_Servo::getName() {
  return nom;
}

Smart_Servo::~Smart_Servo() {
  delete[] nom;
}

class Servo_Array {
  private:
    std::map<char*, Smart_Servo*> servo_map = {};
  public:
    Servo_Array(Smart_Servo** servos);
    void add_Servo(Smart_Servo s);
    void move_(move_tuple* move_list, byte number_of_moves);
};

Servo_Array::Servo_Array(Smart_Servo** servos) {
  for (byte i = 0; i < MAX; i++){
    Serial.print(i);  
  }
  Serial.print("Running");
  for (byte i = 0; i < MAX; i++) {
    Serial.print(i);
    if (servos[i] != NULL) {
      Serial.print(servos[i]->getName());
      servo_map[servos[i]->getName()] = servos[i];
    }
  }
  Serial.print("Array Finished");
}

void Servo_Array::move_(move_tuple* move_list, byte number_of_moves) {
  byte largest_change = 0;
  Serial.print("Move Started");
  Serial.println(largest_change);
  for (byte i = 0; i < number_of_moves; i++) {
    byte temp1 = get<1>(move_list[i]);
    if (temp1 > largest_change) {
      largest_change = temp1;
    }
  }
  Serial.println(largest_change);

  for (byte i = 0; i < largest_change; i += 1) {
    for (unsigned int j = 0; j < number_of_moves; j++) {
      byte dest = get<1>(move_list[j]);
      if (i <= largest_change) {
        servo_map[get<0>(move_list[j])]->move_(i);
      }
    }
    delay(20);
  }
}
Servo servoFIL;
Servo servoFOL;
Servo servoFIR;
Servo servoFOR;
Servo servoRIL;
Servo servoROL;
Servo servoRIR;
Servo servoROR;
Smart_Servo *FOL;
Smart_Servo *FIL;
Smart_Servo *FOR;
Smart_Servo *FIR;
Smart_Servo *ROL;
Smart_Servo *RIL;
Smart_Servo *ROR;
Smart_Servo *RIR;

int i = 0;
// Sets Walkerbot to default position_
void Zero() {

  servoFOL.write(180);
  servoFIL.write(180);
  servoFOR.write(0);
  servoFIR.write(0);
  servoROL.write(170);
  servoRIL.write(180);
  servoROR.write(10);
  servoRIR.write(0);

}

void setup() {
  Serial.begin(9600);
  servoFOL.attach(0);
  servoFIL.attach(1);
  servoROL.attach(2);
  servoRIL.attach(3);
  servoFOR.attach(4);
  servoFIR.attach(5);
  servoROR.attach(6);
  servoRIR.attach(7);
  Zero();
  FOL = new Smart_Servo(servoFOL, "FOL");
  FIL = new Smart_Servo(servoFIL, "FIL");
  FOR = new Smart_Servo(servoFOR, "FOR");
  FIR = new Smart_Servo(servoFIR, "FIR");
  ROL = new Smart_Servo(servoROL, "ROL");
  RIL = new Smart_Servo(servoRIL, "RIL");
  ROR = new Smart_Servo(servoROR, "ROR");
  RIR = new Smart_Servo(servoRIR, "RIR");
  Smart_Servo** servos = new Smart_Servo*[MAX];
  servos[0] = FOL;
  servos[1] = FIL;
  servos[2] = ROL;
  servos[3] = RIL;
  servos[4] = FOR;
  servos[5] = FIR;
  servos[6] = ROR;
  servos[7] = RIR;
  Servo_Array* ServoArray = new Servo_Array(servos);
  Serial.println("Servo Array Made");
  move_tuple move1[] = {move_tuple("FOL", 30, 0), move_tuple("FIL", 30, 0),
                        move_tuple("FOR", 30, 0), move_tuple("FIR", 30, 0),
                        move_tuple("ROL", 50, 0), move_tuple("RIL", 50, 0),
                        move_tuple("ROR", 50, 0), move_tuple("RIR", 50, 0)};
  
  Serial.println("Tuples Array Made");
  ServoArray->move_(move1, 8);
  Zero();
  ServoArray->move_(move1, 8);
  Zero();
  ServoArray->move_(move1, 8);
  Zero();
  ServoArray->move_(move1, 8);
  Zero();
  ServoArray->move_(move1, 8);
  Zero();
}

void loop() {
  delay(1000);
  //  for (i = 0; i < 50; i+=3) {
  //    servoFIL.write(i);
  //    servoFOL.write(i);
  ////    flipped for right: side 180-i
  //    servoFIR.write(180);
  //    servoFOR.write(180-i);
  //    delay(10);
  //  }
  //  delay(2000);
  //  for (i = 50; i > 0; i-=3) {
  //    servoFIL.write(i);
  //    servoFOL.write(i);
  ////    flipped for right: side 180-i
  //    servoFIR.write(180);
  //    servoFOR.write(180-i);
  //    delay(10);
  //  }
  //  delay(2000);

}
