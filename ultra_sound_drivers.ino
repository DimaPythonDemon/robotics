#define F_PIN_TRIG 12
#define F_PIN_ECHO 11

#define R_PIN_TRIG 9
#define R_PIN_ECHO 10

long duration,cm;

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
  // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
  digitalWrite(R_PIN_TRIG, LOW);
  digitalWrite(F_PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(F_PIN_TRIG, HIGH);
  digitalWrite(R_PIN_TRIG, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(F_PIN_TRIG, LOW);
  digitalWrite(R_PIN_TRIG, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  f_duration = pulseIn(F_PIN_ECHO, HIGH);
  r_duration = pulseIn(R_PIN_ECHO, HIGH);

  // Теперь осталось преобразовать время в расстояние
  f_cm = (f_duration / 2) / 29.1;
  r_cm = (r_duration / 2) / 29.1;

  Serial.print("F: ");
  Serial.print(f_cm);
  Serial.print(" R: ");
  Serial.println(r_cm);

  // Задержка между измерениями для корректной работы скеча
  delay(250);
}
