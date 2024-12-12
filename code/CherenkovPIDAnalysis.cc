// Copyright 2023, Christopher Dilks
// Subject to the terms in the LICENSE file found in the top-level directory.

#include "CherenkovPIDAnalysis.h"

namespace benchmarks {

  // AlgorithmInit
  //---------------------------------------------------------------------------
  void CherenkovPIDAnalysis::AlgorithmInit(
      std::string                      rad_name,
      std::shared_ptr<spdlog::logger>& logger
      )
  {
    m_rad_name = rad_name;
    m_log      = logger;

    // set radiator title
    m_rad_title = TString(m_rad_name);
    if(m_rad_name=="Merged") m_rad_title += " Aerogel+Gas";

    // truncate `m_rad_name` (for object names)
    m_rad_name_trun = TString(m_rad_name);
    m_rad_name_trun(TRegexp(" .*")) = "";

    // distributions
    m_npe_dist = new TH1D(
        "npe_dist_"+m_rad_name_trun,
        "Overall NPE for "+m_rad_title+";NPE",
        Tools::npe_max, 0, Tools::npe_max
        );
    m_theta_dist = new TH1D(
        "theta_dist_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle for "+m_rad_title+";#theta [mrad]",
        Tools::theta_bins, 0, Tools::theta_max
        );
    m_thetaResid_dist = new TH1D(
        "thetaResid_dist_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle Residual for "+m_rad_title+";#Delta#theta [mrad]",
        Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
        );
    m_photonTheta_vs_photonPhi = new TH2D(
        "photonTheta_vs_photonPhi_"+m_rad_name_trun,
        "Reconstructed Photon #theta vs #phi for "+m_rad_title+";#phi [rad];#theta [mrad]",
        Tools::phi_bins, -TMath::Pi(), TMath::Pi(),
        Tools::theta_bins, 0, Tools::theta_max
        );
    m_speTheta_dist = new TH1D(
      "speTheta_dist_"+m_rad_name_trun,
      "Reconstructed Photon SPE #theta for "+m_rad_title+";#theta[mrad]",
      Tools::theta_bins, 0, Tools::theta_max
    );
     
    m_speResid_dist = new TH1D(
      "speResid_dist_"+m_rad_name_trun,
      "Reconstructed SPE Cherenkov Angle Residual for "+m_rad_title+";#Delta#theta [mrad]",
      Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
    );

    //truth and reconstruction check
    m_speTheta_vs_mcWavelength = new TH2D(
      "speTheta_vs_mcW_"+m_rad_name_trun,
      "Dependency of SPE Reconstruction with true mc wavelength for "+m_rad_title+";[nm];[mrad]",
      Tools::n_bins,0,1000,Tools::theta_bins,0,Tools::theta_max
    );
    m_speResid_vs_mcWavelength = new TH2D(
      "speResid_vs_mcW_"+m_rad_name_trun,
      "Dependency of SPE Residual with true mc wavelength for "+m_rad_title+";[nm];[mrad]",
      Tools::n_bins,0,1000,Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
    );
    
    // truth distributions
    m_mcWavelength_dist = new TH1D(
        "mcWavelength_"+m_rad_name_trun,
        "MC Photon Wavelength for "+m_rad_title+";#lambda [nm]",
        Tools::n_bins, 0, 1000
        );
    m_mcRindex_dist = new TH1D(
        "mcRindex_"+m_rad_name_trun,
        "MC Refractive Index for "+m_rad_title+";n",
        10*Tools::n_bins, 0.99, 1.03
        );

    // PID
    m_highestWeight_dist = new TH1D(
        "highestWeight_dist_"+m_rad_name_trun,
        "Highest PDG Weight for "+m_rad_title+";PDG",
        Tools::GetNumPDGs()+1, 0, Tools::GetNumPDGs()+1
        );

    // momentum scans
    m_npe_vs_p = new TH2D(
        "npe_vs_p_"+m_rad_name_trun,
        "Overall NPE vs. Particle Momentum for "+m_rad_title+";p [GeV];NPE",
        Tools::momentum_bins, 0, Tools::momentum_max,
        Tools::npe_max, 0, Tools::npe_max
        );
    m_theta_vs_p = new TH2D(
        "theta_vs_p_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle vs. Particle Momentum for "+m_rad_title+";p [GeV];#theta [mrad]",
        Tools::momentum_bins, 0, Tools::momentum_max,
        Tools::theta_bins, 0, Tools::theta_max
        );
    m_thetaResid_vs_p = new TH2D(
        "thetaResid_vs_p_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle Residual vs. Particle Momentum for "+m_rad_title+";p [GeV];#Delta#theta [mrad]",
        Tools::momentum_bins, 0, Tools::momentum_max,
        Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
        );
    m_spetheta_vs_p = new TH2D(
        "spetheta_vs_p_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle vs. Particle Momentum for "+m_rad_title+";p [GeV];#theta [mrad]",
        Tools::momentum_bins, 0, Tools::momentum_max,
        Tools::theta_bins, 0, Tools::theta_max
        );
    m_spethetaResid_vs_p = new TH2D(
        "speResid_vs_p_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle Residual vs. Particle Momentum for "+m_rad_title+";p [GeV];#Delta#theta [mrad]",
        Tools::momentum_bins, 0, Tools::momentum_max,
        Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
        );
    m_highestWeight_vs_p = new TH2D(
        "highestWeight_vs_p_"+m_rad_name_trun,
        "Highest PDG Weight vs. Particle Momentum for "+m_rad_title+";p [GeV]",
        Tools::momentum_bins, 0, Tools::momentum_max,
        Tools::GetNumPDGs()+1, 0, Tools::GetNumPDGs()+1
        );

    // pseudorapidity scans
    m_npe_vs_eta = new TH2D(
        "npe_vs_eta_"+m_rad_name_trun,
        "Overall NPE vs. Pseudorapidity for "+m_rad_title+";#eta;NPE",
        Tools::eta_bins, Tools::eta_min, Tools::eta_max,
        Tools::npe_max, 0, Tools::npe_max
        );
    m_theta_vs_eta = new TH2D(
        "theta_vs_eta_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle vs. Pseudorapidity for "+m_rad_title+";#eta;#theta [mrad]",
        Tools::eta_bins, Tools::eta_min, Tools::eta_max,
        Tools::theta_bins, 0, Tools::theta_max
        );
    m_thetaResid_vs_eta = new TH2D(
        "thetaResid_vs_eta_"+m_rad_name_trun,
        "Reconstructed Cherenkov Angle Residual vs. Pseudorapidity for "+m_rad_title+";#eta;#Delta#theta [mrad]",
        Tools::eta_bins, Tools::eta_min, Tools::eta_max,
        Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
        );
    m_spetheta_vs_eta = new TH2D(
        "spetheta_vs_eta_"+m_rad_name_trun,
        "Reconstructed SPE Cherenkov Angle vs. Pseudorapidity for "+m_rad_title+";#eta;#theta [mrad]",
        Tools::eta_bins, Tools::eta_min, Tools::eta_max,
        Tools::theta_bins, 0, Tools::theta_max
        );
    m_spethetaResid_vs_eta = new TH2D(
        "speResid_vs_eta_"+m_rad_name_trun,
        "Reconstructed SPE Cherenkov Angle Residual vs. Pseudorapidity for "+m_rad_title+";#eta;#Delta#theta [mrad]",
        Tools::eta_bins, Tools::eta_min, Tools::eta_max,
        Tools::theta_bins, -Tools::thetaResid_max, Tools::thetaResid_max
        );
    m_highestWeight_vs_eta = new TH2D(
        "highestWeight_vs_eta_"+m_rad_name_trun,
        "Highest PDG Weight vs. Pseudorapidity for "+m_rad_title+";#eta",
        Tools::eta_bins, Tools::eta_min, Tools::eta_max,
        Tools::GetNumPDGs()+1, 0, Tools::GetNumPDGs()+1
        );
    // mass square plots                                                                 // ADDED BY NEBIN
    m_mass_square_vs_p_ = new TH2D(                                                     // ADDED BY NEBIN
        "mass_square_vs_p_"+m_rad_name_trun,                                           // ADDED BY NEBIN
        "mass_square vs p" +m_rad_title+";p (GeV/c) ;mass^{2} (GeV/c^{2})^{2}",         // ADDED BY NEBIN			
        600, 0, 60,                                                                  // ADDED BY NEBIN
        100, -2, 2                                                                  // ADDED BY NEBIN
        );                                                                         // ADDED BY NEBIN                                      
    m_spe_mass_square_vs_p_ = new TH2D(                                            // ADDED BY NEBIN
        "spe_mass_square_vs_p_"+m_rad_name_trun,                                           // ADDED BY NEBIN
        "spe_mass_square vs p" +m_rad_title+";p (GeV/c) ;mass^{2} (GeV/c^{2})^{2}",                                    // ADDED BY NEBIN
        600, 0, 60,                                  // ADDED BY NEBIN
        100, -2, 2                              // ADDED BY NEBIN
        );                                                                        // ADDED BY NEBIN  

    // format histograms
    auto format1D = [] (auto h) {
      h->SetLineColor(kAzure);
      h->SetFillColor(kAzure);
    };
    format1D(m_npe_dist);
    format1D(m_theta_dist);
    format1D(m_thetaResid_dist);
    format1D(m_mcWavelength_dist);
    format1D(m_speTheta_dist);
    format1D(m_speResid_dist);
    format1D(m_mcRindex_dist);
    format1D(m_highestWeight_dist);
  }


  // AlgorithmProcess
  //---------------------------------------------------------------------------
  void CherenkovPIDAnalysis::AlgorithmProcess(
      const edm4hep::MCParticleCollection&          mc_parts,
      const edm4eic::CherenkovParticleIDCollection& cherenkov_pids
      )
  {
    m_log->trace("-> {} Radiator:", m_rad_name);

    // get thrown momentum from a single MCParticle
    auto part = std::make_unique<ChargedParticle>(m_log);
    part->SetSingleParticle(mc_parts);
    auto thrown_momentum = part->GetMomentum();
    auto thrown_eta      = part->GetEta();
    auto thrown_pdg      = part->GetPDG();

    // loop over `CherenkovParticleID` objects
    for(const auto& cherenkov_pid : cherenkov_pids) {

      // skip if NPE==0
      if(cherenkov_pid.getNpe() == 0) {
        m_log->warn("Event found with NPE=0");
        continue;
      }

      // estimate the charged particle momentum using the momentum of the first TrackPoint at this radiator's entrance
      /*
      auto charged_particle = cherenkov_pid.getChargedParticle();
      if(!charged_particle.isAvailable())   { m_log->warn("Charged particle not available in this radiator");      continue; }
      if(charged_particle.points_size()==0) { m_log->warn("Charged particle has no TrackPoints in this radiator"); continue; }
      auto charged_particle_momentum = edm4hep::utils::magnitude( charged_particle.getPoints(0).momentum );
      m_log->trace("  Charged Particle p = {} GeV at radiator entrance", charged_particle_momentum);
      m_log->trace("  If it is a pion, E = {} GeV", std::hypot(charged_particle_momentum, Tools::GetPDGMass(211)));
      */
      // alternatively: use `thrown_momentum` (FIXME: will not work for multi-track events)
      auto charged_particle_momentum = thrown_momentum;
      auto charged_particle_eta      = thrown_eta;
      auto charged_particle_pdg      = thrown_pdg;
      auto charged_particle_mass     = Tools::GetPDGMass(charged_particle_pdg);
      m_log->trace("  Charged Particle PDG={}, mass={} GeV, p={} GeV from truth",
          charged_particle_pdg, charged_particle_mass, charged_particle_momentum);

      // get average Cherenkov angle: `theta_rec`
      double theta_rec = 0.0;
      //for(const auto& [theta,phi] : cherenkov_pid.getThetaPhiPhotons())
        //theta_rec += theta;
      //theta_rec /= cherenkov_pid.getNpe();
      //auto theta_rec_mrad = theta_rec * 1e3; // [rad] -> [mrad]

      // calculate expected Cherenkov angle `theta_exp` and residual `theta_resid`,
      // using refractive index from MC truth
      float mc_rindex=0; 
      if(m_rad_name=="Aerogel") mc_rindex =1.026; 
      else if(m_rad_name=="Gas") mc_rindex = 1.00076; 
      //else mc_rindex = cherenkov_pid.getRefractiveIndex(); // average 
      auto theta_exp = TMath::ACos(
          TMath::Hypot( charged_particle_momentum, charged_particle_mass ) /
          ( mc_rindex * charged_particle_momentum )
          );

      int npe =0;
      for(const auto& [theta,phi] : cherenkov_pid.getThetaPhiPhotons()){
        if(TMath::Abs(theta-theta_exp)<0.01){
          theta_rec += theta; npe++;
	  m_speTheta_dist->Fill(theta*1e3);
	  m_speResid_dist->Fill((theta-theta_exp)*1e3);
	  m_spetheta_vs_p->Fill(charged_particle_momentum,theta*1e3);
	  m_spethetaResid_vs_p->Fill(charged_particle_momentum,(theta-theta_exp)*1e3);
	  m_spetheta_vs_eta->Fill(charged_particle_eta,theta*1e3);
          m_spethetaResid_vs_eta->Fill(charged_particle_eta,(theta-theta_exp)*1e3);
    //mass square expression                                                    // ADDED BY NEBIN
      auto spe_mass_sqr_exp = charged_particle_momentum * charged_particle_momentum *      //ADDED BY NEBIN
                 (mc_rindex * mc_rindex * TMath::Cos(theta) * TMath::Cos(theta) - 1); //ADDED BY NEBIN
      // mass sqaure plot        //ADDED BY NEBIN
    m_spe_mass_square_vs_p_->Fill(charged_particle_momentum,spe_mass_sqr_exp); // ADDED BY NEBIN
        }
      }
      theta_rec /= npe; //cherenkov_pid.getNpe();
      auto mass_sqr_exp = charged_particle_momentum * charged_particle_momentum *      //ADDED BY NEBIN
                 (mc_rindex * mc_rindex * TMath::Cos(theta_rec) * TMath::Cos(theta_rec) - 1); //ADDED BY NEBIN
      // mass square plot   //ADDED BY NEBIN
    m_mass_square_vs_p_->Fill( charged_particle_momentum,mass_sqr_exp); // ADDED BY NEBIN
      auto theta_rec_mrad = theta_rec * 1e3; // [rad] -> [mrad]
      
      auto theta_resid = theta_rec - theta_exp;
      auto theta_resid_mrad = theta_resid * 1e3; // [rad] -> [mrad]

      for(const auto& [theta,phi] : cherenkov_pid.getThetaPhiPhotons()){
        m_photonTheta_vs_photonPhi->Fill( phi, theta*1e3);
        //m_speTheta_dist->Fill(theta*1e3);
        //m_speResid_dist->Fill((theta-theta_exp)*1e3);
        //m_spetheta_vs_p->Fill(charged_particle_momentum,theta*1e3);
        //m_spethetaResid_vs_p->Fill(charged_particle_momentum,(theta-theta_exp)*1e3);
        //m_spetheta_vs_eta->Fill(charged_particle_eta,theta*1e3);
        //m_spethetaResid_vs_eta->Fill(charged_particle_eta,(theta-theta_exp)*1e3);
        m_speTheta_vs_mcWavelength->Fill(Tools::HC/ cherenkov_pid.getPhotonEnergy(),theta*1e3 );
        m_speResid_vs_mcWavelength->Fill(Tools::HC/ cherenkov_pid.getPhotonEnergy(),(theta-theta_exp)*1e3 );
      } // [rad] -> [mrad]

      m_speTheta_dist->Fit("gaus");

      // fill PID histograms
      m_npe_dist->Fill( npe  );//cherenkov_pid.getNpe());
      m_npe_vs_p->Fill(   charged_particle_momentum, cherenkov_pid.getNpe());
      m_npe_vs_eta->Fill( charged_particle_eta,      cherenkov_pid.getNpe());
      m_theta_dist->Fill(   theta_rec_mrad);
      m_theta_vs_p->Fill(   charged_particle_momentum, theta_rec_mrad);
      m_theta_vs_eta->Fill( charged_particle_eta,      theta_rec_mrad);
      m_thetaResid_dist->Fill(   theta_resid_mrad);
      m_thetaResid_vs_p->Fill(   charged_particle_momentum, theta_resid_mrad);
      m_thetaResid_vs_eta->Fill( charged_particle_eta,      theta_resid_mrad);
      /*
      for(const auto& [theta,phi] : cherenkov_pid.getThetaPhiPhotons()){
        m_photonTheta_vs_photonPhi->Fill( phi, theta*1e3);
        m_speTheta_dist->Fill(theta*1e3);
        m_speResid_dist->Fill((theta-theta_exp)*1e3);
        m_spetheta_vs_p->Fill(charged_particle_momentum,theta*1e3);
        m_spethetaResid_vs_p->Fill(charged_particle_momentum,(theta-theta_exp)*1e3);
        m_spetheta_vs_eta->Fill(charged_particle_eta,theta*1e3);
        m_spethetaResid_vs_eta->Fill(charged_particle_eta,(theta-theta_exp)*1e3);
      	m_speTheta_vs_mcWavelength->Fill(Tools::HC/ cherenkov_pid.getPhotonEnergy(),theta*1e3 );
	      m_speResid_vs_mcWavelength->Fill(Tools::HC/ cherenkov_pid.getPhotonEnergy(),(theta-theta_exp)*1e3 );
      } // [rad] -> [mrad]
      */
      
      m_mcWavelength_dist->Fill(  Tools::HC / cherenkov_pid.getPhotonEnergy() ); // energy [GeV] -> wavelength [nm]
      m_mcRindex_dist->Fill( mc_rindex);     
      // find the PDG hypothesis with the highest weight
      float max_weight     = -1000;
      int   pdg_max_weight = 0;
      for(const auto& hyp : cherenkov_pid.getHypotheses()) {
        if(hyp.weight > max_weight) {
          max_weight     = hyp.weight;
          pdg_max_weight = hyp.PDG;
        }
      }
      std::string pdg_max_weight_str = "UNKNOWN";
      if(pdg_max_weight!=0 && !std::isnan(pdg_max_weight))
        pdg_max_weight_str = std::to_string(pdg_max_weight);
      m_log->trace("  Highest weight is {} for PDG {} (string='{}')", max_weight, pdg_max_weight, pdg_max_weight_str);
      m_highestWeight_dist->Fill(   pdg_max_weight_str.c_str(), 1);
      m_highestWeight_vs_p->Fill(   charged_particle_momentum,  pdg_max_weight_str.c_str(), 1);
      m_highestWeight_vs_eta->Fill( charged_particle_eta,       pdg_max_weight_str.c_str(), 1);

    } // end loop over `CherenkovParticleID` objects
  }


  // AlgorithmFinish
  //---------------------------------------------------------------------------
  void CherenkovPIDAnalysis::AlgorithmFinish() {
  }

}

