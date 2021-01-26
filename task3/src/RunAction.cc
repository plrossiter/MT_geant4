#include "RunAction.hh"
#include "Analysis.hh"
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4AccumulableManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4MuonPlus.hh>
#include <G4MuonMinus.hh>

#include <G4RunManager.hh>


RunAction::RunAction() :
  G4UserRunAction(),
  fNGammas("NGammas", 0),
  fNElectrons("NElectrons", 0),
  fNMuPlus("NMuPlus", 0),
  fNMuMinus("NMuMinus", 0),
  fNMuPlus_primary("NMuPlus_primary", 0),
  fNMuMinus_primary("NMuMinus_primary", 0),
  fAverageGammaEnergy("AvgGammaEnergy",0.),
  fAverageElectronEnergy("AvgElectronEnergy",0.),
  fAverageMuPlusEnergy("AvgMuPlusEnergy",0.),
  fAverageMuMinusEnergy("AvgMuMinusEnergy",0.),
  fTotalTrackLength("TotalTrackLength",0.)
{


  // Register created accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fNGammas);
  accumulableManager->RegisterAccumulable(fNElectrons);
  accumulableManager->RegisterAccumulable(fNMuPlus);
  accumulableManager->RegisterAccumulable(fNMuMinus);
  accumulableManager->RegisterAccumulable(fNMuPlus_primary);
  accumulableManager->RegisterAccumulable(fNMuMinus_primary);
  accumulableManager->RegisterAccumulable(fAverageGammaEnergy);
  accumulableManager->RegisterAccumulable(fAverageElectronEnergy);
  accumulableManager->RegisterAccumulable(fAverageMuPlusEnergy);
  accumulableManager->RegisterAccumulable(fAverageMuMinusEnergy);
  accumulableManager->RegisterAccumulable(fTotalTrackLength);
  

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstNtupleId(0);
  analysisManager->SetFirstHistoId(1); 
  analysisManager->SetNtupleMerging(true);

  analysisManager->CreateH1("mu_init_ene", "mu_init_ene" ,1000, 1000, 1000000);

  analysisManager->CreateNtuple("1","EneDist");
  analysisManager->CreateNtupleDColumn("TotalEnergyDist");
  analysisManager->CreateNtupleDColumn("KineticEnergyDist");
  analysisManager->CreateNtupleDColumn("ThetaDist");
  analysisManager->CreateNtupleDColumn("PhiDist");
  analysisManager->CreateNtupleDColumn("XDist");
  analysisManager->CreateNtupleDColumn("YDist");
  analysisManager->CreateNtupleDColumn("ZDist");
  analysisManager->CreateNtupleDColumn("EventNumber");
  analysisManager->CreateNtupleDColumn("ZMomDir");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("2","muons");
  analysisManager->CreateNtupleDColumn("EnergyDeposit");
  analysisManager->CreateNtupleDColumn("Time");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("Z");
  analysisManager->CreateNtupleDColumn("ParticleId");
  analysisManager->CreateNtupleDColumn("PreStep_mom_dir_X");
  analysisManager->CreateNtupleDColumn("PreStep_mom_dir_Y");
  analysisManager->CreateNtupleDColumn("PreStep_mom_dir_Z");
  analysisManager->CreateNtupleDColumn("PostStep_mom_dir_X");
  analysisManager->CreateNtupleDColumn("PostStep_mom_dir_Y");
  analysisManager->CreateNtupleDColumn("PostStep_mom_dir_Z");
  analysisManager->CreateNtupleDColumn("PreStep_mom_X");
  analysisManager->CreateNtupleDColumn("PreStep_mom_Y");
  analysisManager->CreateNtupleDColumn("PreStep_mom_Z");
  analysisManager->CreateNtupleDColumn("PostStep_mom_X");
  analysisManager->CreateNtupleDColumn("PostStep_mom_Y");
  analysisManager->CreateNtupleDColumn("PostStep_mom_Z");
  analysisManager->CreateNtupleDColumn("Kinetic_Energy");
  analysisManager->CreateNtupleDColumn("Theta");
  analysisManager->CreateNtupleDColumn("EventNumber");
  analysisManager->CreateNtupleDColumn("DetectorId");
  analysisManager->FinishNtuple();

  analysisManager->OpenFile("output");  


}


void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  //retrieve the number of events produced in the run
  G4int nofEvents = run->GetNumberOfEvent();

  //do nothing, if no events were processed
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  if (IsMaster())
  {
    G4cout
     << "\n--------------------End of Global Run-----------------------"
     << " \n The run was " << nofEvents << " events " << G4endl;
    if (fNGammas.GetValue())
      G4cout << " * Produced " << fNGammas.GetValue()/((G4double)nofEvents) <<
	" secondary gammas/event. Average energy: "
	     << fAverageGammaEnergy.GetValue()/keV/fNGammas.GetValue() << " keV" << G4endl;
    else
      G4cout << " * No secondary gammas produced" << G4endl;
    if (fNElectrons.GetValue())
      G4cout << " * Produced " << fNElectrons.GetValue()/((G4double)nofEvents)  <<
	" secondary electrons/event. Average energy: "
	     << fAverageElectronEnergy.GetValue()/keV/fNElectrons.GetValue() << " keV" << G4endl;
    else
      G4cout << " * No secondary electrons produced" << G4endl;
    if (fNMuPlus.GetValue())
      G4cout << " * Produced " << fNMuPlus.GetValue()/((G4double)nofEvents)  <<
	" secondary mu+/event. Average energy: "
	     << fAverageMuPlusEnergy.GetValue()/keV/fNMuPlus.GetValue() << " keV" << G4endl;
    else
      G4cout << " * No secondary mu+ produced" << G4endl;
    if (fNMuMinus.GetValue())
      G4cout << " * Produced " << fNMuMinus.GetValue()/((G4double)nofEvents)  <<
	" secondary mu-/event. Average energy: "
	     << fAverageMuMinusEnergy.GetValue()/keV/fNMuMinus.GetValue() << " keV" << G4endl;
    else
      G4cout << " * No secondary mu- produced" << G4endl;
    if (fTotalTrackLength.GetValue())
     {
        G4cout << " * Total track length of muons in world volume: ";
        G4cout << fTotalTrackLength.GetValue() / mm << " mm" << G4endl;
     }

  }
}

RunAction::~RunAction()
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write(); 
}

void RunAction::AddPrimary(const G4ParticleDefinition* particle,
			     G4double energy)
{
  if (particle == G4MuonPlus::Definition())
    {
      fNMuPlus_primary += 1;
    } 
  else if (particle == G4MuonMinus::Definition())
    {
      fNMuMinus_primary += 1;
    } 
  return;
}

void RunAction::AddSecondary(const G4ParticleDefinition* particle,
			     G4double energy)
{
  if (particle == G4Gamma::Definition())
    {
      fNGammas += 1;
      fAverageGammaEnergy += energy;
    }
  else if (particle == G4Electron::Definition())
    {
      fNElectrons += 1;
      fAverageElectronEnergy += energy;
    } 
 else if (particle == G4MuonPlus::Definition())
    {
      fNMuPlus += 1;
      fAverageMuPlusEnergy += energy;
    } 
 else if (particle == G4MuonMinus::Definition())
    {
      fNMuMinus += 1;
      fAverageMuMinusEnergy += energy;
    } 
  return;
}

void RunAction::AddTrackLength(G4double trackLength)
{
  fTotalTrackLength += trackLength;
}
