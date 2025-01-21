/* ------------------------------------------------------------------------
PROJETO SEGUIDOR DE LINHA COM LÓGICA FUZZY

REALIZADO PARA AVALIAÇÃO NA MATÉRIA DE CONTROLE INTELIGENTE - PROF: IVAN
EQUIPE:
    JORGE PALMA
    VINICIUS BARRETO
    LEONARDO PESSOA
    BLENDA SANTANA
------------------------------------------------------------------------- */

#include <stdlib.h>
#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

// Definição dos pinos dos sensores
#define SENSOR_ESQUERDO A15
#define SENSOR_CENTRAL A14
#define SENSOR_DIREITO A12

#define VELOCIDADE_SUPER_LENTA_1 45
#define VELOCIDADE_SUPER_LENTA_2 50


// Definição dos pinos dos motores (Driver 1)
#define MOT_TRAS_ESQ_H 2
#define MOT_TRAS_ESQ_L 3
#define ENA_MOT_TRAS_ESQ 9
#define MOT_TRAS_DIR_H 4
#define MOT_TRAS_DIR_L 5
#define ENB_MOT_TRAS_DIR 10

// Definição dos pinos dos motores (Driver 2)
#define MOT_FRENTE_ESQ_H 6
#define MOT_FRENTE_ESQ_L 7
#define ENA_MOT_FRENTE_ESQ 11
#define MOT_FRENTE_DIR_H 8
#define MOT_FRENTE_DIR_L 12
#define ENB_MOT_FRENTE_DIR 13

int sensor_direito = 0;  
int sensor_central = 0; 
int sensor_esquerdo = 0;

// Criação do objeto fuzzy
Fuzzy *fuzzy = new Fuzzy();

void setup() {
  Serial.begin(9600);
  
  // Configuração dos pinos dos sensores como entrada
  pinMode(SENSOR_ESQUERDO, INPUT);
  pinMode(SENSOR_CENTRAL, INPUT);
  pinMode(SENSOR_DIREITO, INPUT);

  // Configuração dos pinos dos motores como saída
  pinMode(MOT_TRAS_ESQ_H, OUTPUT);
  pinMode(MOT_TRAS_ESQ_L, OUTPUT);
  pinMode(ENA_MOT_TRAS_ESQ, OUTPUT);
  pinMode(MOT_TRAS_DIR_H, OUTPUT);
  pinMode(MOT_TRAS_DIR_L, OUTPUT);
  pinMode(ENB_MOT_TRAS_DIR, OUTPUT);

  pinMode(MOT_FRENTE_ESQ_H, OUTPUT);
  pinMode(MOT_FRENTE_ESQ_L, OUTPUT);
  pinMode(ENA_MOT_FRENTE_ESQ, OUTPUT);
  pinMode(MOT_FRENTE_DIR_H, OUTPUT);
  pinMode(MOT_FRENTE_DIR_L, OUTPUT);
  pinMode(ENB_MOT_FRENTE_DIR, OUTPUT);

  digitalWrite(ENA_MOT_FRENTE_ESQ, HIGH);
  digitalWrite(MOT_FRENTE_ESQ_L, LOW);
  digitalWrite(ENA_MOT_TRAS_ESQ, HIGH);
  digitalWrite(MOT_TRAS_ESQ_L, LOW);
  digitalWrite(ENB_MOT_FRENTE_DIR, HIGH);
  digitalWrite(MOT_FRENTE_DIR_L, LOW);
  digitalWrite(ENB_MOT_TRAS_DIR, HIGH);
  digitalWrite(MOT_TRAS_DIR_L, LOW);

  /* ------------------------------------------------------
  Definindo variáveis fuzzy para os sensores da direita
  ---------------------------------------------------------*/
  FuzzyInput *SensorLinhaDireito = new FuzzyInput(1);
  
  FuzzySet *ForaLinhaDireito = new FuzzySet(0, 0, 210, 400);
  SensorLinhaDireito->addFuzzySet(ForaLinhaDireito);

  FuzzySet *DentroLinhaDireito = new FuzzySet(250, 630, 1020, 1020);
  SensorLinhaDireito->addFuzzySet(DentroLinhaDireito);
  
  fuzzy->addFuzzyInput(SensorLinhaDireito);
  
  /* ------------------------------------------------------
    Definindo variáveis fuzzy para os sensores do centro
  ---------------------------------------------------------*/
  FuzzyInput *SensorLinhaCentro = new FuzzyInput(2);
  
  FuzzySet *ForaLinhaCentro = new FuzzySet(0, 0, 210, 400);
  SensorLinhaCentro->addFuzzySet(ForaLinhaCentro);
  
  FuzzySet *DentroLinhaCentro = new FuzzySet(250, 630, 1020, 1020);
  SensorLinhaCentro->addFuzzySet(DentroLinhaCentro);
  
  fuzzy->addFuzzyInput(SensorLinhaCentro);
  
  /* ------------------------------------------------------
    Definindo variáveis fuzzy para os sensores da esquerda
  ---------------------------------------------------------*/
  FuzzyInput *SensorLinhaEsquerdo = new FuzzyInput(3);
  
  FuzzySet *ForaLinhaEsquerdo = new FuzzySet(0, 0, 210, 400);
  SensorLinhaEsquerdo->addFuzzySet(ForaLinhaEsquerdo);

  FuzzySet *DentroLinhaEsquerdo = new FuzzySet(250, 630, 1020, 1020);
  SensorLinhaEsquerdo->addFuzzySet(DentroLinhaEsquerdo);
  
  fuzzy->addFuzzyInput(SensorLinhaEsquerdo);

  /* ------------------------------------------------------
    Definição Variaveis Fuzzy Saida de Velocidade
  ---------------------------------------------------------*/ 
  FuzzyOutput *Velocidade = new FuzzyOutput(1);
  
  FuzzySet *MuitoLento = new FuzzySet(0, 0, 35, 40); // 0
  Velocidade->addFuzzySet(MuitoLento);
  
  FuzzySet *Lento = new FuzzySet(30, 54, 63, 70); // 104
  Velocidade->addFuzzySet(Lento);
  
  FuzzySet *Normal = new FuzzySet(54, 69, 88, 97); // 136
  Velocidade->addFuzzySet(Normal);
  
  FuzzySet *Rapido = new FuzzySet(80, 100, 150, 150); // 223
  Velocidade->addFuzzySet(Rapido);
  
  fuzzy->addFuzzyOutput(Velocidade);

  /* ------------------------------------------------------
    Variaveis de Saida do Motor Direito
  ---------------------------------------------------------*/
  FuzzyOutput *VelocidadeAjusteDireita = new FuzzyOutput(2);
  
  FuzzySet *MuitoLentoAjusteDireita = new FuzzySet(0, 0, 25, 40); //0
  VelocidadeAjusteDireita->addFuzzySet(MuitoLentoAjusteDireita);
  
  FuzzySet *LentoAjusteDireita = new FuzzySet(30, 54, 63, 70); //140
  VelocidadeAjusteDireita->addFuzzySet(LentoAjusteDireita);
  
  FuzzySet *NormalAjusteDireita = new FuzzySet(80, 95, 101, 110); //170
  VelocidadeAjusteDireita->addFuzzySet(NormalAjusteDireita);
  
  FuzzySet *RapidoAjusteDireita = new FuzzySet(100, 125, 170, 170); //240
  VelocidadeAjusteDireita->addFuzzySet(RapidoAjusteDireita);
  
  fuzzy->addFuzzyOutput(VelocidadeAjusteDireita);

  /* ------------------------------------------------------
    Variaveis de Saida do Motor Esquerdo
  ---------------------------------------------------------*/
  FuzzyOutput *VelocidadeAjusteEsquerda = new FuzzyOutput(3);
  
  FuzzySet *MuitoLentoAjusteEsquerda = new FuzzySet(0, 0, 25, 40);
  VelocidadeAjusteEsquerda->addFuzzySet(MuitoLentoAjusteEsquerda);
  
  FuzzySet *LentoAjusteEsquerda = new FuzzySet(30, 54, 63, 70); //140
  VelocidadeAjusteEsquerda->addFuzzySet(LentoAjusteEsquerda);
  
  FuzzySet *NormalAjusteEsquerda = new FuzzySet(80, 95, 101, 110); //170
  VelocidadeAjusteEsquerda->addFuzzySet(NormalAjusteEsquerda);
  
  FuzzySet *RapidoAjusteEsquerda = new FuzzySet(115, 125, 135, 145); //240
  VelocidadeAjusteEsquerda->addFuzzySet(RapidoAjusteEsquerda);
  
  fuzzy->addFuzzyOutput(VelocidadeAjusteEsquerda);

  /* ------------------------------------------------------
    Regra 1 - Centro detecta full, esquerda e direita não detectam nada
  ---------------------------------------------------------*/
  FuzzyRuleAntecedent *ifSensorCentralDentroAndSensorEsquerdoFora = new FuzzyRuleAntecedent();          
  ifSensorCentralDentroAndSensorEsquerdoFora->joinWithAND(DentroLinhaCentro, ForaLinhaEsquerdo);

  FuzzyRuleAntecedent *ifSensorDireitoForaAndSensorCentralDentroAndSensorEsquerdoFora = new FuzzyRuleAntecedent();          
  ifSensorDireitoForaAndSensorCentralDentroAndSensorEsquerdoFora->joinWithAND(ForaLinhaDireito, ifSensorCentralDentroAndSensorEsquerdoFora);

  FuzzyRuleConsequent *velocidade1 = new FuzzyRuleConsequent(); 
  velocidade1->addOutput(Lento);

  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifSensorDireitoForaAndSensorCentralDentroAndSensorEsquerdoFora, velocidade1); 
  fuzzy->addFuzzyRule(fuzzyRule1);


  /* ------------------------------------------------------
    Regra 2 - Centro e esquerda detectam, e direita não detecta nada, simulando uma curva suave
  ---------------------------------------------------------*/
  FuzzyRuleAntecedent *ifSensorCentralDentroAndSensorEsquerdoDentro = new FuzzyRuleAntecedent();          
  ifSensorCentralDentroAndSensorEsquerdoDentro->joinWithAND(DentroLinhaCentro, DentroLinhaEsquerdo);

  FuzzyRuleAntecedent *ifSensorDireitoForaAndSensorCentralDentroAndSensorEsquerdoDentro = new FuzzyRuleAntecedent();          
  ifSensorDireitoForaAndSensorCentralDentroAndSensorEsquerdoDentro->joinWithAND(ForaLinhaDireito, ifSensorCentralDentroAndSensorEsquerdoDentro);

  FuzzyRuleConsequent *velocidade2 = new FuzzyRuleConsequent(); 
  velocidade2->addOutput(Normal);
  velocidade2->addOutput(MuitoLentoAjusteEsquerda);         //LENTOAJUSTEESQUERDA (2) E LENTOAJUSTEDIREITA(4)                               

  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifSensorDireitoForaAndSensorCentralDentroAndSensorEsquerdoDentro, velocidade2); 
  fuzzy->addFuzzyRule(fuzzyRule2); 

  /* ------------------------------------------------------
    Regra 3 - Esquerda detecta, centro e direita não detectam nada, simulando uma curva brusca
  ---------------------------------------------------------*/
  FuzzyRuleAntecedent *ifSensorCentralForaAndSensorEsquerdoDentro = new FuzzyRuleAntecedent();          
  ifSensorCentralForaAndSensorEsquerdoDentro->joinWithAND(ForaLinhaCentro, DentroLinhaEsquerdo);

  FuzzyRuleAntecedent *ifSensorDireitoForaAndSensorCentralForaAndSensorEsquerdoDentro = new FuzzyRuleAntecedent();          
  ifSensorDireitoForaAndSensorCentralForaAndSensorEsquerdoDentro->joinWithAND(ForaLinhaDireito, ifSensorCentralForaAndSensorEsquerdoDentro);

  FuzzyRuleConsequent *velocidade3 = new FuzzyRuleConsequent(); 
  velocidade3->addOutput(Rapido);
  velocidade3->addOutput(MuitoLentoAjusteEsquerda);                                        

  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifSensorDireitoForaAndSensorCentralForaAndSensorEsquerdoDentro, velocidade3); 
  fuzzy->addFuzzyRule(fuzzyRule3); 

  /* ------------------------------------------------------
    Regra 4 - Centro e direita detectam, e esquerda não detecta nada, simulando uma curva suave
  ---------------------------------------------------------*/
  FuzzyRuleAntecedent *ifSensorCentralDentroAndSensorDireitoDentro = new FuzzyRuleAntecedent();          
  ifSensorCentralDentroAndSensorDireitoDentro->joinWithAND(DentroLinhaCentro, DentroLinhaDireito);

  FuzzyRuleAntecedent *ifSensorEsquerdoForaAndSensorCentralDentroAndSensorDireitoDentro = new FuzzyRuleAntecedent();          
  ifSensorEsquerdoForaAndSensorCentralDentroAndSensorDireitoDentro->joinWithAND(ForaLinhaEsquerdo, ifSensorCentralDentroAndSensorDireitoDentro);

  FuzzyRuleConsequent *velocidade4 = new FuzzyRuleConsequent(); 
  velocidade4->addOutput(Normal);
  velocidade4->addOutput(MuitoLentoAjusteDireita);                                        

  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifSensorEsquerdoForaAndSensorCentralDentroAndSensorDireitoDentro, velocidade4); 
  fuzzy->addFuzzyRule(fuzzyRule4); 

  /* ------------------------------------------------------
    Regra 5 - Direita detecta, centro e esquerda não detectam nada, simulando uma curva brusca
  ---------------------------------------------------------*/
  FuzzyRuleAntecedent *ifSensorCentralForaAndSensorDireitoDentro = new FuzzyRuleAntecedent();          
  ifSensorCentralForaAndSensorDireitoDentro->joinWithAND(ForaLinhaCentro, DentroLinhaDireito);

  FuzzyRuleAntecedent *ifSensorEsquerdoForaAndSensorCentralForaAndSensorDireitoDentro = new FuzzyRuleAntecedent();          
  ifSensorEsquerdoForaAndSensorCentralForaAndSensorDireitoDentro->joinWithAND(ForaLinhaEsquerdo, ifSensorCentralForaAndSensorDireitoDentro);

  FuzzyRuleConsequent *velocidade5 = new FuzzyRuleConsequent(); 
  velocidade5->addOutput(Rapido);
  velocidade5->addOutput(MuitoLentoAjusteDireita);                                        

  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifSensorEsquerdoForaAndSensorCentralForaAndSensorDireitoDentro, velocidade5); 
  fuzzy->addFuzzyRule(fuzzyRule5);

  /* ------------------------------------------------------
    Regra 6 - Centro, esquerda e direita detecta full
  ---------------------------------------------------------*/
  FuzzyRuleAntecedent *ifSensorDireitoDentroAndSensorCentralDentroAndSensorEsquerdoDentro = new FuzzyRuleAntecedent();          
  ifSensorDireitoDentroAndSensorCentralDentroAndSensorEsquerdoDentro->joinWithAND(DentroLinhaDireito, ifSensorCentralDentroAndSensorEsquerdoDentro);

  FuzzyRuleConsequent *velocidade6 = new FuzzyRuleConsequent(); 
  velocidade6->addOutput(Normal);                                        

  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifSensorDireitoDentroAndSensorCentralDentroAndSensorEsquerdoDentro, velocidade6); 
  fuzzy->addFuzzyRule(fuzzyRule6); 
}

void loop() {  
  // Realizando a leitura dos sensores
  sensor_direito = analogRead(SENSOR_DIREITO);  
  sensor_central = analogRead(SENSOR_CENTRAL); 
  sensor_esquerdo = analogRead(SENSOR_ESQUERDO);
  
  Serial.print(sensor_esquerdo);Serial.print(", ");
  Serial.print(sensor_central);Serial.print(", ");
  Serial.print(sensor_direito);Serial.print(", ");
  
  //Serial.println("");

  // Realizando o set das inputs fuzzy
  fuzzy->setInput(1, sensor_direito);
  fuzzy->setInput(2, sensor_central);
  fuzzy->setInput(3, sensor_esquerdo);

  // Habilitando proecessamento das regras fuzzy
  fuzzy->fuzzify();
   
  // Obter os valores de saída fuzzy
  int velocidade = fuzzy->defuzzify(1);
  
  int velocidade_ajuste_direita = fuzzy->defuzzify(2);
  int velocidade_ajuste_esquerda = fuzzy->defuzzify(3);
  
  Serial.print(velocidade);Serial.print(", ");
  Serial.print(velocidade_ajuste_direita);Serial.print(", ");
  Serial.print(velocidade_ajuste_esquerda);
  Serial.println("");

  // TESTE
  //velocidade = 100;
  if (fuzzy->isFiredRule(1))
  {
    // Configurando a velocidade do lado esquerdo do carrinho
    analogWrite(MOT_FRENTE_ESQ_H, velocidade);
    analogWrite(MOT_TRAS_ESQ_H, velocidade);
    analogWrite(MOT_FRENTE_DIR_H, velocidade);
    analogWrite(MOT_TRAS_DIR_H, velocidade);
  }
  else if (fuzzy->isFiredRule(2))
  {
    // Configurando a velocidade do lado esquerdo do carrinho
    analogWrite(MOT_FRENTE_ESQ_H, velocidade_ajuste_esquerda);
    analogWrite(MOT_TRAS_ESQ_H, velocidade_ajuste_esquerda);
    analogWrite(MOT_FRENTE_DIR_H, velocidade);
    analogWrite(MOT_TRAS_DIR_H, velocidade);    
  }
  if (fuzzy->isFiredRule(3))
  {
    // Configurando a velocidade do lado esquerdo do carrinho
    analogWrite(MOT_FRENTE_ESQ_H, velocidade_ajuste_esquerda);
    analogWrite(MOT_TRAS_ESQ_H, velocidade_ajuste_esquerda);
    analogWrite(MOT_FRENTE_DIR_H, velocidade);
    analogWrite(MOT_TRAS_DIR_H, velocidade);    
  }
  else if (fuzzy->isFiredRule(4))
  {
    // Configurando a velocidade do lado esquerdo do carrinho
    analogWrite(MOT_FRENTE_ESQ_H, velocidade);
    analogWrite(MOT_TRAS_ESQ_H, velocidade);
    analogWrite(MOT_FRENTE_DIR_H, velocidade_ajuste_direita);
    analogWrite(MOT_TRAS_DIR_H, velocidade_ajuste_direita);    
  }
  else if (fuzzy->isFiredRule(5))
  {
    // Configurando a velocidade do lado esquerdo do carrinho
    analogWrite(MOT_FRENTE_ESQ_H, velocidade);
    analogWrite(MOT_TRAS_ESQ_H, velocidade);
    analogWrite(MOT_FRENTE_DIR_H, velocidade_ajuste_direita);
    analogWrite(MOT_TRAS_DIR_H, velocidade_ajuste_direita);    
  }
  else if (fuzzy->isFiredRule(6))
  {
    // Configurando a velocidade do lado esquerdo do carrinho
    analogWrite(MOT_FRENTE_ESQ_H, velocidade);
    analogWrite(MOT_TRAS_ESQ_H, velocidade);
    analogWrite(MOT_FRENTE_DIR_H, velocidade);
    analogWrite(MOT_TRAS_DIR_H, velocidade);    
  }
}
