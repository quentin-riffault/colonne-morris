/*
*
*	Attention:  utiliser un éditeur type Sublime Text pour visionner ce code par parties
*	
*
*/

String buf = "";

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
      Serial.print("Received: ");
      Serial.println(buf);
      buf = "";
}

void update_motors(){
    if(Serial1.available() > 0){
		buf += Serial1.read();
	}else if(buf != ""){
		parse(); 
		analogWrite(3, bulleur_speed);
		analogWrite(11, ventilateur_speed);
	}
}

void loop(){

	update_motors();

}
