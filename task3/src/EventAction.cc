#include "EventAction.hh"

#include <G4SDManager.hh>
#include <G4THitsMap.hh>
#include <G4SystemOfUnits.hh>
#include <G4Event.hh>

#include "Analysis.hh"

#include "EnergyTimeHit.hh"
#include "EnergyDistHit.hh"

#include <G4RunManager.hh>

using namespace std;


void EventAction::BeginOfEventAction(const G4Event* event)
{

}


void EventAction::EndOfEventAction(const G4Event* event)
{
    G4SDManager* sdm = G4SDManager::GetSDMpointer();
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();


    G4HCofThisEvent* hcofEvent = event->GetHCofThisEvent();

    fScorer_sum = fScorer_sum+1;


    if(!hcofEvent) return;


    if (fScorerId < 0)
      {
	G4cout << "EventAction: scorer energy scorer ID: " << fScorerId << G4endl;
	fScorerId = sdm->GetCollectionID("scorer_ED/energy_dist");
      }
    if (fDetectorId_x < 0)
      {
        G4cout << "EventAction: detector energy scorer ID: " << fDetectorId_x << G4endl;
	fDetectorId_x = sdm->GetCollectionID("detectorET_x/energy_time");
      }
    if (fDetectorId_z < 0)
      {
        G4cout << "EventAction: detector energy scorer ID: " << fDetectorId_z << G4endl;
	fDetectorId_z = sdm->GetCollectionID("detectorET_z/energy_time");
      }
    
    
    G4int histogramId = 1; 
    
    if (fScorerId >= 0)
      {
	EnergyDistHitsCollection* hitCollection_ED = dynamic_cast<EnergyDistHitsCollection*>(hcofEvent->GetHC(fScorerId));
	if (hitCollection_ED)
	  {

            for (auto hit: *hitCollection_ED->GetVector())
	      {
		// here the "save" from the other code is not required as this is now using CRY library
		if((hit->GetParticleName_scorer() == "mu+" || hit->GetParticleName_scorer() == "mu-")){

		  fPhi = std::acos(hit->GetTrackMomDir().x()/(-1*std::sin(hit->GetTrackTheta())));

		  analysis->FillH1(histogramId,hit->GetTrackEnergy());
		  analysis->FillNtupleDColumn(0, 0, hit->GetTrackEnergy());
		  analysis->FillNtupleDColumn(0, 1, hit->GetTrackKineticEnergy());
		  analysis->FillNtupleDColumn(0, 2, hit->GetTrackTheta() - 3.14);		
		  analysis->FillNtupleDColumn(0, 3, fPhi);
		  analysis->FillNtupleDColumn(0, 4, hit->GetTrackPosition().x()/m);
		  analysis->FillNtupleDColumn(0, 5, hit->GetTrackPosition().y()/m);
		  analysis->FillNtupleDColumn(0, 6, hit->GetTrackPosition().z()/m);
		  analysis->FillNtupleDColumn(0, 7, fScorer_sum);		  
		  analysis->FillNtupleDColumn(0, 8, hit->GetTrackMomDir().z());
		  analysis->AddNtupleRow();		  
		  
		}
	      }
	  }
	
      }



    vector<G4int> hitCollectionIds = {
      fDetectorId_z, fDetectorId_x // here looping over both collections
    };

    for (G4int collectionId : hitCollectionIds)
      {
        if (collectionId == -1) continue;
	EnergyTimeHitsCollection* hitCollection = dynamic_cast<EnergyTimeHitsCollection*>(hcofEvent->GetHC(collectionId));
	if (hitCollection)
	  {
            for (auto hit: *hitCollection->GetVector())
	      {

		if(hit->GetParticleName() == "mu+" || hit->GetParticleName() == "mu-"){


		  analysis->FillNtupleDColumn(1,0, hit->GetDeltaEnergy());
		  analysis->FillNtupleDColumn(1,1, hit->GetTime());
		  analysis->FillNtupleDColumn(1,2, hit->GetPosition().x()/m);
		  analysis->FillNtupleDColumn(1,3, hit->GetPosition().y()/m);
		  analysis->FillNtupleDColumn(1,4, hit->GetPosition().z()/m);
		  analysis->FillNtupleDColumn(1, 5, hit->GetParticleID());
		  analysis->FillNtupleDColumn(1, 6, hit->GetPreStepMomDir().x());
		  analysis->FillNtupleDColumn(1, 7, hit->GetPreStepMomDir().y());
		  analysis->FillNtupleDColumn(1, 8, hit->GetPreStepMomDir().z());
		  analysis->FillNtupleDColumn(1, 9, hit->GetPostStepMomDir().x());
		  analysis->FillNtupleDColumn(1, 10, hit->GetPostStepMomDir().y());
		  analysis->FillNtupleDColumn(1, 11, hit->GetPostStepMomDir().z());
		  analysis->FillNtupleDColumn(1, 12, hit->GetPreStepMom().x());
		  analysis->FillNtupleDColumn(1, 13, hit->GetPreStepMom().y());
		  analysis->FillNtupleDColumn(1, 14, hit->GetPreStepMom().z());
		  analysis->FillNtupleDColumn(1, 15, hit->GetPostStepMom().x());
		  analysis->FillNtupleDColumn(1, 16, hit->GetPostStepMom().y());
		  analysis->FillNtupleDColumn(1, 17, hit->GetPostStepMom().z());
		  analysis->FillNtupleDColumn(1, 18, hit->GetKinetic_energy());
		  analysis->FillNtupleDColumn(1, 19, hit->GetTheta());		  
		  analysis->FillNtupleDColumn(1, 20, fScorer_sum);		  
		  analysis->FillNtupleDColumn(1, 21, collectionId);		  
		  analysis->AddNtupleRow(1);

		}


	      }
	  }
      }
}
