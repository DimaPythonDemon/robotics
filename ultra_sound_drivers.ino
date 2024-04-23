#define R_PIN_TRIG 12
#define R_PIN_ECHO 11

#define F_PIN_TRIG 9
#define F_PIN_ECHO 10

unsigned long real_f_ultra_time;
unsigned long real_r_ultra_time;

int f_cm, r_cm;

char f = 'f';
char r = 'r';
char l = 'l';

double all_speed_k = 1;

int turn_spd = 70;

// блоки прохождения программы
boolean b1 = true;
boolean b2 = false;
boolean b3 = false;
boolean b4 = false;
boolean b5 = false;

//массив для кубиков
//справа
boolean right_cubs[7];
int right_cube_counter = 0;
//слева
boolean left_cubs[7];
int left_cube_counter = 0;

int cur_perec_counter = 0;

double driver_k = 0.8;
//расстояние для бочки
int right_circle_distance = 9;

void setup() {
  //инициализируем взаимодействие по последовательному порту
  Serial.begin (9600);

  Serial.println(right_cubs[0]);
  
  //датчики ультразвука
  pinMode(F_PIN_TRIG, OUTPUT);
  pinMode(F_PIN_ECHO, INPUT);

  pinMode(R_PIN_TRIG, OUTPUT);
  pinMode(R_PIN_ECHO, INPUT);

  //моторы
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  delay(1000);
//  forward(100);
}

void circle(int r_cm, int turn_spd){
  if (r_cm > right_circle_distance){
    right(turn_spd);
  }
  else{
    forward(80);
  }
}

int measureCm(char side, int prev_cm){
  int response_rate = 50;
  if (side=='f'){
    if (millis()-real_f_ultra_time>response_rate){
      long f_duration,f_cm;
      // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
      digitalWrite(F_PIN_TRIG, LOW);
      delayMicroseconds(5);
      digitalWrite(F_PIN_TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(F_PIN_TRIG, LOW);
      f_duration = pulseIn(F_PIN_ECHO, HIGH);
      f_cm = (f_duration / 2) / 29.1;
      real_f_ultra_time = millis();
      return f_cm;
    }
    else return prev_cm;
  }
  else{
    if (millis()-real_r_ultra_time>response_rate){
      long r_duration,r_cm;
      digitalWrite(R_PIN_TRIG, LOW);
      delayMicroseconds(5);
      digitalWrite(R_PIN_TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(R_PIN_TRIG, LOW);
      r_duration = pulseIn(R_PIN_ECHO, HIGH);  
      r_cm = (r_duration / 2) / 29.1;
      real_r_ultra_time = millis();
      return r_cm;
    }
    else return prev_cm;
  }
}

void ride(char motor, int spd){
  if (motor == 'l'){
    if (spd<0){
      digitalWrite(7,0);
      analogWrite(6, -spd*all_speed_k);
    }
    else{
      digitalWrite(7,1);
      analogWrite(6, spd*all_speed_k);
    }
  }
  else {
    if (spd<0){
      digitalWrite(4,0);
      analogWrite(5, -spd*all_speed_k );
    }
    else{
      digitalWrite(4,1);
      analogWrite(5, spd*all_speed_k );
    }    
  }
}

void forward(int speed){
  ride(l,speed);
  ride(r,speed*driver_k);
}

void left(int speed){
  ride(l,-speed);
  ride(r,speed);
}

void right(int speed){
  left(-speed);
}

void backward(int speed){
  forward(-speed);
}

void pidLineRegul(int spd = 110){
  double driver_k = 0.3;
  //перекрёсток
  if (digitalRead(A1) and digitalRead(A2)){
    forward(spd);
  }
  //левый на линии, правый - нет
  else if (digitalRead(A2)){
    ride(r, spd);
    ride(l, spd*driver_k);
  }
  else if (digitalRead(A1)){
    ride(r, spd*driver_k);
    ride(l, spd);
  }
  else{
    forward(spd);
  }
}

boolean perec(){
  if (not(digitalRead(A1) and digitalRead(A2))){
    return true;
  }
  else return false; 
}

void stp() {forward(0);}

//для того чтобы робот понимал, что он поворачивается
boolean start_turning = false;
unsigned long start_turning_real_time;

void loop() {
  if (b1){
    Serial.println("PDREG");
    f_cm = measureCm(f, f_cm);
    if (f_cm > 12){
      pidLineRegul(80);  
    }
    else{
      b1 = false;
      b2 = true;
      stp();
      delay(500);
    }
  }
  else if (b2){
    Serial.println("turning");
    left(90);
    delay(500);
    stp();
    delay(1000);
    b2 = false;
    b3 = true;
  }
  else if (b3){
    Serial.println("CIRCLE");
    r_cm = measureCm(r, r_cm);
    circle(r_cm, turn_spd);  
    if (perec()){
      forward(100);
      delay(200);
      stp();
      delay(500);
      cur_perec_counter+=1;
      b3 = false;
      b4 = true;
      start_turning_real_time = millis();
    }
  }
  else if (b4){
    f_cm = measureCm(f, f_cm);
    r_cm = measureCm(r, r_cm);
    if (millis() - start_turning_real_time > 800){
      //замедляемся чтоб не проебать поворот
      left(74);
      if (r_cm < 15){
            stp();
            delay(500);
            b4 = false;
            if (cur_perec_counter<7) b3 = true;
            else b5 = true;      
      } 
    }
    else{
      if (f_cm > 30){
        left(76);
      }
      else{
        stp();
        right_cubs[right_cube_counter] = true;
        right_cube_counter += 1;
        b4 = false;
        if (cur_perec_counter<7) b3 = true;
        else b5 = true;
        while (measureCm(f, f_cm)>right_circle_distance)right(85);
        right(100);
        delay(500);
        stp();
        delay(500);
      }
    }
  }
}
