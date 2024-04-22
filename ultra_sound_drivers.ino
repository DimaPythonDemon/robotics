#define F_PIN_TRIG 12
#define F_PIN_ECHO 11

#define R_PIN_TRIG 9
#define R_PIN_ECHO 10

unsigned long real_f_ultra_time;
unsigned long real_r_ultra_time;

int f_cm, r_cm;

char f = 'f';
char r = 'r';
char l = 'l';

double driver_k = 0.75;

void setup() {
  //инициализируем взаимодействие по последовательному порту
  Serial.begin (9600);
  
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
}

void loop() {
  f_cm = measureCm(f, f_cm);
  r_cm = measureCm(r, r_cm);
  Serial.print("F: ");
  Serial.print(f_cm);
  Serial.print(" R: ");
  Serial.println(r_cm);
}

int measureCm(char side, int prev_cm){
  if (side=='f'){
    if (millis()-real_f_ultra_time>250){
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
    if (millis()-real_r_ultra_time>250){
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

void pid_regul(){
  double k = 0.3;
  int spd = 110;
  //перекрёсток
  if (digitalRead(A1) and digitalRead(A2)){
    forward(spd);
  }
  //левый на линии, правый - нет
  else if (digitalRead(A2)){
    ride(r, spd);
    ride(l, spd*k);
  }
  else if (digitalRead(A1)){
    ride(r, spd*k);
    ride(l, spd);
  }
  else{
    forward(0);
  }
}
