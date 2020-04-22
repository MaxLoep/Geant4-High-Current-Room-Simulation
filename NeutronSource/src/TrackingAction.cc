/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TrackingAction.hh"

#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* event)
:G4UserTrackingAction(), fEventAction(event)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{  
  //count secondary particles
  if (track->GetTrackID() == 1) return;  
  G4String name   = track->GetDefinition()->GetParticleName();
  G4double energy = track->GetKineticEnergy();
  //G4ThreeVector momentum = track->GetMomentum();
  //G4double mx = momentum.x();
  //G4double my = momentum.y();
  //G4double mz = momentum.z();
  //G4double mphi = momentum.phi();
  //G4double mtheta = momentum.theta();
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());    
  run->ParticleCount(name,energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
 // keep only outgoing particle
 G4StepStatus status = track->GetStep()->GetPostStepPoint()->GetStepStatus();
 if (status != fWorldBoundary) return;
 
 const G4ParticleDefinition* particle = track->GetParticleDefinition();
 G4String name   = particle->GetParticleName();
 //if (particle != G4Neutron::Neutron()) return;
 G4double energy = track->GetKineticEnergy();
 G4ThreeVector momentum = track->GetMomentum();
 G4double mx = momentum.x();
 G4double my = momentum.y();
 G4double mz = momentum.z();
 //G4double mphi = momentum.phi();
 //G4double mtheta = momentum.theta();
 G4double angle = std::acos(mz/(sqrt(mx*mx+my*my+mz*mz)));

 //Cuts - only count what fulfils the cut criteria
 //if (angle > 1.57) return;      //90 degree cutoff
 //if (angle > 0.087266) return;  //5 degree cutoff
 //if (angle > 0.01745329) return;  //1 degree cutoff
 if (angle > 0.00872664626) return;  //0.5 degree cutoff - paper
 //if (angle > 0.001745329) return;  //0.1 degree cutoff
 //if (energy < 0.1*MeV) return;    //5.1 MeV Cutoff

 fEventAction->AddEflow(energy);  
 
 Run* run = static_cast<Run*>(
              G4RunManager::GetRunManager()->GetNonConstCurrentRun());
 run->ParticleFlux(name,energy);               
 
 // histograms: enery flow
 //
 G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
 G4int ih = 0; 
 G4String type   = particle->GetParticleType();      
 G4double charge = particle->GetPDGCharge();
 if (charge > 3.)  ih = 10; 
 else if (particle == G4Gamma::Gamma())       ih = 4;
 else if (particle == G4Electron::Electron()) ih = 5;
 else if (particle == G4Positron::Positron()) ih = 5;
 else if (particle == G4Neutron::Neutron())   ih = 6;
 else if (particle == G4Proton::Proton())     ih = 7;
 else if (particle == G4Deuteron::Deuteron()) ih = 8;
 else if (particle == G4Alpha::Alpha())       ih = 9;
 else if (type == "nucleus")                  ih = 10;
 else if (type == "baryon")                   ih = 11;
 else if (type == "meson")                    ih = 12;
 else if (type == "lepton")                   ih = 13;
 if (ih > 0) analysis->FillH1(ih,energy);
 //if (ih > 6) analysis->FillH1(ih,angle);
 if (particle == G4Neutron::Neutron()) analysis->FillH1(14,angle);
 if (particle == G4Proton::Proton()) analysis->FillH1(15,angle);
 if (particle == G4Deuteron::Deuteron()) analysis->FillH1(16,angle);
// if (ih = 6) analysis->FillH1(15,mphi);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

