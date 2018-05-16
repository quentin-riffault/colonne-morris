/*
*
*	Attention:  utiliser un éditeur type Sublime Text pour visionner ce code par parties
*	
*
*/

/*====================Variables globales====================*/
	int co2_amount = -1;
	int pm_amount = -1;
	float temperature = -1;
	bool comp_state[3] = {true, true, true};

/*========================Webserver========================*/
	#include <SPI.h>
	#include <Ethernet.h>

	byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x81, 0x39 };
	IPAddress ip(192,168,0,143);
	EthernetServer server(4200);

	EthernetClient client;

	void sendJSON()
		{
		    Serial.print(millis());    
		    Serial.println(": Envoi du JSON");
		    client.println("HTTP/1.1 200 OK"); // Code 200 = réussite
		    client.println("Content-Type: application/json"); //Type MIME (JSON)
		    client.println("Access-Control-Allow-Origin: *");
		    client.println(); //Fin du header
		    
		    client.println("{"); // JSON
		      client.print("\t\"state\":[");
		      for(int i = 0; i < 3; i++)
		      {
					client.print(comp_state[i]);
					(i != 2) ? client.print(",") : client.print("]");
		      }
		      client.println(",");

		      client.print("\t\"temp\":");
		      client.print("26");
		      client.println(",");

		      client.print("\t\"speed\":");
		      client.print("250");
		      client.println(",");

		      client.print("\t\"time\":");
		      client.print(millis());
		      client.println(",");

		      client.print("\t\"co2\":");
		      client.print(co2_amount);
		      client.println(",");

		      client.print("\t\"pm1\":");
		      client.print(pm_amount);

		    client.println("\n}");  
		}

	const unsigned int MAX_INPUT = 100;
	const int MAX_PARAM = 10;

	// Example GET line: GET /?foo=bar HTTP/1.1
	void processGet (const char * data)
		{
		    // find where the parameters start
		    const char * paramsPos = strchr (data, '?');
		    if (paramsPos == NULL)
		        return;  // no parameters
		    // find the trailing space
		    const char * spacePos = strchr (paramsPos, ' ');
		    if (spacePos == NULL)
		        return;  // no space found
		    // work out how long the parameters are
		    int paramLength = spacePos - paramsPos - 1;
		    // see if too long
		    if (paramLength >= MAX_PARAM)
		        return;  // too long for us
		    // copy parameters into a buffer
		    char param [MAX_PARAM];
		    memcpy (param, paramsPos + 1, paramLength);  // skip the "?"
		    param [paramLength] = 0;  // null terminator
		    // do things depending on argument (GET parameters)

		    if (strcmp (param, "s") == 0) {
		        Serial.println (F("Envoyer le json!"));
		        sendJSON();
		    }
		    else if (strcmp (param, "b") == 0){
		        Serial.println (F("Signal de Bulleur"));
		        comp_state[0] = !comp_state[0];
		        ack();
		    }
		    else if (strcmp (param, "v") == 0){
		        Serial.println (F("Signal de Ventilateur"));
		        comp_state[1] = !comp_state[1];
		        ack();
		    }
		    else if (strcmp (param, "l") == 0){
		        Serial.println (F("Signal de LED"));
		        comp_state[1] = !comp_state[1];
		        ack();
		    }
		    else if (param[0] = 'm') {
		        Serial.println (F("Signal de vitesse Moteur!"));
                         short n[3];
                         int c = strlen(param);
                         int cp = c;
                        for(c; c > 1; c--){
                          n[c-2] = (int)param[c]-48;
                          Serial.print(n[c-2]);
                        }
		    } // Fin moteur
		    else Serial.println(param);
		}  // end of processGet
	void processData (const char * data)
		{
		    Serial.println (data);
		    if (strlen (data) < 4)
		        return;

		    if (memcmp (data, "GET ", 4) == 0)
		        processGet (&data [4]);
		}  // end of processData
	bool processIncomingByte (const byte inByte)
		{
		    static char input_line [MAX_INPUT];
		    static unsigned int input_pos = 0;
		    switch (inByte) {
		    case '\n':   // end of text
		        input_line [input_pos] = 0;  // terminating null byte
		        if (input_pos == 0)
		            return true;   // got blank line
		        // terminator reached! process input_line here ...
		        processData (input_line);
		        // reset buffer for next time
		        input_pos = 0;
		        break;

		    case '\r':   // discard carriage return
		        break;

		    default:
		        // keep adding if not full ... allow for terminating null byte
		        if (input_pos < (MAX_INPUT - 1))
		            input_line [input_pos++] = inByte;
		        break;
		    }  // end of switch
		    return false;    // don't have a blank line yet
		} // end of processIncomingByte
	void serverProcess()
		{
			// listen for incoming clients
		    client = server.available();
		    if (client) {
		        Serial.println(F("Client connected"));
		        // an http request ends with a blank line
		        boolean done = false;
		        while (client.connected() && !done) {
		            while (client.available () > 0 && !done)
		                done = processIncomingByte (client.read ());
		        }  // end of while client connected
		        client.stop();
		    }  // end of got a new client
		}
/*===========================CO2===========================*/
	#define         MG_PIN                       (A0)
	#define         BOOL_PIN                     (2)
	#define         DC_GAIN                      (8.5)

	#define         READ_SAMPLE_INTERVAL         (50)
	#define         READ_SAMPLE_TIMES            (5)

	#define         ZERO_POINT_VOLTAGE           (0.330)
	#define         REACTION_VOLTGAE             (0.030)

	float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};

	float MGRead(int mg_pin)
		{
		    int i;
		    float v=0;

		    for (i=0;i<READ_SAMPLE_TIMES;i++) {
		        v += analogRead(mg_pin);
		        delay(READ_SAMPLE_INTERVAL);
		    }
		    v = (v/READ_SAMPLE_TIMES) *5/1024 ;
		    return v;  
		}
	int  MGGetPercentage(float volts, float *pcurve)
		{
		   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
		      return -1;
		   } else { 
		      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
		   }
		}

	void update_CO2()
		{
			int percentage;
		    float volts;

		    volts = MGRead(MG_PIN);
		    percentage = MGGetPercentage(volts,CO2Curve);
		    if (percentage == -1) {
		        co2_amount = 400;
		    } else {
		        co2_amount = percentage;
		    }
		    delay(500);
		}
/*============================µP===========================*/
	byte bff[2];
	int DSM501A_pin = 39;//DSM501A input D39
	unsigned long duration;
	unsigned long hdepart;
	unsigned long harrive;
	unsigned long tpsmesurems = 30000;
	unsigned long tpsimpulsion = 0;
	float ratio = 0;
	float concentration = 0;

	void update_pm()
		{
		  duration = pulseIn(DSM501A_pin, LOW);
		  tpsimpulsion += duration;
		  harrive = millis();
		  if ((harrive-hdepart) > tpsmesurems)
		  {
		    ratio = (tpsimpulsion-harrive+hdepart + tpsmesurems)/(tpsmesurems*100);  // Integer percentage 0=>100
		    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
		    tpsimpulsion = 0;
		    hdepart = millis();
		  }

		  pm_amount = concentration;
		}
/*=======================Température=======================*/
	#include <OneWire.h>
	const byte BROCHE_ONEWIRE = 7;

	enum DS18B20_RCODES 
			{
			  READ_OK,  // Lecture ok
			  NO_SENSOR_FOUND,  // Pas de capteur
			  INVALID_ADDRESS,  // Adresse reçue invalide
			  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
			};
	byte getTemperature(float *temperature, byte reset_search) 
		{
		  byte data[9], addr[8];

		  if (reset_search) {
		    ds.reset_search();
		  }

		  if (!ds.search(addr)) {
		    return NO_SENSOR_FOUND;
		  }
		  
		  if (OneWire::crc8(addr, 7) != addr[7]) {
		    return INVALID_ADDRESS;
		  }
		 
		  if (addr[0] != 0x28) {
		    return INVALID_SENSOR;
		  }
		 
		  ds.reset();
		  ds.select(addr);

		  ds.write(0x44, 1);
		  delay(800);
		  
		  ds.reset();
		  ds.select(addr);
		  ds.write(0xBE);

		  for (byte i = 0; i < 9; i++) {
		    data[i] = ds.read();
		  }
		   
		  *temperature = (int16_t) ((data[1] << 8) | data[0]) * 0.0625; 

		  return READ_OK;
		}
	void update_temperature()
		{    
		  if (getTemperature(&temperature, true) != READ_OK) {
		    return;
		}
}
void setup()
{

  Serial.begin(9600);
  while (!Serial) { }

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print(F("Server is at "));
  Serial.println(Ethernet.localIP());

  pinMode(BOOL_PIN, INPUT);
  digitalWrite(BOOL_PIN, HIGH);

  pinMode(DSM501A_pin,INPUT);
  hdepart = millis();  

}

void update(){
	//update_motors();
	update_CO2();
	update_pm();
	//update_temperature();
}

void loop(){
	update();
	serverProcess();
}
