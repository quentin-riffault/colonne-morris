/*
*
*	Attention:  utiliser un éditeur type Sublime Text pour visionner ce code par parties	
*
*/
unsigned int bulleur_dir = 12,
             bulleur_brake = 9,
             bulleur_speed = 255,
             
             ventilateur_dir = 13,
             ventilateur_brake = 8,
             ventilateur_speed = 255;
             
int incomingByte = 0;

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

bool p = false;


void loop(){
		if(Serial1.available() > 0)
		{
		  incomingByte = Serial1.read();
                  Serial.println();
                  if (!p){
                    Serial.print("Changing bubbler speed to ");
                    Serial.write(incomingByte);
                    bulleur_speed = incomingByte;
                  }else{
                    Serial.print("Changing fan speed to ");
                    Serial.write(incomingByte);
                     ventilateur_speed = incomingByte;
                    }
                  p = !p;
		}

		analogWrite(3, bulleur_speed);
		analogWrite(11, ventilateur_speed);
}
