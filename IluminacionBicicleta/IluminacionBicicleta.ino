
//on off input pin
int pinOnOffSwitch = 2;
// freno Derecho
int pinFrenoSwitch = 3;
// intermitentes
int pinIntDerSwitch = 4;
int pinIntIzqSwitch = 5;


//On Off outputPin
int pinOnOffOut = 7;
int pinFrontalLigth = 9;
int pinBackLigth = 10;
int pinIntDerLigth = 8;
int pinIntIzqLigth = 16;

// Status variables
// On off
boolean onOffStatus;
boolean onOffSwitchHolded;
long holdStartTime;
int prevPinOnOffSwitchSatus;
//Intensidad
int ligthIntensity;
//intermitentes
long intStartTime;
int direcctionSelected;
int direcctionStatus;
int prevDirecctionStatus;

// 3 Seconds interval to change status from on to off and viceversa
const int onOffInterval = 3000;
const int intermitenteInterval = 10;
const int backLigthNormal = 64;
const int backLigthBrake = 255;

void setup() {
  // put your setup code here, to run once:
  //On off configuration
  pinMode(pinOnOffSwitch, INPUT);
  pinMode(pinOnOffOut, OUTPUT);
  onOffStatus = false;
  holdStartTime = 0;
  prevPinOnOffSwitchSatus = LOW;

  // Frontal Ligth
  pinMode(pinFrontalLigth, OUTPUT);
  ligthIntensity = 1;

  //Luces frenos
  pinMode(pinFrenoSwitch, INPUT);
  pinMode(pinBackLigth, OUTPUT);

  //Intermitentes
  pinMode(pinIntDerSwitch, INPUT);
  pinMode(pinIntIzqSwitch, INPUT);
  pinMode(pinIntDerLigth, OUTPUT);
  pinMode(pinIntIzqLigth, OUTPUT);
  direcctionStatus = LOW;
  intStartTime = intermitenteInterval;
  prevDirecctionStatus = LOW;

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int pinOnOffSwitchValue = digitalRead(pinOnOffSwitch);
  delay(1);

  int pinFrenoSwitchValue = digitalRead(pinFrenoSwitch);
  delay(1);

  int pinIntDerSwitchValue = digitalRead(pinIntDerSwitch);
  delay(1);

  int pinIntIzqSwitchValue = digitalRead(pinIntIzqSwitch);
  delay(1);

  comprobarEncendidoApagadoSistema(pinOnOffSwitchValue);

  if (onOffStatus) {
    // sistema encendido
    digitalWrite(pinOnOffOut, HIGH);

    cambiarIntensidadLuzFrontal(pinOnOffSwitchValue);

    comprobarFrenada(pinFrenoSwitchValue);

    comprobarIntermitentes(pinIntDerSwitchValue, pinIntIzqSwitchValue);
    Serial.println("sistema encendido");
  } else {
    digitalWrite(pinOnOffOut, LOW);
  }

}

void cambiarIntensidadLuzFrontal(int pinOnOffSwitchStatus) {
  if (pinOnOffSwitchStatus == HIGH) {
    switch (ligthIntensity) {
      case 1:
      case 2:
      case 3:
        ligthIntensity++;
        break;
      default :
        ligthIntensity = 1;
    }

    int value = map(ligthIntensity, 0, 3, 0, 255);
    analogWrite(pinFrontalLigth, value);

  }
}

/**
   Comprueba el boton de encendido. Si se pulsa y mantiene durante el intervalo estipulado (3 seg.) el sistema cambia de encendido a apagao y viceversa
*/
void comprobarEncendidoApagadoSistema(int pinOnOffSwitchSatus) {

  // Sistema apagado
  // comprobar si se ordena encender
  if (pinOnOffSwitchSatus == HIGH) {
    if (pinOnOffSwitchSatus != prevPinOnOffSwitchSatus) {
      //Hay cambio de estado en el boton de encendido
      // se toma tiempo inicial de espera
      holdStartTime = millis();
      onOffSwitchHolded = false;
    } else {
      // No hay cambio de estado en el boton.
      // Se comprueba si ha pasado el tiempo de espera
      if ((millis() - holdStartTime) > onOffInterval) {
        // si han trascurrido 3 segundos mantenidos se cambia el estado del sistema
        onOffStatus = !onOffStatus;
        // una ves encendido, para evitar encender y apagar el sistema de continuo, se reinicia el sistema para, por lo menos esperar el tiempo de intervalo.
        holdStartTime = millis();
      }
    }
  }
  prevPinOnOffSwitchSatus = pinOnOffSwitchSatus;
}

void comprobarFrenada(int pinBrakeSwitchSatus) {
  int backLigthValue = backLigthNormal;
  if (pinBrakeSwitchSatus == LOW) {
    backLigthValue = backLigthBrake;
  }
  analogWrite(pinBackLigth, backLigthValue);
}

void comprobarIntermitentes(int pinIntDerSwitchValue, int pinIntIzqSwitchValue) {
  if (pinIntDerSwitchValue || pinIntIzqSwitchValue) {
    intStartTime = 0;
  digitalWrite(pinIntDerLigth, LOW);
  digitalWrite(pinIntIzqLigth, LOW);
    if (pinIntDerSwitchValue == HIGH) {
      direcctionSelected = pinIntDerLigth;
    } else {
      direcctionSelected = pinIntIzqLigth;
    }
  }

//Comprueba si el segundo es par o impar
  if (((millis() / 500) & 1) == 0) {
    direcctionStatus = HIGH;
  } else {
    direcctionStatus = LOW;
  }

  if (direcctionStatus != prevDirecctionStatus) {
    intStartTime ++;
  }

  if (intStartTime > intermitenteInterval) {
    direcctionStatus = LOW;
  }

  digitalWrite(direcctionSelected, direcctionStatus);
  prevDirecctionStatus = direcctionStatus;
}

