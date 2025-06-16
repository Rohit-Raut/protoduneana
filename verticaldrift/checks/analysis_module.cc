// File: analysis_module.cc

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/PFParticle.h"

#include "TTree.h"

namespace duneana {
  class ExampleAna : public art::EDAnalyzer {
  public:
    explicit ExampleAna(fhicl::ParameterSet const& p);
    void beginJob() override;
    void analyze(art::Event const& e) override;

  private:
    art::InputTag fMCTruthTag;
    art::InputTag fMCParticleTag;
    art::InputTag fTrackTag;
    art::InputTag fPFParticleTag;

    TTree* fPrimaryTruthTree;
    TTree* fTrueTree;
    TTree* fRecoTree;

    int fRun;
    int fSubRun;
    int fEvent;

//Full Truth Level Variables for Primaries
int fPrimTrackId, fPrimPdg, fPrimStatus, fPrimMother, fPrimNDaughters, fPrimFirstDaughter, fPrimLastDaughter;
float fPrimVx, fPrimVy, fPrimVz, fPrimT;
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
std::vector<int> fDaughter_trackID;
std::vector<int> fDaughter_pdg;
std::vector<float> fDaughter_E;
std::vector<float> fDaughter_vx, fDaughter_vy, fDaughter_vz;
std::vector<float> fDaughter_px, fDaughter_vy, fDaughter_vz;
std::vector<std::string> fDaughter_process;






//Reco
    float fRecoX, fRecoY, fRecoZ;
    int fRecoPdg;
    float fRecoX_start, fRecoY_start, fRecoZ_start;
    float fRecoX_end, fRecoY_end, fRecoZ_end;
    float fReco_len;
    float fReco_theta, fReco_phi;
    float fReco_chi_per_ndof;
    int  fReco_ID;








  };
}

duneana::ExampleAna::ExampleAna(fhicl::ParameterSet const& p)
  : EDAnalyzer(p),
    fMCTruthTag(p.get<std::string>("MCTruthTag")),
    fMCParticleTag(p.get<std::string>("MCParticleTag")),
    fTrackTag(p.get<std::string>("TrackTag")),
    fPFParticleTag(p.get<std::string>("PFParticleTag")) {}

void duneana::ExampleAna::beginJob() {
  art::ServiceHandle<art::TFileService> tfs;


// Primary MCTruth Tree (1 row per primary particle)
fPrimaryTruthTree = tfs->make<TTree>("PrimaryTruthTree", "Primary MCTruth info");

// Event info
fPrimaryTruthTree->Branch("run",     &fRun,    "run/I");
fPrimaryTruthTree->Branch("subrun",  &fSubRun, "subrun/I");
fPrimaryTruthTree->Branch("event",   &fEvent,  "event/I");

// Primary particle info
fPrimaryTruthTree->Branch("trackId",       &fPrimTrackId,       "trackId/I");
fPrimaryTruthTree->Branch("pdg",           &fPrimPdg,           "pdg/I");
fPrimaryTruthTree->Branch("status",        &fPrimStatus,        "status/I");
fPrimaryTruthTree->Branch("mother",        &fPrimMother,        "mother/I");
fPrimaryTruthTree->Branch("nDaughters",    &fPrimNDaughters,    "nDaughters/I");
fPrimaryTruthTree->Branch("firstDaughter", &fPrimFirstDaughter, "firstDaughter/I");
fPrimaryTruthTree->Branch("lastDaughter",  &fPrimLastDaughter,  "lastDaughter/I");
fPrimaryTruthTree->Branch("process",       &fPrimProcess);
fPrimaryTruthTree->Branch("endProcess",    &fPrimEndProcess);

fPrimaryTruthTree->Branch("vtxx", &fPrimVx, "vtxx/F");
fPrimaryTruthTree->Branch("vtxy", &fPrimVy, "vtxy/F");
fPrimaryTruthTree->Branch("vtxz", &fPrimVz, "vtxz/F");
fPrimaryTruthTree->Branch("t",  &fPrimT,  "t/F");

fPrimaryTruthTree->Branch("Px",   &fPrimPx,   "Px/F");
fPrimaryTruthTree->Branch("Py",   &fPrimPy,   "Py/F");
fPrimaryTruthTree->Branch("Pz",   &fPrimPz,   "Pz/F");
fPrimaryTruthTree->Branch("E",    &fPrimE,    "E/F");
fPrimaryTruthTree->Branch("mass", &fPrimMass, "mass/F");


//Keeping track of the daughter particle
fPrimeTruthTree->Branch("daughter_trackID", &fDaughter_trackID);
fPrimeTruthTree->Branch("daughter_pdg", &fDaughter_pdg);
fPrimeTruthTree->Branch("daughter_E", &fDaughter_E);
fPrimeTruthTree->Branch("daughter_vx", &fDaughter_vx);
fPrimeTruthTree->Branch("daughter_vy", &fDaughter_vy);
fPrimeTruthTree->Branch("daughter_vz", &fDaughter_vz);
fPrimeTruthTree->Branch("daughter_px", &fDaughter_px);
fPrimeTruthTree->Branch("daughter_py", &fDaughter_py);
fPrimeTruthTree->Branch("daughter_pz", &fDaughter_pz);
fPrimeTruthTree->Branch("daughter_process", &fDaughter_process);

  fTrueTree = tfs->make<TTree>("TruthTree", "Primary MCTruth info");

fTrueTree->Branch("run",     &fRun,    "run/I");
fTrueTree->Branch("subrun",  &fSubRun, "subrun/I");
fTrueTree->Branch("event",   &fEvent,  "event/I");

fTrueTree->Branch("trackId", &fTrackId, "trackId/I");
fTrueTree->Branch("status",  &fStatus,  "status/I");
fTrueTree->Branch("pdg",     &fPdgCode, "pdg/I");
fTrueTree->Branch("mother",  &fMother,  "mother/I");
fTrueTree->Branch("process", &fProcess);
fTrueTree->Branch("endprocess", &fEndProcess);

fTrueTree->Branch("vtxx", &fVx, "vtxx/F");
fTrueTree->Branch("vtxy", &fVy, "vtxy/F");
fTrueTree->Branch("vtxz", &fVz, "vtxz/F");
fTrueTree->Branch("t",  &fT,  "t/F");

fTrueTree->Branch("Px", &fPx, "Px/F");
fTrueTree->Branch("Py", &fPy, "Py/F");
fTrueTree->Branch("Pz", &fPz, "Pz/F");
fTrueTree->Branch("E",  &fE,  "E/F");
fTrueTree->Branch("mass", &fMass, "mass/F");

fTrueTree->Branch("endx", &fEndX, "endx/F");
fTrueTree->Branch("endy", &fEndY, "endy/F");
fTrueTree->Branch("endz", &fEndZ, "endz/F");
fTrueTree->Branch("endt", &fEndT, "endt/F");

fTrueTree->Branch("endPx", &fEndPx, "endPx/F");
fTrueTree->Branch("endPy", &fEndPy, "endPy/F");
fTrueTree->Branch("endPz", &fEndPz, "endPz/F");
fTrueTree->Branch("endE",  &fEndE,  "endE/F");

fTrueTree->Branch("polx", &fPolX, "polx/F");
fTrueTree->Branch("poly", &fPolY, "poly/F");
fTrueTree->Branch("polz", &fPolZ, "polz/F");

fTrueTree->Branch("ntrajpoints", &fNTrajPoints, "ntrajpoints/I");
fTrueTree->Branch("ndaughters", &fNDaughters, "ndaughters/I");
fTrueTree->Branch("firstdaughter", &fFirstDaughter, "firstdaughter/I");
fTrueTree->Branch("lastdaughter",  &fLastDaughter,  "lastdaughter/I");

fTrueTree->Branch("rescatter", &fRescatter, "rescatter/I");


 
  fRecoTree = tfs->make<TTree>("recoTree", "Reconstructed Tree");
  fRecoTree->Branch("run",     &fRun,    "run/I");
  fRecoTree->Branch("subrun",  &fSubRun, "subrun/I");
  fRecoTree->Branch("event",   &fEvent,  "event/I");



  fRecoTree->Branch("pdg", &fRecoPdg, "pdg/I");
  fRecoTree->Branch("id", &fReco_ID, "id/I");


  fRecoTree->Branch("len", &fReco_len, "len/F");
  fRecoTree->Branch("start_x", &fRecoX_start, "start_x/F");
  fRecoTree->Branch("start_y", &fRecoY_start, "start_y/F");
  fRecoTree->Branch("start_z", &fRecoZ_start, "start_z/F");

  fRecoTree->Branch("end_x", &fRecoX_end, "end_x/F");
  fRecoTree->Branch("end_y", &fRecoY_end, "end_y/F");
  fRecoTree->Branch("end_z", &fRecoZ_end, "end_z/F");


  fRecoTree->Branch("vtxx", &fRecoX, "vtxx/F");
  fRecoTree->Branch("vtxy", &fRecoY, "vtxy/F");
  fRecoTree->Branch("vtxz", &fRecoZ, "vtxz/F");

  fRecoTree->Branch("theta", &fReco_theta, "theta/F");
  fRecoTree->Branch("phi", &fReco_phi, "phi/F");
  fRecoTree->Branch("chi_ndof", &fReco_chi_per_ndof, "chi_ndof/F");



}




void duneana::ExampleAna::analyze(art::Event const& e) {


// Event info
fRun    = e.run();
fSubRun = e.subRun();
fEvent  = e.id().event();

// Store event metadata
fRun    = e.run();
fSubRun = e.subRun();
fEvent  = e.id().event();


//Clearing all the vector to avoid spilling into the different dataset
fDaughter_trackID.clear();
fDaughter_pdg.clear();
fDaughter_E.clear();
fDaughter_vx.clear();  fDaughter_vy.clear();  fDaughter_vz.clear();
fDaughter_px.clear();  fDaughter_py.clear();  fDaughter_pz.clear();
fDaughter_process.clear();




// Get MCParticle handle
auto const& mcparticleHandle = e.getValidHandle<std::vector<simb::MCParticle>>(fMCParticleTag);

for (auto const& part : *mcparticleHandle) {
  // Fill PrimaryTruthTree (only for primaries)
  if (part.Mother() == 0) {
    fPrimTrackId       = part.TrackId();
    fPrimPdg           = part.PdgCode();
    fPrimStatus        = part.StatusCode();
    fPrimMother        = part.Mother();
    fPrimNDaughters    = part.NumberDaughters();
    fPrimFirstDaughter = (fPrimNDaughters > 0) ? part.FirstDaughter() : -1;
    fPrimLastDaughter  = (fPrimNDaughters > 0) ? part.LastDaughter()  : -1;

    fPrimProcess       = part.Process().empty()    ? "none" : part.Process();
    fPrimEndProcess    = part.EndProcess().empty() ? "none" : part.EndProcess();

    fPrimVx   = part.Vx();
    fPrimVy   = part.Vy();
    fPrimVz   = part.Vz();
    fPrimT    = part.T();

    fPrimPx   = part.Px();
    fPrimPy   = part.Py();
    fPrimPz   = part.Pz();
    fPrimE    = part.E();
    fPrimMass = part.Mass();

    fPrimaryTruthTree->Fill();
  }

  // Fill Truth Tree (for all particles)
  fTrackId  = part.TrackId();
  fStatus   = part.StatusCode();
  fPdgCode  = part.PdgCode();
  fMother   = part.Mother();
  fProcess  = part.Process();
  fEndProcess = part.EndProcess();

  fVx = part.Vx(); fVy = part.Vy(); fVz = part.Vz(); fT = part.T();
  fPx = part.Px(); fPy = part.Py(); fPz = part.Pz(); fE = part.E();
  fMass = part.Mass();

  fEndX = part.EndX(); fEndY = part.EndY(); fEndZ = part.EndZ(); fEndT = part.EndT();
  fEndPx = part.EndPx(); fEndPy = part.EndPy(); fEndPz = part.EndPz(); fEndE = part.EndE();

  fPolX = part.Polarization().X();
  fPolY = part.Polarization().Y();
  fPolZ = part.Polarization().Z();

  fNTrajPoints   = part.NumberTrajectoryPoints();
  fNDaughters    = part.NumberDaughters();
  fFirstDaughter = (fNDaughters > 0) ? part.FirstDaughter() : -1;
  fLastDaughter  = (fNDaughters > 0) ? part.LastDaughter()  : -1;

  fRescatter = static_cast<int>(part.Rescatter());

  fTrueTree->Fill();  // One row per particle
}

  // --- Fill reco tree ---
  auto const& trackHandle = e.getValidHandle<std::vector<recob::Track>>(fTrackTag);
  for (auto const& track : *trackHandle) {
    
    fReco_ID = track.ID();
    fReco_len = track.Length();
    
    fRecoX_start = track.Start().X();
    fRecoY_start = track.Start().Y();
    fRecoZ_start = track.Start().Z();
	
    fRecoX_end = track.End().X();
    fRecoY_end = track.End().Y();
    fRecoZ_end = track.End().Z();

    fReco_theta = track.Theta();
    fReco_phi   = track.Phi();

    if (track.Ndof()>0){
	fReco_chi_per_ndof = track.Chi2()/track.Ndof();
    }
    else{
	fReco_chi_per_ndof = -1.0;
    }



    fRecoPdg = -1; // placeholder
    fRecoTree->Fill();
  }
}

DEFINE_ART_MODULE(duneana::ExampleAna)

