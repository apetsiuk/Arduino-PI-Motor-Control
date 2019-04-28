/************************************
 *          Motor Speed Control     *       
 ************************************/

//PID constants
float Kp = 2;
float Ki = 25;

// initialize variables
float error    = 0;
float intError = 0;
float control  = 0;
float Vs       = 0;
int dutyCycle  = 0;

// set step size for integration
float Ts       = 0.01; // 0.010 s = 10 ms

float current_time, previous_time, elapsed_time;

// set desired speed
float tachometer = 0.0286; // sensor scaling factor
float ref_speed  = 20; // reference angular velocity
float Vr = ref_speed * tachometer; // reference voltage

 
void setup(){
  
  // Default output (PWM rate ~1kHz)
  pinMode(5,OUTPUT); 
  
  // Timer/Counter Control Register 2 A
  // set PWM pins 3 and 11 (timer2) for 3906 Hz
  TCCR2B=(TCCR2B&0xF8) | 2;
  
  pinMode(3,OUTPUT); // 3906 HZ PWM
  // the duty cycle with higher frequency will give us 
  // smooth velocity change and the motor will not provide 
  // high sound

  Serial.begin(9600);
}    
 
void loop(){

        /**************************************************/
        // uncomment for debugging
        // it will allow you to change the reference 
        // speed on-the-fly (after each arduino reset)
        /*
        if(ref_speed == 0){
          while(1){
            ref_speed = Serial.parseFloat();
            if(ref_speed != 0) break;
          }
        }
        */
        /**************************************************/

        current_time = millis(); // get current time
        
        Vr = ref_speed * tachometer;
        
        Vs = analogRead(A1);
        Vs = (float)(Vs/256);
        //currentTime = millis();
        //elapsedTime = (double)(currentTime - previousTime); 
        error = Vr - Vs;
        
        // increment integrational error
        intError += error * Ts;
        // bound intError to [-1...1]
        intError = bound(intError, -1, 1);

        // calculate control signal
        control = Kp*error+Ki*intError;
        // bound control signal
        control = bound(control, -10, 10);
        
        dutyCycle=round(256/20*control+256/2);

        analogWrite(5,dutyCycle); // control PWM ~1.0 kHz
        analogWrite(3,dutyCycle); // control PWM ~3.9 kHz

        delay(Ts*1000); // [0.01(s)*1000](ms) = 10(ms)
        elapsed_time = millis() - current_time;

        /**************************************************/
        // uncomment for the debugging
        // Serial.print is time consuming function
        // which will take approx 80 ms
        /*
        Serial.print("Vr = "); Serial.print(Vr);
        Serial.print("\tVs = "); Serial.print(Vs);
        Serial.print("\tcontrol = "); Serial.print(control);
        Serial.print("\telapsed_time = "); Serial.print(elapsed_time);
        Serial.print("\t\tserial port time = "); Serial.print(millis()-current_time);
        Serial.print("\n");
        */
        /**************************************************/
}
           
// external functions
// bound signal in [min...max] range
float bound(float x, float x_min, float x_max){
  if(x < x_min){x = x_min;}
  if(x > x_max){x = x_max;}
  return x;
}

        
 



