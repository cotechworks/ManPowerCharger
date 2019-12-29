#include <TimerOne.h>

#define LEVEL_1 3
#define LEVEL_2 4
#define LEVEL_3 5
#define LEVEL_4 6
#define LEVEL_5 7
#define LEVEL_6 8
#define LEVEL_7 9
#define LEVEL_8 10
#define LEVEL_9 11
#define LEVEL_10 12
#define ADC1_PIN 7
#define ADC2_PIN 0
#define SW_PIN 13

#define ADC1_LOWER_VOLT 8.7   /* 充電開始電圧 */
#define ADC2_LOWER_VOLT 7.4   /* 残量計　下限 */
#define ADC2_UPPER_VOLT 8.4   /* 残量計　上限 */
#define ADC_CV 1.135          /* ADCの補正係数 */

int i;
int ADC1,ADC2;
float ADC2_Spec[10] = {0};
float ADC1_Volt,ADC2_Volt;

void setup() {
  Serial.begin(9600);
  pinMode(LEVEL_1, OUTPUT);
  pinMode(LEVEL_2, OUTPUT);
  pinMode(LEVEL_3, OUTPUT);
  pinMode(LEVEL_4, OUTPUT);
  pinMode(LEVEL_5, OUTPUT);
  pinMode(LEVEL_6, OUTPUT);
  pinMode(LEVEL_7, OUTPUT);
  pinMode(LEVEL_8, OUTPUT);
  pinMode(LEVEL_9, OUTPUT);
  pinMode(LEVEL_10, OUTPUT);
  pinMode(SW_PIN, OUTPUT);

  for (i = 0; i < 10; i++) {
    ADC2_Spec[i] = ADC2_LOWER_VOLT + (ADC2_UPPER_VOLT - ADC2_LOWER_VOLT) * (i) / 10;
    Serial.print(ADC2_Spec[i]);
    Serial.print("\n");
  }

  Timer1.initialize(1000000); //マイクロ秒単位で設定
  Timer1.attachInterrupt(timerFire);
}

void loop() {
  /* 電圧確認 */
  ADC1 = analogRead(ADC1_PIN);
  ADC2 = analogRead(ADC2_PIN);

  ADC1_Volt = ((float)ADC1 * 5.0 * 3.2) / (1023.0) * ADC_CV;
  ADC2_Volt = ((float)ADC2 * 5.0 * 3.2) / (1023.0) * ADC_CV;

  /* 残量計 */
  digitalWrite(LEVEL_1, LOW);
  digitalWrite(LEVEL_2, LOW);
  digitalWrite(LEVEL_3, LOW);
  digitalWrite(LEVEL_4, LOW);
  digitalWrite(LEVEL_5, LOW);
  digitalWrite(LEVEL_6, LOW);
  digitalWrite(LEVEL_7, LOW);
  digitalWrite(LEVEL_8, LOW);
  digitalWrite(LEVEL_9, LOW);
  digitalWrite(LEVEL_10, LOW);
  
  digitalWrite(LEVEL_1, HIGH);
  if (ADC2_Volt > ADC2_Spec[1])digitalWrite(LEVEL_2, HIGH);
  if (ADC2_Volt > ADC2_Spec[2])digitalWrite(LEVEL_3, HIGH);
  if (ADC2_Volt > ADC2_Spec[3])digitalWrite(LEVEL_4, HIGH);
  if (ADC2_Volt > ADC2_Spec[4])digitalWrite(LEVEL_5, HIGH);
  if (ADC2_Volt > ADC2_Spec[5])digitalWrite(LEVEL_6, HIGH);
  if (ADC2_Volt > ADC2_Spec[6])digitalWrite(LEVEL_7, HIGH);
  if (ADC2_Volt > ADC2_Spec[7])digitalWrite(LEVEL_8, HIGH);
  if (ADC2_Volt > ADC2_Spec[8])digitalWrite(LEVEL_9, HIGH);
  if (ADC2_Volt > ADC2_Spec[9])digitalWrite(LEVEL_10, HIGH);

  /* 電源電圧の確認・ON/OFF制御 */
  if(ADC1_Volt >= ADC1_LOWER_VOLT){
    digitalWrite(SW_PIN, HIGH);
  }else{
    digitalWrite(SW_PIN, LOW);
  }
}

void timerFire() {  /* タイマー割込み */
  Serial.print(ADC1_Volt);
  Serial.print(",");
  Serial.print(ADC2_Volt);
  Serial.print("\n");
}
