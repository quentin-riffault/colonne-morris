/*
*
*	Attention:  utiliser un éditeur type Sublime Text pour visionner ce code par parties
*	
*
*/

String buf = "";
short state = 0;

unsigned int bulleur_dir = 12,
             bulleur_brake = 9,
             bulleur_speed = 0,
             
             ventilateur_dir = 13,
             ventilateur_brake = 8,
             ventilateur_speed = 0;

void setup(){
	pinMode(bulleur_dir, OUTPUT);
	pinMode(bulleur_brake, OUTPUT);
	  
	digitalWrite(bulleur_dir, HIGH);
	digitalWrite(bulleur_brake, LOW);

	pinMode(ventilateur_dir, OUTPUT); //Initiates Motor Channel B pin
	pinMode(ventilateur_brake, OUTPUT);  //Initiates Brake Channel B pin

	digitalWrite(ventilateur_dir, HIGH);
	digitalWrite(ventilateur_brake, LOW);

	Serial.begin(9600);
	Serial1.begin(9600);
}

void parse(){
      if(state == 1){
          state++;
          Serial.print("Bulleur: ");
          bulleur_speed = buf.toInt();
          Serial.println(bulleur_speed);
        }else if(state == 2){
          Serial.print("Ventilateur: ");
          ventilateur_speed = buf.toInt();
          Serial.println(ventilateur_speed);
          }
      buf = "";
}

void update_motors(){
  if(!state){
      Serial.println("Waiting for connection");
      state = 503;
    }
    
  if(Serial1.available() > 0){
      buf += Serial1.read();
      //Serial.print("Received: ");
      //Serial.println(buf);
            
      if(buf == "36"){
          Serial.println("Incoming TX");
          state = 1;
       }else if(buf == "35"){
          Serial.println("End of TX");
          state = 0;
        }
     
      if(state >= 1 && buf != "36"){
          parse();
      }
      
      buf="";


    }
    
    analogWrite(11, ventilateur_speed);
    analogWrite(3, bulleur_speed);
    
}

void loop(){

	update_motors();

}
