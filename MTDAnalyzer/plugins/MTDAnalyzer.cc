#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "DataFormats/ForwardDetId/interface/MTDDetId.h"
#include "DataFormats/ForwardDetId/interface/BTLDetId.h"
#include "DataFormats/FTLDigi/interface/FTLDigiCollections.h"
#include "DataFormats/FTLRecHit/interface/FTLRecHitCollections.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CLHEP/Units/GlobalPhysicalConstants.h"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"



struct MTDinfo {

  float sim_energy;
  float sim_time;
  float sim_x;
  float sim_y;
  float sim_z;

  uint32_t digi_charge[2];
  uint32_t digi_time1[2];
  uint32_t digi_time2[2];

  float ureco_charge[2];
  float ureco_time[2];

  float reco_energy;
  float reco_time;

};



bool orderByDetIdThenTime(const std::tuple<PSimHit,uint32_t,float> &a, 
			  const std::tuple<PSimHit,uint32_t,float> &b) {

  unsigned int detId_a(std::get<1>(a)), detId_b(std::get<1>(b));
      
  if(detId_a<detId_b) return true;
  if(detId_a>detId_b) return false;
      
  double time_a(std::get<2>(a)), time_b(std::get<2>(b));
  if(time_a<time_b) return true;
  
  return false;

}


class MTDAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

public:
  explicit MTDAnalyzer(const edm::ParameterSet&);
  ~MTDAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  // ----------member data ---------------------------

  const float btlIntegrationWindow_;
  const float btlMinEnergy_;


  //edm::EDGetTokenT<reco::GenParticleCollection> tok_genPart; 
  //edm::EDGetTokenT<edm::SimTrackContainer> tok_simTrack; 


  // --- Tracking Particles
  //edm::EDGetTokenT<TrackingParticleCollection> tok_trkPart; 

  // --- MTD SIM hits
  edm::EDGetTokenT<edm::PSimHitContainer> tok_BTL_sim; 
  edm::EDGetTokenT<edm::PSimHitContainer> tok_ETL_sim; 

  // --- MTD DIGI hits
  edm::EDGetTokenT<BTLDigiCollection> tok_BTL_digi; 
  edm::EDGetTokenT<ETLDigiCollection> tok_ETL_digi; 
  
  // --- MTD uncalibrated RECO hits 
  edm::EDGetTokenT<FTLUncalibratedRecHitCollection> tok_BTL_ureco; 
  edm::EDGetTokenT<FTLUncalibratedRecHitCollection> tok_ETL_ureco; 
  
  // --- MTD RECO hits 
  edm::EDGetTokenT<FTLRecHitCollection> tok_BTL_reco; 
  edm::EDGetTokenT<FTLRecHitCollection> tok_ETL_reco; 

  
  std::unordered_map<uint32_t, std::set<int> > n_btl_simHits;
  std::unordered_map<uint32_t, std::set<int> > n_etl_simHits;
  std::unordered_map<uint32_t, MTDinfo> btl_hits;
  std::unordered_map<uint32_t, MTDinfo> etl_hits;
  

  // ============================================================================================
  // Histograms declaration

  // --- SIM

  TH1F *h_n_sim_trk;
  TH1F *h_n_sim_cell;
  TH1F *h_t_sim;
  TH1F *h_e_sim;
  TH1F *h_x_sim;
  TH1F *h_y_sim;
  TH1F *h_z_sim;
  TH2F *h_occupancy_sim;
  TH1F *h_phi_sim;
  TH1F *h_eta_sim;
 
  TH2F *h_t_e_sim;
  TH2F *h_e_eta_sim;
  TH2F *h_t_eta_sim;
  TH2F *h_e_phi_sim;
  TH2F *h_t_phi_sim;

  TProfile *p_t_e_sim;
  TProfile *p_e_eta_sim;
  TProfile *p_t_eta_sim;
  TProfile *p_e_phi_sim;
  TProfile *p_t_phi_sim;


  // --- DIGI

  TH1F *h_n_digi[2];
  TH1F *h_t1_digi[2];
  TH1F *h_t2_digi[2];
  TH1F *h_e_digi[2];
  TH2F *h_occupancy_digi[2];
  TH1F *h_phi_digi[2];
  TH1F *h_eta_digi[2];

  TH2F *h_t1_e_digi[2]; 
  TH2F *h_t2_e_digi[2];
  TH2F *h_e_eta_digi[2];
  TH2F *h_t1_eta_digi[2];
  TH2F *h_t2_eta_digi[2];
  TH2F *h_e_phi_digi[2];
  TH2F *h_t1_phi_digi[2];
  TH2F *h_t2_phi_digi[2];

  TProfile *p_t1_e_digi[2];
  TProfile *p_t2_e_digi[2];
  TProfile *p_e_eta_digi[2];
  TProfile *p_t1_eta_digi[2]; 
  TProfile *p_t2_eta_digi[2];
  TProfile *p_e_phi_digi[2];
  TProfile *p_t1_phi_digi[2];
  TProfile *p_t2_phi_digi[2];


  // --- Uncalibrated RECO

  TH1F *h_n_ureco[2];
  TH1F *h_t_ureco[2];
  TH1F *h_t_ureco_uncorr[2];
  TH1F *h_e_ureco[2];
  TH2F *h_occupancy_ureco[2];

  TH2F *h_t_amp_ureco[2];
  TProfile *p_t_amp_ureco[2];

  // --- RECO

  TH1F *h_n_reco;
  TH2F *h_occupancy_reco;
  TH1F *h_t_reco;
  TH1F *h_t_reco_uncorr;
  TH1F *h_e_reco;

  TH1F *h_t_res;
  TH1F *h_t_res_uncorr;
  TH1F *h_e_res;

  TH2F *h_t_reco_sim;
  TH2F *h_e_reco_sim;

};


MTDAnalyzer::MTDAnalyzer(const edm::ParameterSet& iConfig) :
  btlIntegrationWindow_( iConfig.getParameter<double>("BTLIntegrationWindow") ),
  btlMinEnergy_( iConfig.getParameter<double>("BTLMinimumEnergy") ) {

  tok_BTL_sim = consumes<edm::PSimHitContainer>(edm::InputTag("g4SimHits","FastTimerHitsBarrel"));
  tok_ETL_sim = consumes<edm::PSimHitContainer>(edm::InputTag("g4SimHits","FastTimerHitsEndcap"));

  tok_BTL_digi = consumes<BTLDigiCollection>(edm::InputTag("mix","FTLBarrel"));
  tok_ETL_digi = consumes<ETLDigiCollection>(edm::InputTag("mix","FTLEndcap"));

  tok_BTL_ureco = consumes<FTLUncalibratedRecHitCollection>(edm::InputTag("mtdUncalibratedRecHits","FTLBarrel"));
  tok_ETL_ureco = consumes<FTLUncalibratedRecHitCollection>(edm::InputTag("mtdUncalibratedRecHits","FTLEndcap"));

  tok_BTL_reco = consumes<FTLRecHitCollection>(edm::InputTag("mtdRecHits","FTLBarrel"));
  tok_ETL_reco = consumes<FTLRecHitCollection>(edm::InputTag("mtdRecHits","FTLEndcap"));

  edm::Service<TFileService> fs;


  // ============================================================================================
  // Histograms definition

  // --- SIM

  h_n_sim_trk  = fs->make<TH1F>("h_n_sim_trk", "Number of tracks per BTL cell;N_{trk}", 10, 0., 10.);
  h_n_sim_cell = fs->make<TH1F>("h_n_sim_cell", "Number of BTL cells with SIM hits;N_{BTL cells}", 100, 0., 100.);

  h_t_sim = fs->make<TH1F>("h_t_sim", "SIM hits ToA;ToA [ns]", 500, 0., 50.);
  h_e_sim = fs->make<TH1F>("h_e_sim", "SIM hits energy;E [MeV]", 200, 0., 20.);
  h_x_sim = fs->make<TH1F>("h_x_sim", "SIM local x;x_{SIM} [mm]", 290, -1.45, 1.45);
  h_y_sim = fs->make<TH1F>("h_y_sim", "SIM local y;y_{SIM} [mm]", 600, -30., 30.);
  h_z_sim = fs->make<TH1F>("h_z_sim", "SIM local z;z_{SIM} [MeV]", 400, -2., 2.);

  h_occupancy_sim = fs->make<TH2F>("h_occupancy_sim", "SIM hits occupancy;cell #phi;cell #eta", 
				   145, 0., 2305., 86, -43., 43.);
  h_phi_sim = fs->make<TH1F>("h_phi_sim", "SIM hits #phi;#phi index", 145, 0., 2305.);
  h_eta_sim = fs->make<TH1F>("h_eta_sim", "SIM hits #eta;#eta index", 86, -43., 43.);

  h_t_e_sim   = fs->make<TH2F>("h_t_e_sim", "SIM time vs energy;E_{SIM} [MeV];T_{SIM} [ns]", 
			       100, 0., 20., 100, 0., 50.);
  h_e_eta_sim = fs->make<TH2F>("h_e_eta_sim", "SIM energy vs |#eta|;cell |#eta|;E_{SIM} [MeV]", 
			       43, 0., 43., 100, 0., 20.);
  h_t_eta_sim = fs->make<TH2F>("h_t_eta_sim", "SIM time vs |#eta|;cell |#eta|;T_{SIM} [ns]", 
			       43, 0., 43., 100, 0., 50.);
  h_e_phi_sim = fs->make<TH2F>("h_e_phi_sim", "SIM energy vs #phi;cell #phi;E_{SIM} [MeV]", 
			       145, 0., 2305., 100, 0., 20.);
  h_t_phi_sim = fs->make<TH2F>("h_t_phi_sim", "SIM time vs #phi;cell #phi;T_{SIM} [ns]", 
			       145, 0., 2305., 100, 0., 50.);

  p_t_e_sim   = fs->make<TProfile>("p_t_e_sim", "SIM time vs energy;E_{SIM} [MeV];T_{SIM} [ns]", 
				   100, 0., 20.);
  p_e_eta_sim = fs->make<TProfile>("p_e_eta_sim", "SIM energy vs |#eta|;cell |#eta|;E_{SIM} [MeV]", 
				   43, 0., 43.);
  p_t_eta_sim = fs->make<TProfile>("p_t_eta_sim", "SIM time vs |#eta|;cell |#eta|;T_{SIM} [ns]", 
				   43, 0., 43.);
  p_e_phi_sim = fs->make<TProfile>("p_e_phi_sim", "SIM energy vs #phi;cell #phi;E_{SIM} [MeV]", 
				   145, 0., 2305.);
  p_t_phi_sim = fs->make<TProfile>("p_t_phi_sim", "SIM time vs #phi;cell #phi;T_{SIM} [ns]", 
				   145, 0., 2305.);


  // --- DIGI

  h_n_digi[0]  = fs->make<TH1F>("h_n_digi_0", "Number of DIGI hits (L);N_{DIGI hits}", 100, 0., 100.);
  h_n_digi[1]  = fs->make<TH1F>("h_n_digi_1", "Number of DIGI hits (R);N_{DIGI hits}", 100, 0., 100.);
  h_t1_digi[0] = fs->make<TH1F>("h_t1_digi_0", "DIGI hits ToA1 (L);ToA [TDC counts]", 1024, 0., 1024.);
  h_t1_digi[1] = fs->make<TH1F>("h_t1_digi_1", "DIGI hits ToA1 (R);ToA [TDC counts]", 1024, 0., 1024.);
  h_t2_digi[0] = fs->make<TH1F>("h_t2_digi_0", "DIGI hits ToA2 (L);ToA [TDC counts]", 1024, 0., 1024.);
  h_t2_digi[1] = fs->make<TH1F>("h_t2_digi_1", "DIGI hits ToA2 (R);ToA [TDC counts]", 1024, 0., 1024.);
  h_e_digi[0]  = fs->make<TH1F>("h_e_digi_0", "DIGI hits energy (L);amplitude [ADC counts]", 1024, 0., 1024.);
  h_e_digi[1]  = fs->make<TH1F>("h_e_digi_1", "DIGI hits energy (R);amplitude [ADC counts]", 1024, 0., 1024.);

  h_occupancy_digi[0] = fs->make<TH2F>("h_occupancy_digi_0", "DIGI hits occupancy (L);cell #phi;cell #eta", 
				       145, 0., 2305., 86, -43., 43.);
  h_occupancy_digi[1] = fs->make<TH2F>("h_occupancy_digi_1", "DIGI hits occupancy (R);cell #phi;cell #eta", 
				       145, 0., 2305., 86, -43., 43.);
  h_phi_digi[0] = fs->make<TH1F>("h_phi_digi_0", "DIGI hits #phi (L);#phi index", 145, 0., 2305.);
  h_phi_digi[1] = fs->make<TH1F>("h_phi_digi_1", "DIGI hits #phi (R);#phi index", 145, 0., 2305.);
  h_eta_digi[0] = fs->make<TH1F>("h_eta_digi_0", "DIGI hits #eta (L);#eta index", 86, -43., 43.);
  h_eta_digi[1] = fs->make<TH1F>("h_eta_digi_1", "DIGI hits #eta (R);#eta index", 86, -43., 43.);


  h_t1_e_digi[0]   = fs->make<TH2F>("h_t1_e_digi_0", "DIGI time1 vs charge (L);ADC counts;TDC counts", 
				    128, 0., 1024., 128, 0., 1024.);
  h_t1_e_digi[1]   = fs->make<TH2F>("h_t1_e_digi_1", "DIGI time1 vs charge (R);ADC counts;TDC counts", 
				    128, 0., 1024., 128, 0., 1024.);
  h_t2_e_digi[0]   = fs->make<TH2F>("h_t2_e_digi_0", "DIGI time2 vs charge (L);ADC counts;TDC counts", 
				    128, 0., 1024., 128, 0., 1024.);
  h_t2_e_digi[1]   = fs->make<TH2F>("h_t2_e_digi_1", "DIGI time2 vs charge (R);ADC counts;TDC counts", 
				    128, 0., 1024., 128, 0., 1024.);
  h_e_eta_digi[0]  = fs->make<TH2F>("h_e_eta_digi_0", "DIGI charge vs |#eta| (L);cell |#eta|;ADC counts", 
				    43, 0., 43., 128, 0., 1024.);
  h_e_eta_digi[1]  = fs->make<TH2F>("h_e_eta_digi_1", "DIGI charge vs |#eta| (R);cell |#eta|;ADC counts", 
				    43, 0., 43., 128, 0., 1024.);
  h_t1_eta_digi[0] = fs->make<TH2F>("h_t1_eta_digi_0", "DIGI time1 vs |#eta| (L);cell |#eta|;TDC counts", 
				    43, 0., 43., 128, 0., 1024.);
  h_t1_eta_digi[1] = fs->make<TH2F>("h_t1_eta_digi_1", "DIGI time1 vs |#eta| (R);cell |#eta|;TDC counts", 
				    43, 0., 43., 128, 0., 1024.);
  h_t2_eta_digi[0] = fs->make<TH2F>("h_t2_eta_digi_0", "DIGI time2 vs |#eta| (L);cell |#eta|;TDC counts", 
				    43, 0., 43., 128, 0., 1024.);
  h_t2_eta_digi[1] = fs->make<TH2F>("h_t2_eta_digi_1", "DIGI time2 vs |#eta| (R);cell |#eta|;TDC counts", 
				    43, 0., 43., 128, 0., 1024.);
  h_e_phi_digi[0]  = fs->make<TH2F>("h_e_phi_digi_0", "DIGI charge vs #phi (L);cell #phi;ADC counts", 
				    145, 0., 2305., 128, 0., 1024.);
  h_e_phi_digi[1]  = fs->make<TH2F>("h_e_phi_digi_1", "DIGI charge vs #phi (R);cell #phi;ADC counts", 
				    145, 0., 2305., 128, 0., 1024.);
  h_t1_phi_digi[0] = fs->make<TH2F>("h_t1_phi_digi_0", "DIGI time1 vs #phi (L);cell #phi;TDC counts", 
				    145, 0., 2305., 128, 0., 1024.);
  h_t1_phi_digi[1] = fs->make<TH2F>("h_t1_phi_digi_1", "DIGI time1 vs #phi (R);cell #phi;TDC counts", 
				    145, 0., 2305., 128, 0., 1024.);
  h_t2_phi_digi[0] = fs->make<TH2F>("h_t2_phi_digi_0", "DIGI time2 vs #phi (L);cell #phi;TDC counts", 
				    145, 0., 2305., 128, 0., 1024.);
  h_t2_phi_digi[1] = fs->make<TH2F>("h_t2_phi_digi_1", "DIGI time2 vs #phi (R);cell #phi;TDC counts", 
				    145, 0., 2305., 128, 0., 1024.);

  p_t1_e_digi[0]   = fs->make<TProfile>("p_t1_e_digi_0", "DIGI time1 vs charge (L);ADC counts;TDC counts", 
					128, 0., 1024.);
  p_t1_e_digi[1]   = fs->make<TProfile>("p_t1_e_digi_1", "DIGI time1 vs charge (R);ADC counts;TDC counts", 
					128, 0., 1024.);
  p_t2_e_digi[0]   = fs->make<TProfile>("p_t2_e_digi_0", "DIGI time2 vs charge (L);ADC counts;TDC counts", 
					128, 0., 1024.);
  p_t2_e_digi[1]   = fs->make<TProfile>("p_t2_e_digi_1", "DIGI time2 vs charge (R);ADC counts;TDC counts", 
					128, 0., 1024.);
  p_e_eta_digi[0]  = fs->make<TProfile>("p_e_eta_digi_0", "DIGI charge vs |#eta| (L);cell |#eta|;ADC counts", 
					43, 0., 43.);
  p_e_eta_digi[1]  = fs->make<TProfile>("p_e_eta_digi_1", "DIGI charge vs |#eta| (R);cell |#eta|;ADC counts", 
					43, 0., 43.);
  p_t1_eta_digi[0] = fs->make<TProfile>("p_t1_eta_digi_0", "DIGI time1 vs |#eta| (L);cell |#eta|;TDC counts", 
					43, 0., 43.);
  p_t1_eta_digi[1] = fs->make<TProfile>("p_t1_eta_digi_1", "DIGI time1 vs |#eta| (R);cell |#eta|;TDC counts", 
					43, 0., 43.);
  p_t2_eta_digi[0] = fs->make<TProfile>("p_t2_eta_digi_0", "DIGI time2 vs |#eta| (L);cell |#eta|;TDC counts", 
					43, 0., 43.);
  p_t2_eta_digi[1] = fs->make<TProfile>("p_t2_eta_digi_1", "DIGI time2 vs |#eta| (R);cell |#eta|;TDC counts", 
					43, 0., 43.);
  p_e_phi_digi[0]  = fs->make<TProfile>("p_e_phi_digi_0", "DIGI charge vs #phi (L);cell #phi;ADC counts", 
					145, 0., 2305.);
  p_e_phi_digi[1]  = fs->make<TProfile>("p_e_phi_digi_1", "DIGI charge vs #phi (R);cell #phi;ADC counts", 
					145, 0., 2305.);
  p_t1_phi_digi[0] = fs->make<TProfile>("p_t1_phi_digi_0", "DIGI time1 vs #phi (L);cell #phi;TDC counts", 
					145, 0., 2305.);
  p_t1_phi_digi[1] = fs->make<TProfile>("p_t1_phi_digi_1", "DIGI time1 vs #phi (R);cell #phi;TDC counts", 
					145, 0., 2305.);
  p_t2_phi_digi[0] = fs->make<TProfile>("p_t2_phi_digi_0", "DIGI time2 vs #phi (L);cell #phi;TDC counts", 
					145, 0., 2305.);
  p_t2_phi_digi[1] = fs->make<TProfile>("p_t2_phi_digi_1", "DIGI time2 vs #phi (R);cell #phi;TDC counts", 
					145, 0., 2305.);


  // --- Uncalibrated RECO

  h_n_ureco[0]  = fs->make<TH1F>("h_n_ureco_0", "Number of URECO hits (L);N_{URECO hits}", 100, 0., 100.);
  h_n_ureco[1]  = fs->make<TH1F>("h_n_ureco_1", "Number of URECO hits (R);N_{URECO hits}", 100, 0., 100.);
  h_occupancy_ureco[0] = fs->make<TH2F>("h_occupancy_ureco_0", "URECO hits occupancy (L);cell #phi;cell #eta", 
					145, 0., 2305., 86, -43., 43.);
  h_occupancy_ureco[1] = fs->make<TH2F>("h_occupancy_ureco_1", "URECO hits occupancy (R);cell #phi;cell #eta", 
					145, 0., 2305., 86, -43., 43.);
  h_t_ureco[0] = fs->make<TH1F>("h_t_ureco_0", "URECO hits ToA (L);ToA [ns]", 250, 0., 25.);
  h_t_ureco[1] = fs->make<TH1F>("h_t_ureco_1", "URECO hits ToA (R);ToA [ns]", 250, 0., 25.);
  h_t_ureco_uncorr[0] = fs->make<TH1F>("h_t_ureco_uncorr_0", "URECO hits ToA (L);ToA [ns]", 250, 0., 25.);
  h_t_ureco_uncorr[1] = fs->make<TH1F>("h_t_ureco_uncorr_1", "URECO hits ToA (R);ToA [ns]", 250, 0., 25.);
  h_e_ureco[0] = fs->make<TH1F>("h_e_ureco_0", "URECO hits energy (L);Q [pC]", 300, 0., 600.);
  h_e_ureco[1] = fs->make<TH1F>("h_e_ureco_1", "URECO hits energy (R);Q [pC]", 300, 0., 600.);

  h_t_amp_ureco[0] = fs->make<TH2F>("h_t_amp_ureco_0", "time vs amplitude (L);amplitude [pC];time [ns]", 
				    100, 0., 600., 400, 0., 20.);
  h_t_amp_ureco[1] = fs->make<TH2F>("h_t_amp_ureco_1", "time vs amplitude (R);amplitude [pC];time [ns]", 
				    100, 0., 600., 400, 0., 20.);
  p_t_amp_ureco[0] = fs->make<TProfile>("p_t_amp_ureco_0", "time vs amplitude (L);amplitude [pC];time [ns]", 
					100, 0., 600.);
  p_t_amp_ureco[1] = fs->make<TProfile>("p_t_amp_ureco_1", "time vs amplitude (R);amplitude [pC];time [ns]", 
					100, 0., 600.);


  // --- RECO

  h_n_reco  = fs->make<TH1F>("h_n_reco", "Number of RECO hits;N_{RECO hits}", 100, 0., 100.);
  h_occupancy_reco = fs->make<TH2F>("h_occupancy_reco", "RECO hits occupancy;cell #phi;cell #eta", 
				    145, 0., 2305., 86, -43., 43.);
  h_t_reco  = fs->make<TH1F>("h_t_reco", "RECO hits ToA;ToA [ns]", 250, 0., 25.);
  h_t_reco_uncorr = fs->make<TH1F>("h_t_reco_uncorr", "RECO hits ToA;ToA [ns]", 250, 0., 25.);
  h_e_reco  = fs->make<TH1F>("h_e_reco", "RECO hits energy;E [MeV]", 200, 0., 20.);

  h_t_res  = fs->make<TH1F>("h_t_res", "ToA resolution;ToA [ns]", 700, -2., 5.);
  h_t_res_uncorr = fs->make<TH1F>("h_t_res_uncorr", "ToA resolution;ToA [ns]", 700, -2., 5.);
  h_e_res  = fs->make<TH1F>("h_e_res", "Energy resolution;E [MeV]", 200, -1., 1.);

  h_t_reco_sim = fs->make<TH2F>("h_t_reco_sim", "ToA reco vs sim;SIM ToA [ns];RECO ToA [ns]", 
				100, -1., 25., 100, 0., 25.);
  h_e_reco_sim = fs->make<TH2F>("h_e_reco_sim", "E reco vs sim;SIM E [MeV];RECO E [MeV]", 
				100, 0., 20., 100, 0., 20.);



}


MTDAnalyzer::~MTDAnalyzer() {}


//
// member functions
//

void
MTDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace std;


  edm::Handle<edm::PSimHitContainer>  h_BTL_sim;
  iEvent.getByToken( tok_BTL_sim, h_BTL_sim );
  edm::Handle<edm::PSimHitContainer>  h_ETL_sim;
  iEvent.getByToken( tok_ETL_sim, h_ETL_sim );

  edm::Handle<BTLDigiCollection>   h_BTL_digi;
  iEvent.getByToken( tok_BTL_digi, h_BTL_digi );
  edm::Handle<ETLDigiCollection>   h_ETL_digi;
  iEvent.getByToken( tok_ETL_digi, h_ETL_digi );

  edm::Handle<FTLUncalibratedRecHitCollection> h_BTL_ureco;
  iEvent.getByToken( tok_BTL_ureco, h_BTL_ureco );
  edm::Handle<FTLUncalibratedRecHitCollection> h_ETL_ureco;
  iEvent.getByToken( tok_ETL_ureco, h_ETL_ureco );

  edm::Handle<FTLRecHitCollection> h_BTL_reco;
  iEvent.getByToken( tok_BTL_reco, h_BTL_reco );
  edm::Handle<FTLRecHitCollection> h_ETL_reco;
  iEvent.getByToken( tok_ETL_reco, h_ETL_reco );


  std::set<uint32_t> unique_simHit;



  // ==============================================================================
  //  MTD SIM hits
  // ==============================================================================

  // --- BTL
  if ( h_BTL_sim->size() > 0 ) {
  
    std::vector< std::tuple<PSimHit,uint32_t,float> > hitRefs;
    hitRefs.reserve(h_BTL_sim->size());

    // Sort the SimHits per detector id and time 
    for (auto const& simHit: *h_BTL_sim) {

      DetId id = simHit.detUnitId();
      if (id.rawId() != 0)
	hitRefs.emplace_back( simHit, id.rawId(), simHit.tof() );

    } // simHit loop 
    std::sort(hitRefs.begin(),hitRefs.end(), orderByDetIdThenTime);
    
    // Accumulate the SimHits in the same detector cell
    for (auto const& hitRef: hitRefs) {
    
      const PSimHit &hit = std::get<0>(hitRef);
      DetId id = hit.detUnitId();

      unique_simHit.insert(id.rawId());

      auto simHitIt = btl_hits.emplace(id.rawId(),MTDinfo()).first;

      if ( hit.tof() < btlIntegrationWindow_ ) // This is to emulate the time integration
	                                       // window in the readout electronics.
	(simHitIt->second).sim_energy += 1000.*hit.energyLoss();

      n_btl_simHits[id.rawId()].insert(hit.trackId());

      // Get the time of the first SimHit in the cell
      if( (simHitIt->second).sim_time==0 ) {

	auto hit_pos = hit.localPosition();

	(simHitIt->second).sim_x = hit_pos.x();
	(simHitIt->second).sim_y = hit_pos.y();
	(simHitIt->second).sim_z = hit_pos.z();

	(simHitIt->second).sim_time = hit.tof();

      }


    } // hitRef loop

  } // if ( h_BTL_sim->size() > 0 )



  // ==============================================================================
  //  MTD DIGI hits
  // ==============================================================================

  unsigned int n_digi_btl[2] = {0,0};

  if ( h_BTL_digi->size() > 0 ) {

    for (const auto& dataFrame: *h_BTL_digi) {

      DetId id =  dataFrame.id();
      
      const auto& sample_L = dataFrame.sample(0);
      const auto& sample_R = dataFrame.sample(1);

      btl_hits[id.rawId()].digi_charge[0] = sample_L.data();
      btl_hits[id.rawId()].digi_charge[1] = sample_R.data();
      btl_hits[id.rawId()].digi_time1[0]  = sample_L.toa();
      btl_hits[id.rawId()].digi_time1[1]  = sample_R.toa();
      btl_hits[id.rawId()].digi_time2[0]  = sample_L.toa2();
      btl_hits[id.rawId()].digi_time2[1]  = sample_R.toa2();

      if ( sample_L.data() > 0 )
	n_digi_btl[0]++;

      if ( sample_R.data() > 0 )
	n_digi_btl[1]++;

    } // dataFrame loop

  } // if ( h_BTL_digi->size() > 0 )



  // ==============================================================================
  //  MTD Uncalibrated RECO hits
  // ==============================================================================

  unsigned int n_ureco_btl[2] = {0,0};

  if ( h_BTL_ureco->size() > 0 ) {

    for (const auto& urecHit: *h_BTL_ureco) {

      DetId id = urecHit.id();

      btl_hits[id.rawId()].ureco_charge[0] = urecHit.amplitude().first;
      btl_hits[id.rawId()].ureco_charge[1] = urecHit.amplitude().second;
      btl_hits[id.rawId()].ureco_time[0]   = urecHit.time().first;
      btl_hits[id.rawId()].ureco_time[1]   = urecHit.time().second;

      if ( urecHit.amplitude().first > 0. )
	n_ureco_btl[0]++;

      if ( urecHit.amplitude().second > 0. )
	n_ureco_btl[1]++;
      

    } // recHit loop

  } // if ( h_BTL_reco->size() > 0 )



  // ==============================================================================
  //  MTD RECO hits
  // ==============================================================================

  unsigned int n_reco_btl = 0;

  if ( h_BTL_reco->size() > 0 ) {

    for (const auto& recHit: *h_BTL_reco) {

      DetId id = recHit.id();

      btl_hits[id.rawId()].reco_energy = recHit.energy();
      btl_hits[id.rawId()].reco_time   = recHit.time();

      if ( recHit.energy() > 0. )
	n_reco_btl++;


    } // recHit loop

  } // if ( h_BTL_reco->size() > 0 )



  // ==============================================================================
  //  Fill the histograms
  // ==============================================================================

  for (auto const& hit: n_btl_simHits) {
    h_n_sim_trk->Fill((hit.second).size());
  }
  h_n_sim_cell->Fill(unique_simHit.size());
  for (int iside=0; iside<2; ++iside){
    h_n_digi[iside]->Fill(n_digi_btl[iside]);
    h_n_ureco[iside]->Fill(n_ureco_btl[iside]);
  }
  h_n_reco->Fill(n_reco_btl);

  for (auto const& hit: btl_hits) {

    BTLDetId detId(hit.first); 


    if ( (hit.second).reco_energy < btlMinEnergy_ ) continue;
    

    // --- SIM

    //if ( (hit.second).sim_energy == 0. ) continue;

    int hit_iphi = detId.iphi(BTLDetId::CrysLayout::barzflat);
    int hit_ieta = detId.ieta(BTLDetId::CrysLayout::barzflat);


    h_e_sim->Fill((hit.second).sim_energy);
    h_t_sim->Fill((hit.second).sim_time);

    h_x_sim->Fill((hit.second).sim_x);
    h_y_sim->Fill((hit.second).sim_y);
    h_z_sim->Fill((hit.second).sim_z);

    h_occupancy_sim->Fill(hit_iphi,hit_ieta);
    h_phi_sim->Fill(hit_iphi);
    h_eta_sim->Fill(hit_ieta);
  
    h_t_e_sim->Fill((hit.second).sim_energy,(hit.second).sim_time);
    h_e_eta_sim->Fill(fabs(hit_ieta),(hit.second).sim_energy);
    h_t_eta_sim->Fill(fabs(hit_ieta),(hit.second).sim_time);
    h_e_phi_sim->Fill(hit_iphi,(hit.second).sim_energy);
    h_t_phi_sim->Fill(hit_iphi,(hit.second).sim_time);

    p_t_e_sim->Fill((hit.second).sim_energy,(hit.second).sim_time);
    p_e_eta_sim->Fill(fabs(hit_ieta),(hit.second).sim_energy);
    p_t_eta_sim->Fill(fabs(hit_ieta),(hit.second).sim_time);
    p_e_phi_sim->Fill(hit_iphi,(hit.second).sim_energy);
    p_t_phi_sim->Fill(hit_iphi,(hit.second).sim_time);


    // Time-walk correctio:
    const float p0 =  2.21103;
    const float p1 = -0.933552;
    const float p2 =  0.;
    float time_corr[2];

    for (int iside=0; iside<2; ++iside){

      // --- DIGI

      if ( (hit.second).digi_charge[iside] == 0 ) continue;

      h_e_digi[iside] ->Fill((hit.second).digi_charge[iside]);
      h_t1_digi[iside]->Fill((hit.second).digi_time1[iside]);
      h_t2_digi[iside]->Fill((hit.second).digi_time2[iside]);

      h_occupancy_digi[iside]->Fill(hit_iphi,hit_ieta);
      h_phi_digi[iside]->Fill(hit_iphi);
      h_eta_digi[iside]->Fill(hit_ieta);

      h_t1_e_digi[iside]->Fill((hit.second).digi_charge[iside], (hit.second).digi_time1[iside]);
      h_t2_e_digi[iside]->Fill((hit.second).digi_charge[iside], (hit.second).digi_time2[iside]);
      h_e_eta_digi[iside]->Fill(fabs(hit_ieta),(hit.second).digi_charge[iside]);
      h_t1_eta_digi[iside]->Fill(fabs(hit_ieta),(hit.second).digi_time1[iside]);
      h_t2_eta_digi[iside]->Fill(fabs(hit_ieta),(hit.second).digi_time2[iside]);
      h_e_phi_digi[iside]->Fill(hit_iphi,(hit.second).digi_charge[iside]);
      h_t1_phi_digi[iside]->Fill(hit_iphi,(hit.second).digi_time1[iside]);
      h_t2_phi_digi[iside]->Fill(hit_iphi,(hit.second).digi_time2[iside]);

      p_t1_e_digi[iside]->Fill((hit.second).digi_charge[iside], (hit.second).digi_time1[iside]);
      p_t2_e_digi[iside]->Fill((hit.second).digi_charge[iside], (hit.second).digi_time2[iside]);
      p_e_eta_digi[iside]->Fill(fabs(hit_ieta),(hit.second).digi_charge[iside]);
      p_t1_eta_digi[iside]->Fill(fabs(hit_ieta),(hit.second).digi_time1[iside]);
      p_t2_eta_digi[iside]->Fill(fabs(hit_ieta),(hit.second).digi_time2[iside]);
      p_e_phi_digi[iside]->Fill(hit_iphi,(hit.second).digi_charge[iside]);
      p_t1_phi_digi[iside]->Fill(hit_iphi,(hit.second).digi_time1[iside]);
      p_t2_phi_digi[iside]->Fill(hit_iphi,(hit.second).digi_time2[iside]);


      // --- Uncalibrated RECO

      if ( (hit.second).ureco_charge[iside] == 0. ) continue;

      h_e_ureco[iside]->Fill((hit.second).ureco_charge[iside]);
      h_t_ureco[iside]->Fill((hit.second).ureco_time[iside]);

      h_occupancy_ureco[iside]->Fill(hit_iphi,hit_ieta);

      h_t_amp_ureco[iside]->Fill((hit.second).ureco_charge[iside],(hit.second).ureco_time[iside]);
      p_t_amp_ureco[iside]->Fill((hit.second).ureco_charge[iside],(hit.second).ureco_time[iside]);
    

      // Reverse the global correction

      time_corr[iside] = p0*pow((hit.second).ureco_charge[iside],p1) + p2;

      float ureco_time_uncorr = (hit.second).ureco_time[iside] + time_corr[iside];

      h_t_ureco_uncorr[iside]->Fill(ureco_time_uncorr);


    } // for iside


    // --- RECO

    if ( (hit.second).reco_energy == 0. ) continue;

    h_occupancy_reco->Fill(hit_iphi,hit_ieta);

    h_e_reco->Fill((hit.second).reco_energy);
    h_t_reco->Fill((hit.second).reco_time);
    
    h_e_res->Fill((hit.second).reco_energy-(hit.second).sim_energy);
    h_t_res->Fill((hit.second).reco_time-(hit.second).sim_time);
    
    h_t_reco_sim->Fill((hit.second).sim_time,(hit.second).reco_time);
    h_e_reco_sim->Fill((hit.second).sim_energy,(hit.second).reco_energy);

    
    float reco_time_uncorr = (hit.second).reco_time + 0.5*(time_corr[0]+time_corr[1]); 

    h_t_reco_uncorr->Fill(reco_time_uncorr);
    h_t_res_uncorr->Fill(reco_time_uncorr-(hit.second).sim_time);


  } // hit loop


  n_btl_simHits.clear();
  n_etl_simHits.clear();
  btl_hits.clear();
  etl_hits.clear();

}


// ------------ method called once each job just before starting event loop  ------------
void 
MTDAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MTDAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MTDAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MTDAnalyzer);
