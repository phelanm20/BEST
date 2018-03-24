/*
  Blink once each time the superloop runs.
  Take x y z coordinates from accelerometer and print to serial monitor (then copy/paste to excel to analyze).
*/
const int X_PIN = 14;
const int Y_PIN = 15;
const int Z_PIN = 16;
const int SECOND = 1000;
const double zeroG = 512;
const double scale = 102.3;
const int buzzer = 5;
const double UNCERTAINTY = 0.05;
const int N = 5; //number of columns in value array; ie.takes acceleration readings every 0.203 seconds and stores five readings if N=5.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
}

void loop() {
    double Xval;
    double Yval;
    double Zval;
    double newval[3];
    double oldval[3][N];
    double time;
    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);

    time = millis();

    //initialize old values array
    if(time<210){
      for(i=0;i<3;i++){
        for(j=0;j<N;j++){
          oldval[i][j]=0;
        }
      }
    }

    Xval = analogRead(X_PIN);
    delay(1);                 /*The person whose code we based this off of reccomended a small delay between pin readings.  He said he wasn't sure why.*/
    Yval = analogRead(Y_PIN);
    delay(1);
    Zval = analogRead(Z_PIN);
    delay(1);

    newval[0] = (Xval-zeroG)/scale;
    newval[1] = (Yval-zeroG)/scale;
    newval[2] = (Zval-zeroG)/scale;

    //move old values back one space inside array
    for(i=0;i<3;i++){
      for(j=0;j<N-1;j++){
        oldval[i][j]=oldval[i][j+1];       
      }
    }
    
    //place new values into array
    for(i=0;i<3;i++){
      oldval[i][N-1]= newval[i];
    }

    //checks to see if values have changed after a two second gap
    for(i=0;i<3;i++){
      if (abs(oldval[i][0]-newval[i])>UNCERTAINTY){
          //stop taking measurements
          while(1){
            //Alarm when patient is not breathing.
            tone(buzzer, 5000);
            delay(100);
            noTone(buzzer);
          }
       else{    
        Serial.print(Xval,4);
        Serial.print("\t");
        Serial.print(Yval,4);
        Serial.print("\t");
        Serial.print(Zval,4);
        Serial.print("\t");
    
        Serial.println(time,4);
       }
      }
    }

}
