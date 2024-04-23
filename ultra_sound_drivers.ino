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

double driver_k = 0.96;

int turn_spd = 60;

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
left_cube_counter = 0;

cur_perec_counter = 0;


//расстояние для бочки
right_circle_distance = 12;

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
  if (r_cm > tight_circle_distance){
    right(turn_spd);
  }
  else{
    forward(70);
  }
}

int measureCm(char side, int prev_cm){
  int response_rate = 10;
  
  if (side=='f'){
    if (millis()-real_f_ultra_time>response_rate){
      long f_duration,f_cm;
      // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
      digitalWrite(F_PIN_TRIG, LOW);
      delayMicroseconds(5);
      digitalWrite(F_PIN_TRIG, HIGH);
  
      // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
      delayMicroseconds(10);
      digitalWrite(F_PIN_TRIG, LOW);
  
      //  Время задержки акустического сигнала на эхолокаторе.
      f_duration = pulseIn(F_PIN_ECHO, HIGH);
    
      // Теперь осталось преобразовать время в расстояние
      f_cm = (f_duration / 2) / 29.1;

      real_f_ultra_time = millis();

      return f_cm;
    }
    else return prev_cm;
  }
  else{
    if (millis()-real_r_ultra_time>response_rate){
      long r_duration,r_cm;
      // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
      digitalWrite(R_PIN_TRIG, LOW);
      delayMicroseconds(5);
      digitalWrite(R_PIN_TRIG, HIGH);  
  
      // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
      delayMicroseconds(10);
      digitalWrite(R_PIN_TRIG, LOW);
  
      //  Время задержки акустического сигнала на эхолокаторе.
      r_duration = pulseIn(R_PIN_ECHO, HIGH);
    
      // Теперь осталось преобразовать время в расстояние
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
      analogWrite(5, -spd*driver_k);
    }
    else{
      digitalWrite(4,1);
      analogWrite(5, spd*driver_k);
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

void loop() {
  if (b1){
    Serial.println("PDREG");
    f_cm = measureCm(f, f_cm);
    if (f_cm > 15){
      pidLineRegul(80);  
    }
    else{
      b1 = false;
      b2 = true;
      stp();
    }
  }
  else if (b2){
    Serial.println("turning");
    left(95);
    delay(300);
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
    }
  }
  else if (b4){
    f_cm = measureCm(f, f_cm);
    if (f_cm > 30){
      left(85);
    }
    else{
      stp();
      right_cubs[right_cube_counter] = true;
      right_cube_counter += 1;
      b4 = false;
      if (cur_perec_counter<7) b3 = true;
      else b5 = true;
      while (measureCm(f, f_cm)>right_circle_distance)right(85);
      stp();
      delay(500);
    }
  }
}
