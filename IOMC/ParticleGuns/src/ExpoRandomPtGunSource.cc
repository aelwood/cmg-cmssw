/*
 *  $Date: 2009/05/25 13:00:58 $
 *  $Revision: 1.4 $
 *  \author Jean-Roch Vlimant
 */

#include <ostream>

#include "IOMC/ParticleGuns/interface/ExpoRandomPtGunSource.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// #include "FWCore/Utilities/interface/Exception.h"

// #include "CLHEP/Random/RandExpo.h"

using namespace edm;
using namespace std;

ExpoRandomPtGunSource::ExpoRandomPtGunSource(const ParameterSet& pset,
                                             const InputSourceDescription& desc) : 
   BaseFlatGunSource(pset, desc)
{


   ParameterSet defpset ;
   ParameterSet pgun_params = 
      pset.getUntrackedParameter<ParameterSet>("PGunParameters",defpset) ;
  
   fMinPt = pgun_params.getUntrackedParameter<double>("MinPt",0.99);
   fMaxPt = pgun_params.getUntrackedParameter<double>("MaxPt",1.01);

   fMeanPt = pgun_params.getUntrackedParameter<double>("MeanPt",-1.);

   produces<HepMCProduct>();

   //the explonential generator
   fRandomExpoGenerator = new CLHEP::RandExponential(fRandomEngine,fMeanPt);
   
}

ExpoRandomPtGunSource::~ExpoRandomPtGunSource()
{
   // no need to cleanup GenEvent memory - done in HepMCProduct
}

bool ExpoRandomPtGunSource::produce(Event &e) 
{

   if ( fVerbosity > 0 )
   {
      cout << " ExpoRandomPtGunSource : Begin New Event Generation" << endl ; 
   }
   // event loop (well, another step in it...)
          
   // no need to clean up GenEvent memory - done in HepMCProduct
   // 
   
   // here re-create fEvt (memory)
   //
   fEvt = new HepMC::GenEvent() ;
   
   // now actualy, cook up the event from PDGTable and gun parameters
   //
   // 1st, primary vertex
   //
   //HepMC::GenVertex* Vtx = new HepMC::GenVertex(CLHEP::HepLorentzVector(0.,0.,0.));
   HepMC::GenVertex* Vtx = new HepMC::GenVertex(HepMC::FourVector(0.,0.,0.));

   // loop over particles
   //
   int barcode = 1 ;
   for (unsigned int ip=0; ip<fPartIDs.size(); ++ip)
   {

     //the max is to ensure you don't generate at 0
     //the 90% is to get rid of edge effect
     
     double pt     =  std::max(0.00001,0.90*fMinPt)+fRandomExpoGenerator->fire(fMeanPt);
     //shoot until in the designated range
     while (pt<fMinPt || pt>fMaxPt)
       {pt = std::max(0.00001,0.90*fMinPt) + fRandomExpoGenerator->fire(fMeanPt);}
     
     double eta    = fRandomGenerator->fire(fMinEta, fMaxEta) ;
       double phi    = fRandomGenerator->fire(fMinPhi, fMaxPhi) ;
       int PartID = fPartIDs[ip] ;
       const HepPDT::ParticleData* 
          PData = fPDGTable->particle(HepPDT::ParticleID(abs(PartID))) ;
       double mass   = PData->mass().value() ;
       double theta  = 2.*atan(exp(-eta)) ;
       double mom    = pt/sin(theta) ;
       double px     = pt*cos(phi) ;
       double py     = pt*sin(phi) ;
       double pz     = mom*cos(theta) ;
       double energy2= mom*mom + mass*mass ;
       double energy = sqrt(energy2) ; 
       //CLHEP::Hep3Vector p(px,py,pz) ;
       //HepMC::GenParticle* Part = 
       //    new HepMC::GenParticle(CLHEP::HepLorentzVector(p,energy),PartID,1);
       HepMC::FourVector p(px,py,pz,energy) ;
       HepMC::GenParticle* Part = 
           new HepMC::GenParticle(p,PartID,1);
       Part->suggest_barcode( barcode ) ;
       barcode++ ;
       Vtx->add_particle_out(Part);

       if ( fAddAntiParticle )
       {
          //CLHEP::Hep3Vector ap(-px,-py,-pz) ;
          HepMC::FourVector ap(-px,-py,-pz,energy) ;
	  int APartID = -PartID ;
	  if ( PartID == 22 || PartID == 23 )
	  {
	     APartID = PartID ;
	  }	  
	  //HepMC::GenParticle* APart =
	  //   new HepMC::GenParticle(CLHEP::HepLorentzVector(ap,energy),APartID,1);
	  HepMC::GenParticle* APart =
	     new HepMC::GenParticle(ap,APartID,1);
	  APart->suggest_barcode( barcode ) ;
	  barcode++ ;
	  Vtx->add_particle_out(APart) ;
       }

   }

   fEvt->add_vertex(Vtx) ;
   fEvt->set_event_number(event()) ;
   fEvt->set_signal_process_id(20) ; 
        
   if ( fVerbosity > 0 )
   {
      fEvt->print() ;  
   }

   auto_ptr<HepMCProduct> BProduct(new HepMCProduct()) ;
   BProduct->addHepMCData( fEvt );
   e.put(BProduct);
    
   if ( fVerbosity > 0 )
   {
      // for testing purpose only
      // fEvt->print() ; // prints empty info after it's made into edm::Event
      cout << " FlatRandomPtGunSource : Event Generation Done " << endl;
   }

   return true;
}

