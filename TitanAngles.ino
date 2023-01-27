/* Titan Encoderizado 
 *  
 *  Grupo de Automação e Robótica Aplicada ( GARRA )
 * 
 */

#include <Encoder.h>

#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKE 3
#define N_MOTORS 12
#define MOVE_SPD 255
#define BOLA 1
#define QUADRADO 2
#define TRIANGULO 3
#define XIS 4
                                                                            
// PATA Bola - 1, 5, 9 | PATA Quadrado - 2, 6, 10 | PATA Triangulo - 3, 7, 11 | PATA Xis - 4, 8, 12   
//     motores = |  1|  2|  3|  4|  5|  6|  7|  8|  9| 10| 11|  12|;              
int inApin[12] = { 38, 25, 39, 24, 42, 29, 43, 28, 33, 31, 32, 30 }; // INA: Clockwise input
int inBpin[12] = { 44, 27, 45, 26, 40, 23, 41, 22, 35, 37, 34, 36 }; // INB: Counter-clockwise input
int pwmpin[12] = {  7,  8, 13,  2,  6,  9, 12,  3, 10, 11,  4,  5 }; // PWM input


// Pinos para os encoders Bola
Encoder x1(A8, A15);
Encoder y1(A9, A14);
Encoder z1(A10, A13);

// Posicao anterior Bola

int pos_ant_x1 = 0;
int pos_ant_y1 = 0;
int pos_ant_z1 = 0;

char buff[3] = {0,0,0};


void setup()
{
  Serial.begin(9600);
  Serial.println("Titan VIII Encoder Test Code");

}

void loop()
{
  //print_pos_enc_bola();
  Serial.readBytes(buff, 3);
  MoveToPosition(int(buff[0]), int(buff[1]), int(buff[2]), x1.read(), y1.read(), z1.read());
  //teste_bola();
}

float StepToAngle(int steps){
  return steps * 1.8;
}

int AngleToStep(float angles){
  return angles / 1.8;
}


void teste_bola()
{
    
    if (z1.read() == 0)
    {
      motorGo(8, CCW,35);
    } else if (z1.read() <= -8)
    {
      motorGo(8, CW, 35);
    }

    
    if (x1.read() == 0)
    {
      motorGo(0, CCW, 40);
    } else if (x1.read() >= 6)
    {
      motorGo(0, CW, 50);
    }

    
    if (y1.read() == 0)
    {
      motorGo(4, CCW, 25);
    } else if (y1.read() <= -15)
    {
      motorGo(4, CW, 35);
    }
}


void print_pos_enc_bola()
{
  pos_ant_x1 = x1.read();
  pos_ant_y1 = y1.read();
  pos_ant_z1 = z1.read();


  Serial.print("X1: ");
  Serial.print(x1.read());
  Serial.print("  Y1: ");
  Serial.print(y1.read());
  Serial.print("  Z1: ");
  Serial.println(z1.read());

  /*
  if (pos_ant_x1 != x1.read() || pos_ant_y1 != y1.read() || pos_ant_z1 != z1.read())
  {
    Serial.print("X1: ");
    Serial.print(x1.read());
    Serial.print("  Y1: ");
    Serial.print(y1.read());
    Serial.print("  Z1: ");
    Serial.println(z1.read());
  }
  */
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  // função para movimentação do motor
  // entra com número do motor, sentido de rotação e PWM de acionamento do motor
  if (motor <= N_MOTORS)
  {
    if (direct <= 4)
    {
      // Set inA[motor]
      if (direct <= 1 )
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);
      // Set inB[motor]
      if ((direct == 0) || (direct == 2) )
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);
      analogWrite(pwmpin[motor], pwm);
    }
  }
}

void setupmotors()
{
  // SETUP DOS MOTORES
  for (int i = 0; i < N_MOTORS; i++)
  {
    // Seta os pinos dos motores como saida
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
    // Começa com os motores parados
    motorOff(N_MOTORS);
  }
}

void motorOff(int motor)
{
  // DESLIGA TODOS OS MOTORES
  for (int i = 0; i < N_MOTORS; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
  analogWrite(pwmpin[motor], BRAKEVCC);
}

void MoveToPosition(int x, int y, int z, int encoderX, int encoderY, int encoderZ){
  //Serial.print(encoderX);
  float degX = StepToAngle(encoderX);
  float degY = StepToAngle(encoderY);
  float degZ = StepToAngle(encoderZ);
   
  Serial.print(x);
  Serial.print(y);
  Serial.println(z);

  Serial.print("X1: ");
  Serial.print(round(degX));
  Serial.print("  Y1: ");
  Serial.print(round(degY));
  Serial.print("  Z1: ");
  Serial.println(round(degZ));
  
  if(round(degY) > y){
    motorGo(4, CCW, 20);
  }
  if(round(degY) < y){
    motorGo(4, CW, 20);
  }
  if(round(degY+0.5) == y){
    motorGo(4, CW, 0);
  }
  //--------------------------------------z
   if(round(degZ) > z){
    motorGo(8, CCW, 20);
  }
  if(round(degZ) < z){
    motorGo(8, CW, 20);
  }
  if(round(degZ+0.5) == z){
    motorGo(8, CW, 0);
  }
  //--------------------------------------x
   if(round(degX) > x){
    motorGo(0, CCW, 40);
  }
  if(round(degX) < x){
    motorGo(0, CW, 40);
  }
  if(round(degX+0.5) == x){
    motorGo(0, CCW, 0);
  }
  
}
