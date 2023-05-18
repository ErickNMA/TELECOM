#include <string.h>

#define tf 3

#define ts 1e-2

const int n = round(tf/ts);

void sendData(int *data)
{
  char buffer[6];
  Serial.write("BEGIN\n");
  for(int i=0; i<n; i++)
  {
    while(1)
    {
      if(Serial.availableForWrite())
      {
        sprintf(buffer, "%d\n", data[i]);
        Serial.write(buffer);
        break; 
      }        
    }
  }
  Serial.write("END\n");
}

int step=1, k=0, piezo[n];

unsigned long tref=0;

void setup() 
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);

  digitalWrite(LED_BUILTIN, LOW);
}

void loop() 
{
  switch(step)
  {
    case 1: // Aguarda comando
      if(Serial.available())
      {
        char aux[6];
        Serial.readBytes(aux, 5);
        aux[5] = '\0';
        if(!strcmp(aux, "START"))
        {
          digitalWrite(LED_BUILTIN, HIGH);
          delay(1000);
          digitalWrite(LED_BUILTIN, LOW);
          step++;
          tref = millis();
        }  
      }    
    break;
    case 2: // Coleta dos dados
      if((millis()-tref) >= (ts*1e3))
      {
        piezo[k] = analogRead(A0);
        k++;
        if(k == n)
        {
          step++;
        }
        tref = millis();
      }
    break;
    case 3: // Envia os dados
      sendData(piezo);
      step++;
    break;
    case 4: // FIM
      digitalWrite(LED_BUILTIN, HIGH);
    break;
  }
}
