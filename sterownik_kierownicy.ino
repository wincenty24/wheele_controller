#include <Math.h>


int val = 0;
int prepared_val = 0;
bool is_working = false;

void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  Serial.begin(9600);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH); 
  Serial.setTimeout(10);
}
template<typename T>
void checker(T ref, T act){
  if(ref > act){

    digitalWrite(4, LOW);
    digitalWrite(5, HIGH); 
  }else if(ref < act ){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);   
  }
  else{
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH); 
  }
}

template<typename T>
T mapf(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void process_info(String info) {
  char data[200];
  info.toCharArray(data, info.length());
  char* splitted_data = strtok(data, " ");
  int index = 0;
  while (splitted_data != NULL) {
    if (index == 0) {
      Serial.println(splitted_data);
      val = atof(splitted_data);
      index++;
    }else if(index == 1) {
      is_working = atof(splitted_data);
    }
    
    splitted_data = strtok(NULL, " ");
    
  }
}

void loop() {
  int mapped_pot_val = map(analogRead(A1), 0, 1023, -50, 50);
  String info;
  if (Serial.available()) {
    info = Serial.readString();
    process_info(info);
  }else{
  }
  if(is_working){
  checker(val, mapped_pot_val);
  }
}
