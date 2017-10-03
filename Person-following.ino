#include <Servo.h>

/*Servo*/
Servo myservo;
int angleLeft =40, angleCenter = 80, angleRight = 110; // the angle for the servo to turn

// These constants won't change.  They are the pin numbers for the three ultrasonic sensors
const int Pin1 = 9; //Left; pin D9
const int Pin2 = 10; //Center; pin D10
const int Pin3 = 11; //Right; pin D11


const long maximum = 100; // if the person is closer to the portrait than this distance, he will be followed
bool isFollowed = false; // to keep track of if a person is being followed

long counter[3] = {0, 0, 0};
bool positionChanged = false;

void setup() {
  myservo.attach(5); //attach servo to pin D5
  // initialize serial communication:
  Serial.begin(9600);
}

void loop()
{  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration1, cm1;
  long duration2, cm2;
  long duration3, cm3;
  long cms[3] = {cm1, cm2, cm3};
  long mincm = 100;
  int closestSensor = 0;


  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(Pin1, OUTPUT);
  digitalWrite(Pin1, LOW);
  delayMicroseconds(2);
  digitalWrite(Pin1, HIGH);
  delayMicroseconds(5);
  digitalWrite(Pin1, LOW);
  pinMode(Pin1, INPUT);
  duration1 = pulseIn(Pin1, HIGH);
  delay(10);

  // convert the time into a distance in centimeters
  cms[0] = microsecondsToCentimeters(duration1);
  
  ///////////////////////////////////////////
  
  pinMode(Pin2, OUTPUT);
  digitalWrite(Pin2, LOW);
  delayMicroseconds(2);
  digitalWrite(Pin2, HIGH);
  delayMicroseconds(5);
  digitalWrite(Pin2, LOW);
  pinMode(Pin2, INPUT);
  duration2 = pulseIn(Pin2, HIGH);
  delay(10);

  // convert the time into a distance
  cms[1] = microsecondsToCentimeters(duration2);
  
  /////////////////////////////////////////////
  
  pinMode(Pin3, OUTPUT);
  digitalWrite(Pin3, LOW);
  delayMicroseconds(2);
  digitalWrite(Pin3, HIGH);
  delayMicroseconds(5);
  digitalWrite(Pin3, LOW);
  pinMode(Pin3, INPUT);
  duration3 = pulseIn(Pin3, HIGH);
  delay(10);

  // convert the time into a distance
  cms[2] = microsecondsToCentimeters(duration3);
  
  //////////////////////////////////////////////
  // Find the closest distance
//  Serial.print(cms[0]);
//  Serial.print(' ');
//  Serial.print(cms[1]);
//  Serial.print(' ');
//  Serial.print(cms[2]);
//  Serial.println();
//  
  delay(20);
  for(int k=0; k < 3; k++)
  {
      if(cms[k] < mincm)
      {
          counter[k]+= 1; //increment the counter for this sensor
          mincm = cms[k];
          closestSensor = k;
      }
  }

//  Serial.print(counter[0]);
//  Serial.print(' ');
//  Serial.print(counter[1]);
//  Serial.print(' ');
//  Serial.print(counter[2]);
//  Serial.println();
  
//  if(mincm < maximum){
//    isFollowed = true;
//  }
  ////////////////////////// case1: The person is only in the range of one sensor ///////////////////////////
  
  if(closestSensor == 0 && cms[1] > maximum && cms[2] > maximum && counter[0] > 2) // only in the range of the leftmost sensor
  {  
    myservo.write(angleLeft);
    positionChanged = true;
  }   
  else if(closestSensor == 1 && cms[0] > maximum && cms[2] > maximum && counter[1] > 2)// only in the range of the middle sensor
  {
    myservo.write(angleCenter);
    positionChanged = true;
  }
     
  else if(closestSensor == 2 && cms[0] > maximum && cms[1] > maximum && counter[2] > 2)// only in the range of the rightmost sensor
  {
    myservo.write(angleRight);
    positionChanged = true;
  }

  if (positionChanged) {
    for (int i = 0; i < 3; i++) {
      counter[i] = 0;
    }
    positionChanged = false;
  }
  delay(15);   

  


//  
//  Serial.print(mincm);
//  Serial.print("cm by sensor ");
//  Serial.print(closestSensor);
//  Serial.println();
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
