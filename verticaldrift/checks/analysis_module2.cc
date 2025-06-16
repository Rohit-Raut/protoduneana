//file: analysis_module.cc

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "art_root_io/TFileService.h"


#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/PFParticle.h"

#include "TTree.h"

namespace duneana{
	class analysis: public art:: EDAnalyzer{
	public:
		explicit analysis(fhicl::ParameterSet const& p);
		void beginJob() Override;
		void analyze(art::Event const& e) override;
	private:
		art::InputTag fMCTruthTag;
		art::InputTag fMCParticleTag;
		art::InputTag fTrackTag;
		art::InputTag fPFParticleTag;
	TTree* fAnalysis;
	int fRun;
	int fSubRun;
	int fEvent;
	//Full Truth Level Variables for Primaries
	int fPrimTrackId, fPrimPdg, fPrimStatus, fPrimMother, fPrimNDaughters, fPrimFirstDaughter, fPrimLastDaughter;
	float fPrimVx, fPrimVy, fPrimVz, fPrimT;
	float fPrim_Edep;
	int fNSecondaries;

	float fPrimPx, fPrimPy, fPrimPz, fPrimE, fPrimMass;
	std::string fPrimProcess, fPrimEndProcess;

	//Full Truth Level Variables for all particles
	int fTrackId, fStatus, fPdgCode, fMother;
	std::string fProcess, fEndProcess;

	float fVx, fVy, fVz, fT;
	float fPx, fPy, fPz, fE, fMass;
	float fEndX, fEndY, fEndZ, fEndT;
	float fEndPx, fEndPy, fEndPz, fEndE;
	float fPolX, fPolY, fPolZ;

	int fNTrajPoints, fNDaughters, fFirstDaughter, fLastDaughter;
	int fRescatter;

	//Reco
    	float fRecoX, fRecoY, fRecoZ;
    	int fRecoPdgi;
	float fRecoFoundTrck;
	float fRecoLength;
	float fReco_Chi2_per_NDOF;
	float fReco_Avg_dQdx_p2;
	float fReco_TotalCharge_p2;
	
	};
}
duneana::analysis::analysis(fhicl::ParameterSet const& p)
	:EDAnalyzer(p),
	fMCTruthTag(p.get<std::string>("MCTruthTag")),
	fMCParticleTag(p.get<std::string>("MCParticleTag")),
	fTrackTag(p.get<std::string>("TrackTag")),	
	fPFParticleTag(p.get<std::string>("PFParticleTag")){}

void duneana::analysis::beginJob(){
	art::ServiceHandle<art::TFileService> tfs;
	fAnalysis=tfs->make<TTree>("analysis","Muon 1GeV");
	fAnalysis->Branch("run", &fRun, "run/I");
	fAnalysis->Branch("subrun",&fSubRun, "subrun/I");
	fAnalysis->Branch("event",&fEvent,"event/I");

	//Primary Particle table
	fAnalysis->Branch("Ppdg",&fPrimPdg,"Ppdg/I");
	fAnalysis->Branch("PtrackID",&fPrimTrackID, "PtrackID/I");
	fAnalysis->Branch("Pstatus", &fPrimStatus, "Pstatus/I");
	fAnalysis->Branch("Pmother", &fPrimMother, "Pmother/I");
	fAnalysis->Branch("PnDaughter", &fPrimNDaughters, "PnDaughter/I");
	fAnalysis->Branch("PfirstDaughter",&fPrimFirstDaughter, "PfirstDaughter/I");
	fAnalysis->Branch("PlastDaughter",&fPrimLastDaughter, "PlastDaughter/I");
	
	fAnalysis->Branch("Pprocess", &fPrimProcess);
	fAnalysis->Branch("PendProcess",&fPrimEndProcess);
	
	fAnalysis->Branch("Pvx", &fPrimVx, "vx/F");
	fAnalysis->Branch("Pvy", &fPrimVy, "vy/F");
	fAnalysis->Branch("Pvz", &fPrimVz, "vz/F");	
	fAnalysis->Branch("Pt", &fPrimT, "t/F");
	
	fAnalysis->Branch("Px", &fPrimPx, "Px/F");
	fAnalysis->Branch("Py", &fPrimPy, "Py/F");
	fAnalysis->Branch("Pz", &fPrimPz, "Pz/F");
	fAnalysis->Branch("E", &fPrimMass, "mass/F");
	fAnalysis->Branch("mass", &fPrimMass, "mass/F");

	fAnalysis->Branch("



































