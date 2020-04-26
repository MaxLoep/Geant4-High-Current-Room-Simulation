/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>


#include "G4HadronicInteraction.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4INCLXXInterface.hh"
#include "G4INCLXXInterfaceStore.hh"
#include "G4AblaInterface.hh"
#include <vector>



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
  : G4UserRunAction(),
    fDetector(det), fPrimary(prim), fRun(0), fHistoManager(0)
{
 // Book predefined histograms
 fHistoManager = new HistoManager(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
 delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{    
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();

  // keep run condition gun
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetGeneralParticleSource()->GetParticleDefinition();
    G4double energy = fPrimary->GetGeneralParticleSource()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }

  // keep run condition GPS
  if (fPrimary) {
    G4ParticleDefinition* particle
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }



  //histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }

  //couple an old version of the ABLA V3 de-excitation model to INCL++
  // Get hold of pointers to the INCL++ model interfaces
   std::vector<G4HadronicInteraction *> interactions = G4HadronicInteractionRegistry::Instance()
     ->FindAllModels(G4INCLXXInterfaceStore::GetInstance()->getINCLXXVersionName());
   for(std::vector<G4HadronicInteraction *>::const_iterator iInter=interactions.begin(), e=interactions.end();
       iInter!=e; ++iInter) {
     G4INCLXXInterface *theINCLInterface = static_cast<G4INCLXXInterface*>(*iInter);
     if(theINCLInterface) {
       // Instantiate the ABLA model
       G4HadronicInteraction *interaction = G4HadronicInteractionRegistry::Instance()->FindModel("ABLA");
       G4AblaInterface *theAblaInterface = static_cast<G4AblaInterface*>(interaction);
       if(!theAblaInterface)
         theAblaInterface = new G4AblaInterface;
       // Couple INCL++ to ABLA
       G4cout << "Coupling INCLXX to ABLA" << G4endl;
       theINCLInterface->SetDeExcitation(theAblaInterface);
     }
   }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  if (isMaster) fRun->EndOfRun();    
  
  //save histograms      
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
      
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
