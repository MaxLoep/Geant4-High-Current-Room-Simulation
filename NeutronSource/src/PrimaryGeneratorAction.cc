// Primary particles are generated with the ParticleGun.
// Attempts to implement the GeneralParticleSource (GPS) failed.
// GPS offers more options than ParticleGun like energy distribution of the primary particles
// or beam shape.
// Therefore code snippets of GPS related stuff exists in the code.
// Maybe I come back to this in the future...



/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <G4GeneralParticleSource.hh>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
: G4VUserPrimaryGeneratorAction(), fParticleGun(0), fDetector(det)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fParticleGun->SetParticleEnergy(14*MeV);                              //set not here but in macrofile
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));           //set not here but below
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));  //set not here but below
/*
  fGPS = new G4GeneralParticleSource();

  G4ParticleDefinition* myParticle;
  myParticle = G4ParticleTable::GetParticleTable()->FindParticle("Deuteron");
  fGPS->SetParticleDefinition(myParticle);
  fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(35.*MeV);
  fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
//  fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0,0,0));
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  //delete fGPS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  //particle type
  if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino()) {
    G4int Z = 1, A = 1;
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 0.*keV;

    G4ParticleDefinition* ion
       = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(ionCharge);
  }
/*
  //vertex position uniform within the absorber
  //
  G4double Rmax = fDetector->GetAbsorRadius();
  G4double Rmax2 = Rmax*Rmax;
  G4double R2 = G4UniformRand()*Rmax2;
  G4double R  = std::sqrt(R2);
  
  G4double phi = twopi*G4UniformRand();
  G4double ux = R*std::cos(phi),
           uy = R*std::sin(phi);
  G4double uz = (fDetector->GetAbsorLength())*(G4UniformRand() - 0.5);
  
  fParticleGun->SetParticlePosition(G4ThreeVector(ux,uy,uz));
*/

  //Starting position of the primaries
  //G4double Surface = ((fDetector->GetAbsorLength())+(fDetector->GetContainThickness()));
  G4double Surface = ((fDetector->GetAbsorLength())+(fDetector->GetCylinderThickness()));
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-Surface));
  //fGPS->SetParticlePosition(G4ThreeVector(0,0,-Surface));
  //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-5*cm));
/*
  //distribution uniform in solid angle
  //
  G4double cosTheta = 2*G4UniformRand() - 1.;
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  phi = twopi*G4UniformRand(); 
  G4double vx = sinTheta*std::cos(phi),
           vy = sinTheta*std::sin(phi),
           vz = cosTheta;

  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx,vy,vz));
*/

  //Direction of the primaries
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)); //set particle momentum
  fParticleGun->GeneratePrimaryVertex(anEvent);

  //fGPS->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

