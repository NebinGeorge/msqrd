// Copyright 2023, Christopher Dilks
// Subject to the terms in the LICENSE file found in the top-level directory.

#pragma once

#include <spdlog/spdlog.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TRegexp.h>

#include <edm4eic/CherenkovParticleIDCollection.h>
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/utils/vector_utils.h>
#include <edm4hep/utils/kinematics.h>

#include "Tools.h"
#include "ChargedParticle.h"

namespace benchmarks {

  class CherenkovPIDAnalysis {

    public:
      CherenkovPIDAnalysis() = default;
      ~CherenkovPIDAnalysis() {}

      // algorithm methods
      void AlgorithmInit(
          std::string                      rad_name,
          std::shared_ptr<spdlog::logger>& logger
          );
      void AlgorithmProcess(
          const edm4hep::MCParticleCollection&          mc_parts,
          const edm4eic::CherenkovParticleIDCollection& cherenkov_pids
          );
      void AlgorithmFinish();

    private:

      // radiator name
      std::string m_rad_name;
      TString     m_rad_name_trun;
      TString     m_rad_title;

      // histograms
      // - distributions
      TH1D *m_npe_dist;
      TH1D *m_theta_dist;
      TH1D *m_thetaResid_dist;
      TH1D *m_mcWavelength_dist;
      TH1D *m_mcRindex_dist;
      TH1D *m_highestWeight_dist;
      TH1D *m_speTheta_dist;
      TH1D *m_speResid_dist;
      TH2D *m_photonTheta_vs_photonPhi;
      TH2D *m_speTheta_vs_mcWavelength;
      TH2D *m_speResid_vs_mcWavelength;
      // - momentum scans
      TH2D *m_npe_vs_p;
      TH2D *m_theta_vs_p;
      TH2D *m_spetheta_vs_p;
      TH2D *m_thetaResid_vs_p;
      TH2D *m_spethetaResid_vs_p;
      TH2D *m_highestWeight_vs_p;
      // - pseudorapidity scans
      TH2D *m_npe_vs_eta;
      TH2D *m_theta_vs_eta;
      TH2D *m_spetheta_vs_eta;
      TH2D *m_thetaResid_vs_eta;
      TH2D *m_spethetaResid_vs_eta;
      TH2D *m_highestWeight_vs_eta;
      // mass square plots 
      TH2D *m_mass_square_vs_p_;// ADDED BY NEBIN
      TH2D *m_spe_mass_square_vs_p_;// ADDED BY NEBIN
      // logger
      std::shared_ptr<spdlog::logger> m_log;

  };

}


