
/**
   gestion del tiempo trascurrido
*/
unsigned long relojPeriodo = 0;
// tiempo de encendido y apagado
const int periodoOnOff = 500;
unsigned long intOnOffClock = 0;
// duracion del intermitente funcionando
const int intEnabledPeriod = 5000;
// control OnOff
unsigned long sysOnOffClock = 0;
const int sysOnOffControl = 3000;


// control de luz
int modoLuz = 0;
int tiposLuz[] = {1, 2, 3, 0};

boolean estadoIntermitenteDcho = false;
boolean estadoIntermitenteIzdo = false;
boolean estadoEncendidoSistema = false;
boolean prevOnOfStatus = false;
boolean estadoFreno = false;

boolean parpadeoOnOff = false;
boolean intermitenteOnOff = false;

// Salidas de luces
int pinLedDcho = 4;
int pinLedIzdo = 3;
int pinLedOnOf = 13;
int pinLedFren = 5;

// Pusadores de entrada
int pinSwtOnOf = 2;
int pinSwtDcho = 11;
int pinSwtIzdo = 12;
int pinSwtFrnD = 6;
int pinSwtFrnI = 7;

void setup() {
  relojPeriodo = millis();
  intOnOffClock = millis();
  sysOnOffClock = millis();
  pinMode(pinLedDcho, OUTPUT);
  pinMode(pinLedIzdo, OUTPUT);
  pinMode(pinLedOnOf, OUTPUT);
  pinMode(pinLedFren, OUTPUT);

  pinMode(pinSwtDcho, INPUT);
  pinMode(pinSwtIzdo, INPUT);
  pinMode(pinSwtOnOf, INPUT);
  pinMode(pinSwtFrnI, INPUT);
  pinMode(pinSwtFrnD, INPUT);
}

void loop() {
  controlPulsadores();

  controlEncendido();
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

boolean isLuzEncendida() {
  return estadoEncendidoSistema;
}

/**
   Controla
*/
void controlEncendido() {
  if (prevOnOfStatus && (millis() - sysOnOffClock > sysOnOffControl)) {
    estadoEncendidoSistema = !estadoEncendidoSistema;
    sysOnOffClock = millis();
  }
}

/**
   Controla esl estado de la luz intermintente, luce o no
*/
void controlParpadeoIntermitente() {
  // control de parpadeo de intermitente
  if ((millis() - relojPeriodo > periodoOnOff)) {
    parpadeoOnOff = !parpadeoOnOff;
    relojPeriodo = millis();
  }
}

/**
   gestiona el tiempo de funcinamiento del intermintente
*/
void controlTiempoEncendidoIntermitente() {
  // control de parpadeo de intermitente
  if (millis() - intOnOffClock > intEnabledPeriod) {
    intOnOffClock = millis();
    intermitenteOnOff = false;
  }
}

/**
   Lee los pulsadores y configura el estado de la maquina
*/
void controlPulsadores() {
  boolean _izdoStatus = digitalRead(pinSwtIzdo);
  boolean _dchoStatus = digitalRead(pinSwtDcho);
  boolean _onOfStatus = digitalRead(pinSwtOnOf);
  boolean _frnDStatus = digitalRead(pinSwtFrnD);
  boolean _frnIStatus = digitalRead(pinSwtFrnI);

  //Control de estado encendido
  if (_onOfStatus) {
    // Pulsado
    if (!prevOnOfStatus) {
      // Modo de luz
      modoLuz = tiposLuz[modoLuz];
      prevOnOfStatus = true;
      sysOnOffClock = millis();
    }
  } else {
    prevOnOfStatus = false;
  }

  estadoFreno = false;
  if(_frnDStatus || _frnIStatus){
    estadoFreno = true;
  }

  // Control estado intermitentes
  if ((_izdoStatus ^ _dchoStatus) || (_izdoStatus ^ _dchoStatus)) {
    estadoIntermitenteDcho = _dchoStatus;
    estadoIntermitenteIzdo = _izdoStatus;
    intOnOffClock = millis();
    intermitenteOnOff = true;
  }
}

/**
   Encender apargar las luces
*/
void controlLuces() {
  digitalWrite(pinLedDcho, isActiadoDerecho());
  digitalWrite(pinLedIzdo, isActiadoIzquierdo());
  digitalWrite(pinLedOnOf, isLuzEncendida());
  analogWrite(pinLedFren, valorLuzFreno());
}

int valorLuzFreno(){
  int valor = 75;
  if (estadoFreno){
    valor = 255;
  }
  return valor;
}

