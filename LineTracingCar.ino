#define ENA 9
#define IN2 7
#define IN1 8

#define ENB 10
#define IN3 11
#define IN4 12

#define IR0 A0
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4

int baseSpeed=130;
int IR_threshold=400;

float Kp=12;
float Ki=3.0;
float Kd=4.0;

float previousError=0;
float integral=0;
const int trigPin=4;
const int echoPin=5;


void setup() {
  // put your setup code here, to run once:
  pinMode(IR0, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(6,LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  randomSeed(analogRead(0));
  
  Serial.begin(9600);
  delay(10000);

}

void loop()
{ 
  if(distance()<20.0)
  {
    setMotorSpeed(0,0);
    delay(20);
  }
  else
  {
    char branch = decideBranch();
    Serial.print(branch);
    if (branch == 'L' || branch == 'R' || branch=='E' || branch=='W' || branch=='S') 
    {
      executeTurn(branch);
    }
    else if(branch=='D')
    {
      setMotorSpeed(-baseSpeed+50, -baseSpeed+50);
      delay(100);
      setMotorSpeed(0,0);
      delay(1000);
      char correct_direction=decideBranch();
      if(correct_direction=='D')
      {
        setMotorSpeed(-100, 100);
        delay(400);
        setMotorSpeed(0,0);
        delay(150);
      }
      else
      {
        executeTurn(correct_direction);
      }
    
    }
    else 
    {
      followLine();
    }
  }
}


int getLineError() {
  int sensorValues[5];
  int binaryValues[5];
  sensorValues[0] = analogRead(IR0);
  sensorValues[1] = analogRead(IR1);
  sensorValues[2] = analogRead(IR2);
  sensorValues[3] = analogRead(IR3);
  sensorValues[4] = analogRead(IR4);

  for (int i = 0; i < 5; i++) {
    binaryValues[i] = (sensorValues[i] > IR_threshold) ? 1 : 0;
  }

  int weights[5] = {-2, -1, 0, 1, 2};

  int sum = 0;
  int count = 0;

  for (int i = 0; i < 5; i++) {
    if (binaryValues[i] == 1) {
      sum += weights[i];
      count++;
    }
  }

  if (count == 0) return 0;
  return (float)sum/count;
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    leftSpeed = -leftSpeed;
  }

  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  if (rightSpeed >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    rightSpeed = -rightSpeed;
  }
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}

bool intersection() {
  int s0 = (analogRead(IR0) > IR_threshold) ? 1 : 0;
  int s1 = (analogRead(IR1) > IR_threshold) ? 1 : 0;
  int s2 = (analogRead(IR2) > IR_threshold) ? 1 : 0;
  int s3 = (analogRead(IR3) > IR_threshold) ? 1 : 0;
  int s4 = (analogRead(IR4) > IR_threshold) ? 1 : 0;

  int sum = s0 + s1 + s2 + s3 + s4;
  Serial.print(sum);

  if ((sum == 1) && (sum==2)) return false;
  return true;
}

float distance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
  return distance;
}

bool isDeadEnd()
{
  int s0 = (analogRead(IR0) > IR_threshold) ? 1 : 0;
  int s1 = (analogRead(IR1) > IR_threshold) ? 1 : 0;
  int s2 = (analogRead(IR2) > IR_threshold) ? 1 : 0;
  int s3 = (analogRead(IR3) > IR_threshold) ? 1 : 0;
  int s4 = (analogRead(IR4) > IR_threshold) ? 1 : 0;

  int sum=0;
  sum=s0+s1+s2+s3+s4;
  if(sum==0)
  {
    setMotorSpeed(-80, -80);
    delay(50);
    if(decideBranch()=='D')
    {
      return true;
    }

    else
    {
      return false;
    }
  }
}

char decideBranch() {
  int s0 = (analogRead(IR0) > IR_threshold) ? 1 : 0;
  int s1 = (analogRead(IR1) > IR_threshold) ? 1 : 0;
  int s2 = (analogRead(IR2) > IR_threshold) ? 1 : 0;
  int s3 = (analogRead(IR3) > IR_threshold) ? 1 : 0;
  int s4 = (analogRead(IR4) > IR_threshold) ? 1 : 0;

  int sum=0;
  sum=s0+s1+s2+s3+s4;
  Serial.print(sum);
  
  if (sum == 0) return 'D';
  if(s0==1 && s4==1)
  {
    int random_opt=random(0,2);

    if(random_opt==0)
    {
      return 'L';
    }
    else
    {
      return 'R';
    }
  }
  if(s2==1 && s1==1 && s3==1) return 'S';
  else if(s2==1) return 'C';
  else if(s0==1) return 'L';
  else if(s4==1) return 'R';
  else if(s1==1 && s2==1) return 'W';
  else if(s2==1 && s3==1) return 'E';
  else if(s1==1) return 'W';
  else if(s3==1) return 'E';
  
  
  

  //if((analogRead(IR0)<IR_threshold) && (analogRead(IR4)<IR_threshold)) return 'C';
  // if (leftBranch && !rightBranch) return 'L';
  // if (!leftBranch && rightBranch) return 'R';
  // if (leftBranch && rightBranch) return 'L';
  return 'C';
}

void executeTurn(char direction) {
  if (direction == 'L') {
    setMotorSpeed(-100, 100);
    delay(80);
    
  
  } else if (direction == 'R') {
    setMotorSpeed(100, -100);
    delay(80);
    
  }

  else if(direction=='W')
  {
    setMotorSpeed(-60, 60);
    delay(60);
    setMotorSpeed(75,75);
    delay(60);
  }

  else if(direction=='E')
  {
    setMotorSpeed(60, -60);
    delay(60);
    setMotorSpeed(75,75);
    delay(60);
  }

  else if(direction=='S')
  {
    setMotorSpeed(80, 80);
    delay(100);
  }
  setMotorSpeed(0,0);
  delay(10);
}

void followLine() {
  float error = getLineError();
  integral += error;
  float derivate = error - previousError;
  float correction = Kp * error + Ki * integral + Kd * derivate;
  previousError = error;

  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  setMotorSpeed(leftSpeed, rightSpeed);
  
}
