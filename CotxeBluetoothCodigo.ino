// Universitat Politècnica de Catalunya (UPC)
// Enginyeria de Sistemes Audiovisuals (EET)
// Projectes d'Enginyeria
// Autors: Adrián Sosa, Xavier Serrat, Sergio Tomás, Antonio J. Urbano

char valor=' ';            // Variable de recepción del dispositivo movil a la placa bluetooth

const char VLR_MES= 'a'; // Valor para acelerar
const char VLR_MENYS= 'r'; // Valor para desacelerar
const char VLR_STOP = 's'; // Valor para detener el vehículo

const char VLR_DRETA ='R'; // Valor para girar a la izquierda
const char VLR_ESQUERRA = 'L'; // Valor para girar a la derecha

const char VLR_END = 'E'; // Valor para que el vehiculo vaya hacia delante
const char VLR_DER = 'D'; // Valor para que el vehículo vaya hacia detrás 
const char VLR_NEUTRE = 'O'; // Valor para que las ruedas vuelvan a su estado inicial

const int VEL_MAX = 255; // Velocidad maxima a la salida del puerto PWD de la placa de Arduino 5V
const int PAS_VEL = 15; //Pasos de velocidad hasta llegar a la velocidad máxima
const int pinMotor=6; // Pin del motor
int vel_act =0; // Velocidad actual del vehiculo

// variables
int led = 13;
int val = -1;

const int SERVO_MAX = 230; //Voltaje máximo del servo de direción

const int pinServo = 9; // Pin del servo

int dirmotorA1 = 7 ; // direccion motor de sentido borna 1 
int dirmotorA2= 8; // direccion motor sentido borna2 
 
int dirmotorB1 = 10; // direccion motor de direccion borna 1 
int dirmotorB2= 11; // direccion motor de dirección borna2 

boolean endevant = true; //Indica si vamos hacia delante o hacia atrás

int pinActual = -1;

void setup(){
  
  /* Inicialización de los pines del motor*/
       pinMode(led, OUTPUT);
       pinMode(pinMotor,OUTPUT);
       pinMode(pinServo,OUTPUT);
       pinMode(dirmotorA1,OUTPUT);
       pinMode(dirmotorA2,OUTPUT);
       pinMode(dirmotorB1,OUTPUT);
       pinMode(dirmotorB2,OUTPUT);
       
   /* Configuración de Bluetooth */          
        delay(2000);
	Serial.begin(115200);
	delay(2000);
	Serial.print("AT+JSEC=3,1,1,06,123456r\n"); // Register local sevice security PIN activated
	delay(2000);
	Serial.print("AT+JDIS=3\r\n"); // Discoverable Inquiry & Page Scan enabled
	delay(2000);
	Serial.print("AT+JRLS=1101,11,Serial Port,01,000000\r\n"); // Register local service
	delay(2000);
	Serial.print("AT+JAAC=1\r\n"); // Auto accept
	delay(2000);
	Serial.print("AT+JPCR=06,123456\r\n"); // PIN code reply
	delay(2000);
	pinMode(led, OUTPUT);
	digitalWrite(led,HIGH);
	Serial.flush();
	val = Serial.read(); // wait for connection
	while (val != 'R'){
	  val = Serial.read();
	}
	delay(1000);
	Serial.print("AT+JSCR\r\n"); // Stream Connection Request
      
        digitalWrite(led,LOW);
}

void loop(){
  
        valor = Serial.read(); //Llegim el valor del port serie i l'assignem a la variable valor
        
        if (valor == VLR_END){ //Condicional de Dirección hacia delante
          endevant = true; // Vamos hacia delante CIERTO
          vel_act=0; // Ponemos el coche a velocidad 0
          analogWrite(pinMotor,vel_act);
        }
          
        if (valor == VLR_DER){ //Condicional de Dirección hacia detrás
          endevant = false;// Vamos hacia delante FALSO (vamos hacia atrás)
          vel_act=0; // Ponemos el coche a velocidad 0
          analogWrite(pinMotor,vel_act);
        }

 
        if( valor == VLR_MES && endevant == true){ //Si aceleramos y vamos hacia delante ACELERAMOS
           accelera(pinMotor);
        }
          
        if(valor == VLR_MENYS && endevant == false){ //Si deceleramos y vamos hacia atrás DECELERAMOS
          desaccelera(pinMotor);
        }
        
         if( valor == VLR_MES && endevant == false){ //Si aceleramos y vamos hacia atrás y vamos hacia atrás Aceleramos en sentido opuesto
           accelera(pinMotor);
        }
          
        if(valor == VLR_MENYS && endevant == true){ //Si desaceleramos y vamos hacia delante y vamos hacia delante DESACELERAMOS
        }
          
        if(valor == VLR_STOP){ //Detenemos el vehículo
           vel_act=0;
           analogWrite(pinMotor,vel_act);
        }
                
        if( valor == VLR_DRETA){//Giramos a la derecha
             giraDreta(pinServo);
        }
        
        if( valor == VLR_ESQUERRA){ //Giramos a la izquierda
            giraEsquerra(pinServo);
        }  
        
        if( valor == VLR_NEUTRE){ // Ponemos las ruedas en posición recta.
            analogWrite( pinServo  , 0 ); 
        }  
}

void accelera(int pinAct){
  if(vel_act < VEL_MAX){
    vel_act = vel_act + PAS_VEL;
  }
  else if (vel_act >= VEL_MAX ) {
          vel_act = VEL_MAX;
  }
  
 /*PUENTE EN H*/
 
  if(endevant == true){//Sentido de giro del motor
        digitalWrite (dirmotorA1,HIGH );// gira motor A izquierda 
        digitalWrite (dirmotorA2,LOW);
        }
  if (endevant == false){ //Sentido de giro del motor
        digitalWrite (dirmotorA1,LOW);// gira motor A derecha 
        digitalWrite (dirmotorA2,HIGH);
        }

 analogWrite( pinAct  , vel_act );
}

void desaccelera(int pinAct){
  
  if (vel_act > 0) {
          vel_act = vel_act - PAS_VEL;   
  }
  else {
          vel_act = 0;            
  }
  
   /*PUENTE EN H*/
   
    if(endevant == true){//Sentido de giro del motor
        digitalWrite (dirmotorA1,HIGH );// gira motor A izquierda 
        digitalWrite (dirmotorA2,LOW);
        }
    if (endevant == false){//Sentido de giro del motor
        digitalWrite (dirmotorA1,LOW);// gira motor A izquierda 
        digitalWrite (dirmotorA2,HIGH);
        }
        
         analogWrite(pinAct, vel_act); 
      
}
  
void giraDreta(int pinGir){ //Función de Giro hacia la derecha
        digitalWrite (dirmotorB1,LOW);// gira motor B izquierda 
        digitalWrite (dirmotorB2,HIGH);
        analogWrite(pinGir, SERVO_MAX ); 
}

void giraEsquerra(int pinGir){
        digitalWrite (dirmotorB1,HIGH);// gira motor B derecha 
        digitalWrite (dirmotorB2,LOW);
        analogWrite (pinGir, SERVO_MAX); 
}


