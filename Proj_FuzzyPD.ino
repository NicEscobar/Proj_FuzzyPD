#include <Fuzzy.h>
#include <math.h>

float PV=0; // inicializando o nível com zero
float Erro;
float DErro; 
float PVanterior;
float Saida=0;
int setpoint=10;
int s;

//Tabelas de pertinência de Erro, DeltaErro e Bomba 
Fuzzy *fuzzy = new Fuzzy();
       
       //Tirado da função de pertinência do erro-----------------------------
       FuzzySet *MN = new FuzzySet(-20, -20, -4, -2);
       FuzzySet *PN = new FuzzySet(-4, -2, -2, 0);
       FuzzySet *ZE = new FuzzySet(-2, 0, 0, 2);
       FuzzySet *PP = new FuzzySet(0,2,2,4);
       FuzzySet *MP = new FuzzySet(2, 4, 20, 20);
       
       //Tirado da função de pertinência do delta erro-------------------------
       FuzzySet *MNd = new FuzzySet(-40, -40, -4, -2);
       FuzzySet *PNd = new FuzzySet(-4, -2, -2, 0);
       FuzzySet *ZEd = new FuzzySet(-2, 0, 0, 2);
       FuzzySet *PPd = new FuzzySet(0,2,2,4);
       FuzzySet *MPd = new FuzzySet(2, 4, 40, 40);
       
       //Tirado da função de pertinência da bomba------------------------
       FuzzySet *MB = new FuzzySet(0,0,0,25);
       FuzzySet *B = new FuzzySet(0, 25, 25, 50);
       FuzzySet *M = new FuzzySet(25, 50, 50, 75);
       FuzzySet *A = new FuzzySet(50, 75, 75, 100);
       FuzzySet *MA = new FuzzySet(75, 100, 100, 100);


void setup()
{
  Serial.begin(9600);
  //  VARIAVEL erro
        FuzzyInput *Erro = new FuzzyInput(1);
        Erro->addFuzzySet(MN);
        Erro->addFuzzySet(PN);
        Erro->addFuzzySet(ZE);
        Erro->addFuzzySet(PP);
        Erro->addFuzzySet(MP);  
        fuzzy->addFuzzyInput(Erro);

  //  VARIAVEL delta erro
        FuzzyInput *DErro = new FuzzyInput(2);
        DErro->addFuzzySet(MNd);
        DErro->addFuzzySet(PNd);
        DErro->addFuzzySet(ZEd);
        DErro->addFuzzySet(PPd);
        DErro->addFuzzySet(MPd);  
        fuzzy->addFuzzyInput(DErro);
  
  //  VARIAVEL saida
        FuzzyOutput *Saida = new FuzzyOutput(1);
        Saida->addFuzzySet(MB);
        Saida->addFuzzySet(B);
        Saida->addFuzzySet(M);
        Saida->addFuzzySet(A);
        Saida->addFuzzySet(MA);  
        fuzzy->addFuzzyOutput(Saida);

  FuzzyRuleConsequent* thenSaidaMB = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* thenSaidaB = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* thenSaidaM = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* thenSaidaA = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* thenSaidaMA = new FuzzyRuleConsequent();


  // Building FuzzyRule "IF Erro=MN and DEerro = MN THEN saida = MB"-------------------------DeltaE = MN
  FuzzyRuleAntecedent* ifErroMNAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroMNd->joinWithAND(MN,MNd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifErroMNAndDErroMNd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Building FuzzyRule "IF Erro=PN and DEerro = MN THEN saida = MB "
  FuzzyRuleAntecedent* ifErroPNAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroMNd->joinWithAND(PN,MNd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifErroPNAndDErroMNd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule2);

   // Building FuzzyRule "IF Erro=ZE and DEerro = MN THEN saida = B"
  FuzzyRuleAntecedent* ifErroZEAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroMNd->joinWithAND(ZE,MNd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifErroZEAndDErroMNd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule3);
  
   // Building FuzzyRule "IF Erro=PP and DEerro = MN THEN saida = M"
  FuzzyRuleAntecedent* ifErroPPAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroMNd->joinWithAND(PP,MNd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifErroPPAndDErroMNd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Building FuzzyRule "IF Erro=MP and DEerro = MN THEN saida = MB"
  FuzzyRuleAntecedent* ifErroMPAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroMNd->joinWithAND(MP,MNd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifErroMPAndDErroMNd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule5);
 
    // Building FuzzyRule "IF Erro=MN and DEerro = PN THEN saida = MB"-------------------------DeltaE = PN
  FuzzyRuleAntecedent* ifErroMNAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroPNd->joinWithAND(MN,PNd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifErroMNAndDErroPNd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Building FuzzyRule "IF Erro=PN and DEerro = PN THEN saida = B"
  FuzzyRuleAntecedent* ifErroPNAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroPNd->joinWithAND(PN,PNd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifErroPNAndDErroPNd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule7);

   // Building FuzzyRule "IF Erro=ZE and DEerro = PN THEN saida = B"
  FuzzyRuleAntecedent* ifErroZEAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroPNd->joinWithAND(ZE,PNd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifErroZEAndDErroPNd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule8);
 
   // Building FuzzyRule "IF Erro=PP and DEerro = PN THEN saida = M"
  FuzzyRuleAntecedent* ifErroPPAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroPNd->joinWithAND(PP,PNd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, ifErroPPAndDErroPNd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule9);

   // Building FuzzyRule "IF Erro=MP and DEerro = PN THEN saida = A"
  FuzzyRuleAntecedent* ifErroMPAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroPNd->joinWithAND(MP,PNd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule10 = new FuzzyRule(10, ifErroMPAndDErroPNd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule10);
 
     // Building FuzzyRule "IF Erro=MN and DEerro = ZE THEN saida = B"-------------------------DeltaE = ZE
  FuzzyRuleAntecedent* ifErroMNAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroZEd->joinWithAND(MN,ZEd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule11 = new FuzzyRule(11, ifErroMNAndDErroZEd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Building FuzzyRule "IF Erro=PN and DEerro = ZE THEN saida = B"
  FuzzyRuleAntecedent* ifErroPNAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroZEd->joinWithAND(PN,ZEd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule12 = new FuzzyRule(12, ifErroPNAndDErroZEd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule12);

   // Building FuzzyRule "IF Erro=ZE and DEerro = ZE THEN saida = M"
  FuzzyRuleAntecedent* ifErroZEAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroZEd->joinWithAND(ZE,ZEd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule13 = new FuzzyRule(13, ifErroZEAndDErroZEd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule13);

   // Building FuzzyRule "IF Erro=PP and DEerro = ZE THEN saida = A"
  FuzzyRuleAntecedent* ifErroPPAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroZEd->joinWithAND(PP,ZEd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule14 = new FuzzyRule(14, ifErroPPAndDErroZEd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule14);

   // Building FuzzyRule "IF Erro=MP and DEerro = ZE THEN saida = A"
  FuzzyRuleAntecedent* ifErroMPAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroZEd->joinWithAND(MP,ZEd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule15 = new FuzzyRule(15, ifErroMPAndDErroZEd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule15);

  // Building FuzzyRule "IF Erro=MN and DEerro = PP THEN saida = B"-------------------------DeltaE = PP
  FuzzyRuleAntecedent* ifErroMNAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroPPd->joinWithAND(MN,PPd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule16 = new FuzzyRule(16, ifErroMNAndDErroPPd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule16);

  // Building FuzzyRule "IF Erro=PN and DEerro = PP THEN saida = B"
  FuzzyRuleAntecedent* ifErroPNAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroPPd->joinWithAND(PN,PPd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule17 = new FuzzyRule(17, ifErroPNAndDErroPPd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule17);

   // Building FuzzyRule "IF Erro=ZE and DEerro = PP THEN saida = M"
  FuzzyRuleAntecedent* ifErroZEAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroPPd->joinWithAND(ZE,PPd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule18 = new FuzzyRule(18, ifErroZEAndDErroPPd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule18);
  
   // Building FuzzyRule "IF Erro=PP and DEerro = PP THEN saida = A"
  FuzzyRuleAntecedent* ifErroPPAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroPPd->joinWithAND(PP,PPd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule19 = new FuzzyRule(19, ifErroPPAndDErroPPd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule19);
  
   // Building FuzzyRule "IF Erro=MP and DEerro = PP THEN saida = MA"
  FuzzyRuleAntecedent* ifErroMPAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroPPd->joinWithAND(MP,PPd);
  thenSaidaMA->addOutput(MA);
  FuzzyRule *fuzzyRule20 = new FuzzyRule(20, ifErroMPAndDErroPPd, thenSaidaMA);
  fuzzy->addFuzzyRule(fuzzyRule20);
  
  // Building FuzzyRule "IF Erro=MN and DEerro = MP THEN saida = M"-------------------------DeltaE = PP
  FuzzyRuleAntecedent* ifErroMNAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroMPd->joinWithAND(MN,MPd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule21 = new FuzzyRule(21, ifErroMNAndDErroMPd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule21);

  // Building FuzzyRule "IF Erro=PN and DEerro = MP THEN saida = M"
  FuzzyRuleAntecedent* ifErroPNAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroMPd->joinWithAND(PN,MPd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule22 = new FuzzyRule(22, ifErroPNAndDErroMPd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule22);

   // Building FuzzyRule "IF Erro=ZE and DEerro = MP THEN saida = A"
  FuzzyRuleAntecedent* ifErroZEAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroMPd->joinWithAND(ZE,MPd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule23 = new FuzzyRule(23, ifErroZEAndDErroMPd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule23);
  
   // Building FuzzyRule "IF Erro=PP and DEerro = MP THEN saida = A"
  FuzzyRuleAntecedent* ifErroPPAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroMPd->joinWithAND(PP,MPd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule24 = new FuzzyRule(24, ifErroPPAndDErroMPd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule24);
  
   // Building FuzzyRule "IF  Erro=MP and DEerro = MP THEN saida = MA"
  FuzzyRuleAntecedent* ifErroMPAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroMPd->joinWithAND(MP,MPd);
  thenSaidaMA->addOutput(MA);
  FuzzyRule *fuzzyRule25 = new FuzzyRule(25, ifErroMPAndDErroMPd, thenSaidaMA);
  fuzzy->addFuzzyRule(fuzzyRule25);
}



void loop()
{
  if (Serial.available() > 0){
      s =  Serial.parseInt();
      if(s != setpoint){
        setpoint=s;
      }
    }
  if (setpoint == 0){
      Saida =0;
      PV = 0;
      setpoint =0;
      Erro =0;
      DErro=0;
      Serial.println (String(PV)+";"+String(Erro)+";"+String(Saida)+";"+String(DErro)+";"+String(setpoint)+";");
   }
   else{  
     Erro=PV-setpoint;
     fuzzy->setInput(1, Erro);
     fuzzy->setInput(2, DErro);
     fuzzy->fuzzify();
     Saida = fuzzy->defuzzify(1);
     Serial.println (String(PV)+";"+String(Erro)+";"+String(Saida)+";"+String(DErro)+";"+String(setpoint)+";");
     PVanterior=PV;
     //função de transferência
     PV=0.9954*PV+0.002763*Saida;
     DErro=PV-PVanterior;
   }
   delay (200);
}
