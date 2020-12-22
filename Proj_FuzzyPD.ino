#include <Fuzzy.h>
#include <math.h>

float PV=0; // inicializando o nível com zero
float Erro;
float DErro; 
float PVanterior=0;
float Saida=0;
float setpoint=0;
int s;

//Tabelas de pertinência de Erro, DeltaErro e Bomba 
Fuzzy *fuzzy = new Fuzzy();
       
       //Tirado da função de pertinência do erro-----------------------------
       FuzzySet *MN = new FuzzySet(-200, -200, -2, -1);
       FuzzySet *PN = new FuzzySet(-2, -1, -1, 0);
       FuzzySet *ZE = new FuzzySet(-1, 0, 0, 1);
       FuzzySet *PP = new FuzzySet(0,1,1,2);
       FuzzySet *MP = new FuzzySet(1, 2, 200, 200);
       
       //Tirado da função de pertinência do delta erro-------------------------
       FuzzySet *MNd = new FuzzySet(-400, -400, -20, -10);
       FuzzySet *PNd = new FuzzySet(-20, -10, -10, 0);
       FuzzySet *ZEd = new FuzzySet(-10, 0, 0, 10);
       FuzzySet *PPd = new FuzzySet(0,10,10,20);
       FuzzySet *MPd = new FuzzySet(10, 20, 400, 400);
       
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


  // Building FuzzyRule "IF Erro=MN and DEerro = MN THEN saida = MA"-------------------------DeltaE = MN
  FuzzyRuleAntecedent* ifErroMNAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroMNd->joinWithAND(MN,MNd);
  thenSaidaMA->addOutput(MA);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifErroMNAndDErroMNd, thenSaidaMA);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Building FuzzyRule "IF Erro=PN and DEerro = MN THEN saida = A "
  FuzzyRuleAntecedent* ifErroPNAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroMNd->joinWithAND(PN,MNd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifErroPNAndDErroMNd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule2);

   // Building FuzzyRule "IF Erro=ZE and DEerro = MN THEN saida = M"
  FuzzyRuleAntecedent* ifErroZEAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroMNd->joinWithAND(ZE,MNd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifErroZEAndDErroMNd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule3);
  
   // Building FuzzyRule "IF Erro=PP and DEerro = MN THEN saida = M"
  FuzzyRuleAntecedent* ifErroPPAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroMNd->joinWithAND(PP,MNd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifErroPPAndDErroMNd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Building FuzzyRule "IF Erro=MP and DEerro = MN THEN saida = B"
  FuzzyRuleAntecedent* ifErroMPAndDErroMNd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroMNd->joinWithAND(MP,MNd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifErroMPAndDErroMNd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule5);
 
    // Building FuzzyRule "IF Erro=MN and DEerro = PN THEN saida = A"-------------------------DeltaE = PN
  FuzzyRuleAntecedent* ifErroMNAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroPNd->joinWithAND(MN,PNd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifErroMNAndDErroPNd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Building FuzzyRule "IF Erro=PN and DEerro = PN THEN saida = A"
  FuzzyRuleAntecedent* ifErroPNAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroPNd->joinWithAND(PN,PNd);
  thenSaidaA->addOutput(A);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifErroPNAndDErroPNd, thenSaidaA);
  fuzzy->addFuzzyRule(fuzzyRule7);

   // Building FuzzyRule "IF Erro=ZE and DEerro = PN THEN saida = B"
  FuzzyRuleAntecedent* ifErroZEAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroPNd->joinWithAND(ZE,PNd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifErroZEAndDErroPNd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule8);
 
   // Building FuzzyRule "IF Erro=PP and DEerro = PN THEN saida = B"
  FuzzyRuleAntecedent* ifErroPPAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroPNd->joinWithAND(PP,PNd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, ifErroPPAndDErroPNd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule9);

   // Building FuzzyRule "IF Erro=MP and DEerro = PN THEN saida = MB"
  FuzzyRuleAntecedent* ifErroMPAndDErroPNd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroPNd->joinWithAND(MP,PNd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule10 = new FuzzyRule(10, ifErroMPAndDErroPNd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule10);
 
     // Building FuzzyRule "IF Erro=MN and DEerro = ZE THEN saida = M"-------------------------DeltaE = ZE
  FuzzyRuleAntecedent* ifErroMNAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroZEd->joinWithAND(MN,ZEd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule11 = new FuzzyRule(11, ifErroMNAndDErroZEd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Building FuzzyRule "IF Erro=PN and DEerro = ZE THEN saida = B"
  FuzzyRuleAntecedent* ifErroPNAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroZEd->joinWithAND(PN,ZEd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule12 = new FuzzyRule(12, ifErroPNAndDErroZEd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule12);

   // Building FuzzyRule "IF Erro=ZE and DEerro = ZE THEN saida = MB"
  FuzzyRuleAntecedent* ifErroZEAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroZEd->joinWithAND(ZE,ZEd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule13 = new FuzzyRule(13, ifErroZEAndDErroZEd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule13);

   // Building FuzzyRule "IF Erro=PP and DEerro = ZE THEN saida = MB"
  FuzzyRuleAntecedent* ifErroPPAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroZEd->joinWithAND(PP,ZEd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule14 = new FuzzyRule(14, ifErroPPAndDErroZEd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule14);

   // Building FuzzyRule "IF Erro=MP and DEerro = ZE THEN saida = MB"
  FuzzyRuleAntecedent* ifErroMPAndDErroZEd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroZEd->joinWithAND(MP,ZEd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule15 = new FuzzyRule(15, ifErroMPAndDErroZEd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule15);

  // Building FuzzyRule "IF Erro=MN and DEerro = PP THEN saida = M"-------------------------DeltaE = PP
  FuzzyRuleAntecedent* ifErroMNAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroPPd->joinWithAND(MN,PPd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule16 = new FuzzyRule(16, ifErroMNAndDErroPPd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule16);

  // Building FuzzyRule "IF Erro=PN and DEerro = PP THEN saida = B"
  FuzzyRuleAntecedent* ifErroPNAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroPPd->joinWithAND(PN,PPd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule17 = new FuzzyRule(17, ifErroPNAndDErroPPd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule17);

   // Building FuzzyRule "IF Erro=ZE and DEerro = PP THEN saida = MB"
  FuzzyRuleAntecedent* ifErroZEAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroPPd->joinWithAND(ZE,PPd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule18 = new FuzzyRule(18, ifErroZEAndDErroPPd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule18);
  
   // Building FuzzyRule "IF Erro=PP and DEerro = PP THEN saida = MB"
  FuzzyRuleAntecedent* ifErroPPAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroPPd->joinWithAND(PP,PPd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule19 = new FuzzyRule(19, ifErroPPAndDErroPPd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule19);
  
   // Building FuzzyRule "IF Erro=MP and DEerro = PP THEN saida = MB"
  FuzzyRuleAntecedent* ifErroMPAndDErroPPd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroPPd->joinWithAND(MP,PPd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule20 = new FuzzyRule(20, ifErroMPAndDErroPPd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule20);
  
  // Building FuzzyRule "IF Erro=MN and DEerro = MP THEN saida = M"-------------------------DeltaE = PP
  FuzzyRuleAntecedent* ifErroMNAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroMNAndDErroMPd->joinWithAND(MN,MPd);
  thenSaidaM->addOutput(M);
  FuzzyRule *fuzzyRule21 = new FuzzyRule(21, ifErroMNAndDErroMPd, thenSaidaM);
  fuzzy->addFuzzyRule(fuzzyRule21);

  // Building FuzzyRule "IF Erro=PN and DEerro = MP THEN saida = B"
  FuzzyRuleAntecedent* ifErroPNAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroPNAndDErroMPd->joinWithAND(PN,MPd);
  thenSaidaB->addOutput(B);
  FuzzyRule *fuzzyRule22 = new FuzzyRule(22, ifErroPNAndDErroMPd, thenSaidaB);
  fuzzy->addFuzzyRule(fuzzyRule22);

   // Building FuzzyRule "IF Erro=ZE and DEerro = MP THEN saida = MB"
  FuzzyRuleAntecedent* ifErroZEAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroZEAndDErroMPd->joinWithAND(ZE,MPd);
  thenSaidaMB->addOutput(B);
  FuzzyRule *fuzzyRule23 = new FuzzyRule(23, ifErroZEAndDErroMPd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule23);
  
   // Building FuzzyRule "IF Erro=PP and DEerro = MP THEN saida = MB"
  FuzzyRuleAntecedent* ifErroPPAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroPPAndDErroMPd->joinWithAND(PP,MPd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule24 = new FuzzyRule(24, ifErroPPAndDErroMPd, thenSaidaMB);
  fuzzy->addFuzzyRule(fuzzyRule24);
  
   // Building FuzzyRule "IF  Erro=MP and DEerro = MP THEN saida = MB"
  FuzzyRuleAntecedent* ifErroMPAndDErroMPd = new FuzzyRuleAntecedent();
  ifErroMPAndDErroMPd->joinWithAND(MP,MPd);
  thenSaidaMB->addOutput(MB);
  FuzzyRule *fuzzyRule25 = new FuzzyRule(25, ifErroMPAndDErroMPd, thenSaidaMB);
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
  if (setpoint == 0){;
      Saida =0;
      PV = 0;
      setpoint =0;
      Erro =0;
      DErro=0;
      Serial.println (String(PV)+";"+String(Saida)+";"+String(Erro)+";"+String(DErro)+";");
      //Serial.println (String(PV)+";"+String(Saida)+";");
   }
   else{  
     Erro=PV-setpoint;
     fuzzy->setInput(1, Erro);
     fuzzy->setInput(2, DErro);
     fuzzy->fuzzify();
     Saida = fuzzy->defuzzify(1);
     Serial.println (String(PV)+";"+String(Saida)+";"+String(Erro)+";"+String(DErro)+";");
     //Serial.println (String(PV)+";"+String(Saida)+";");
     PVanterior=PV;
     //função de transferência
     PV=0.9954*PV+0.002763*Saida;
     DErro=PV-PVanterior;
   }
   delay (200);
}
