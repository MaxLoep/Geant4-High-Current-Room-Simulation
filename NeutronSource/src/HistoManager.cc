/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("NeutronSource")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms
  
  // Define histograms start values
  const G4int kMaxHisto = 17;
  const G4String id[] = {"0","1","2","3","4","5","6","7","8","9",
                         "10","11","12","13","14","15","16"};
  const G4String title[] = 
      { "dummy",                                                       //0
        "total energy deposit",                                        //1
        "dummy",                                                       //2
        "total kinetic energy flow",                                   //3
        "energy spectrum of emerging gamma",                           //4
        "energy spectrum of emerging e+-",                             //5
        "energy spectrum of emerging neutrons",                        //6
        "energy spectrum of emerging protons",                         //7
        "energy spectrum of emerging deuterons",                       //8
        "energy spectrum of emerging alphas",                          //9
        "energy spectrum of all others emerging ions",                 //10
        "energy spectrum of all others emerging baryons",              //11
        "energy spectrum of all others emerging mesons",               //12
        "energy spectrum of all others emerging leptons (neutrinos)"   //13
        "angular distribution of neutrons (calculated)"                //14
        "angular distribution of protons"                              //15
        "angular distribution of deuterons"                            //16
      };

  // Default values (to be reset via /analysis/h1/set command) 
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
