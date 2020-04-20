/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"
#include "G4BaryonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"


#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "HadronElasticPhysicsHP.hh"
#include "G4HadronInelasticProcess.hh"

#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"

#include "G4StoppingPhysics.hh"
#include "GammaNuclearPhysics.hh"

#include "ElectromagneticPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4Neutron.hh"
#include "G4Deuteron.hh"
#include "G4ProcessManager.hh"
#include "G4PreCompoundModel.hh"
#include "G4ExcitationHandler.hh"
#include "G4FTFBuilder.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4BinaryLightIonReaction.hh"
#include "G4ComponentGGNuclNuclXsc.hh"
#include "G4HadronicInteraction.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4CrossSectionInelastic.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
:G4VModularPhysicsList(),
 fHadronElastic(nullptr),
 fHadronInelastic(nullptr),
 fIonElastic(nullptr),
 fIonInelastic(nullptr),
 fGammaNuclear(nullptr),
 fElectromagnetic(nullptr),
 fDecay(nullptr),
 fRadioactiveDecay(nullptr)


{
  G4int verb = 0;
  SetVerboseLevel(verb);
  
  //add new units
  //
  new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
    
  // Hadron Elastic scattering
  fHadronElastic = new HadronElasticPhysicsHP(verb);
  RegisterPhysics(fHadronElastic);
  
  // Hadron Inelastic Physics
  //fHadronInelastic = new G4HadronPhysicsFTFP_BERT_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BIC_HP(verb);      //# default
  ////fHadronInelastic = new G4HadronPhysicsQGSP_BIC_AllHP(verb);
  ////fHadronInelastic = new G4HadronInelasticQBBC(verb);
  fHadronInelastic = new G4HadronPhysicsINCLXX(verb);
  ////fHadronInelastic = new G4HadronPhysicsQGSP_BIC_HP(verb);
  //fHadronInelastic = new G4HadronPhysicsQGSP_BERT_HP(verb);      //#


  RegisterPhysics(fHadronInelastic);

  // Ion Elastic Physics
  fIonElastic = new G4IonElasticPhysics(verb);
  RegisterPhysics(fIonElastic);

  // Ion Inelastic Physics
  //fIonInelastic = new G4IonPhysicsXS(verb); //default
  //fIonInelastic = new G4IonPhysicsPHP(verb);
  fIonInelastic = new G4IonINCLXXPhysics(verb);
  RegisterPhysics(fIonInelastic);

  // stopping Particles
  ///RegisterPhysics( new G4StoppingPhysics(verb));

  // Gamma-Nuclear Physics
  fGammaNuclear = new GammaNuclearPhysics("gamma");
  RegisterPhysics(fGammaNuclear);

  // EM physics
  fElectromagnetic = new ElectromagneticPhysics();
  ////fElectromagnetic = new G4EmStandardPhysics();
  RegisterPhysics(fElectromagnetic);

  // Decay
  fDecay = new G4DecayPhysics();
  RegisterPhysics(fDecay);

  // Radioactive decay
  fRadioactiveDecay = new G4RadioactiveDecayPhysics();
  RegisterPhysics(fRadioactiveDecay);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ }


void PhysicsList::ConstructParticle()
{

  //Construct all baryons
  G4BaryonConstructor::ConstructParticle();
  //Construct all leptons
  G4LeptonConstructor::ConstructParticle();
  //Construct mesons
  G4MesonConstructor::ConstructParticle();
  //Construct Ions
  G4IonConstructor::ConstructParticle();
  //Construct Short Lived
  G4ShortLivedConstructor::ConstructParticle();

  //Construct gamma
  G4Gamma::GammaDefinition();
/*
  //construct single particles
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  //mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
  //  baryons
  G4Proton::ProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4Deuteron::DeuteronDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
  G4AntiDeuteron::AntiDeuteronDefinition();
  G4Lambda::LambdaDefinition();
  G4AntiLambda::AntiLambdaDefinition();
  G4SigmaPlus::SigmaPlusDefinition();
  G4SigmaMinus::SigmaMinusDefinition();
  G4SigmaZero::SigmaZeroDefinition();
  G4AntiSigmaPlus::AntiSigmaPlusDefinition();
  G4AntiSigmaMinus::AntiSigmaMinusDefinition();
  G4AntiSigmaZero::AntiSigmaZeroDefinition();
  G4OmegaMinus::OmegaMinusDefinition();
  G4AntiOmegaMinus::AntiOmegaMinusDefinition();
  G4XiMinus::XiMinusDefinition();
  G4XiZero::XiZeroDefinition();
  G4AntiXiMinus::AntiXiMinusDefinition();
  G4AntiXiZero::AntiXiZeroDefinition();
*/
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  // Transportation first (mandatory)
  //
  AddTransportation();

  // Physics constructors
  //
  fHadronElastic->ConstructProcess();
  fHadronInelastic->ConstructProcess();
  fIonElastic->ConstructProcess();
  fIonInelastic->ConstructProcess();
  fGammaNuclear->ConstructProcess();
  fElectromagnetic->ConstructProcess();
  fDecay->ConstructProcess();
  fRadioactiveDecay->ConstructProcess();


/*  // example of GetHadronicModel (due to bug in QGSP_BIC_AllHP)
  //
  G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();
  G4HadronicProcess* process
       = dynamic_cast<G4HadronicProcess*>(pManager->GetProcess("nCapture"));
  G4HadronicInteraction* model = process->GetHadronicModel("nRadCapture");
  if(model) model->SetMinEnergy(19.9*MeV);
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(0*mm, "proton");
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
