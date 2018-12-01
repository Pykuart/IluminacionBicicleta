
/**
   gestion del tiempo trascurrido
*/
unsigned long relojPeriodo = 0;
// tiempo de encendido y apagado
const int periodoOnOff = 500;
unsigned long relojEncendido = 0;
// duracion del intermitente funcionando
const int duracionEncendido = 5000;

boolean estadoIntermitenteDcho = false;
boolean estadoIntermitenteIzdo = false;

boolean parpadeoOnOff = false;
boolean intermitenteOnOff = false;

// Salidas de luces
int pinLedDcho = 2;
int pinLedIzdo = 3;
int pinLedOnOf = 7;

// Pusadores de entrada
int pinSwtOnOf = 6;
int pinSwtDcho = 5;
int pinSwtIzdo = 4;

void setup() {
  relojPeriodo = millis();
  relojEncendido = millis();
  pinMode(pinLedDcho, OUTPUT);
  pinMode(pinLedIzdo, OUTPUT);
  pinMode(pinLedOnOf, OUTPUT);
  
  pinMode(pinSwtDcho, INPUT);
  pinMode(pinSwtIzdo, INPUT);
  pinMode(pinSwtOnOf, INPUT);
}

void loop() {
  controlPulsadores();

  //
  controlParpadeoIntermitente();
  controlTiempoEncendidoIntermitente();

  controlLuces();

}

boolean isActiadoDerecho() {
  return estadoIntermitenteDcho && parpadeoOnOff && intermitenteOnOff;
}

boolean isActiadoIzquierdo() {
  return estadoIntermitenteIzdo && parpadeoOnOff && intermitenteOnOff;
}

/**

*/
void controlParpadeoIntermitente() {
  // control de parpadeo de intermitente
  if ((millis() - relojPeriodo > periodoOnOff)) {
    parpadeoOnOff = !parpadeoOnOff;
    relojPeriodo = millis();
  }
}

void controlTiempoEncendidoIntermitente() {
  // control de parpadeo de intermitente
  if (millis() - relojEncendido > duracionEncendido) {
    relojEncendido = millis();
    intermitenteOnOff = false;
  }
}

void controlPulsadores() {
  boolean _izdoStatus = digitalRead(pinSwtIzdo);
  boolean _dchoStatus = digitalRead(pinSwtDcho);
  boolean _onOfStatus = digitalRead(pinSwtOnOf);

  //Control de estado encendido
  if(_onOfStatus){
    // Pulsado
    if(_prevOnOfStatus){
      // Modo de luz
      // control de tiempo pulsado      
    }else{
      _prevOnOfStatus = true;
    }
  }

  // Control estado intermitentes
  if ((_izdoStatus ^ _dchoStatus) || (_izdoStatus ^ _dchoStatus)) {
    estadoIntermitenteDcho = _dchoStatus;
    estadoIntermitenteIzdo = _izdoStatus;
    relojEncendido = millis();
    intermitenteOnOff = true;
  }
}

void controlLuces() {
  digitalWrite(pinLedDcho, isActiadoDerecho());
  digitalWrite(pinLedIzdo, isActiadoIzquierdo());

}

