const int Sensor1 = 2;
const int Sensor2 = 3;
volatile long count = 0;
long interval = 0;
volatile bool MIC_status1=0;
volatile bool MIC_status2=0;
volatile long T1 = 0;
volatile long T2 = 0;



void setup() {
  
  attachInterrupt(0, SoundDetection1, RISING);
  attachInterrupt(1, SoundDetection2, RISING);
  Serial.begin(9600);
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  TCCR1B |= (1 << CS11);
  //TCCR1B |= (1 << CS10);

  TIMSK1 |= (1 << TOIE1);
  interrupts();

}

void loop() 
{ if( (MIC_status1 && MIC_status2) == 1)
  {
      interval = T1 - T2 ;
      int timePeriod = interval/2;
      Serial.println(T1);
      Serial.println(T2);
      Serial.print(timePeriod);
      Serial.println("Micro-Seconds");
      delay(1000);
      // Angle calculation
      float A = timePeriod * 346;
      double B = A/200000;
      Serial.println(B);
      float theta_radian = asin(B); //d = 10cm
      int theta = (180/PI) * theta_radian;
      Serial.println("Angle in degree");
      Serial.println(theta);
      int angle = 90 - theta;
      Serial.print("Angle from the centre is ");
      Serial.println(angle);
    
      MIC_status1 = 0;
      MIC_status2 = 0;
      interval = 0;
      count = 0;
      T1 = 0;
      T2 = 0;
      
  }
  

}

void SoundDetection1()
{
  T1 = TCNT1 + count * 65536;
  MIC_status1 = 1;

}
void SoundDetection2()
{
  T2 = TCNT1 + count * 65536;
  MIC_status2 = 1;

}

ISR(TIMER1_OVF_vect)
{
  count++;
}
