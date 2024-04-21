int myInts[] = {1,2,3,4};

char l = 'l';
char r = 'r';

double k = 0.75;

void setup()
{
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);

Serial.begin(9600);
Serial.println(myInts[0]);

forward(250);
}
void loop(){
}

void ride(char motor, int spd){
  if (motor == 'l'){
    if (spd<0){
      digitalWrite(7,0);
      analogWrite(6, -spd);
    }
    else{
      digitalWrite(7,1);
      analogWrite(6, spd);
    }
  }
  else {
    if (spd<0){
      digitalWrite(4,0);
      analogWrite(5, -spd*k);
    }
    else{
      digitalWrite(4,1);
      analogWrite(5, spd*k);
    }    
  }
}

void forward(int speed){
  ride(l,speed);
  ride(r,speed);
}

void left(int speed){
  ride(l,-speed);
  ride(r,speed);
}

void right(int speed){
  left(-speed);
}

void backward(int speed){
  ride(l,-speed);
  ride(r,-speed);
}
