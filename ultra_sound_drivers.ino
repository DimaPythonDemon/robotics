#define F_PIN_TRIG 12
#define F_PIN_ECHO 11

#define R_PIN_TRIG 9
#define R_PIN_ECHO 10

unsigned long real_f_ultra_time;
unsigned long real_r_ultra_time;

long f_cm, r_cm;

char f = 'f';
char r = 'r';
char l = 'l';

void setup() {

  // Инициализируем взаимодействие по последовательному порту

  Serial.begin (9600);
  //Определяем вводы и выводы
  pinMode(F_PIN_TRIG, OUTPUT);
  pinMode(F_PIN_ECHO, INPUT);

  pinMode(R_PIN_TRIG, OUTPUT);
  pinMode(R_PIN_ECHO, INPUT);
}

void loop() {
  f_cm = measure_cm(f);
  if (not(f_cm == -20)){
    Serial.println(f_cm);
  }
}

long measure_cm(char side){
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
    else return -20;
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
    else return -20;
  }
}

