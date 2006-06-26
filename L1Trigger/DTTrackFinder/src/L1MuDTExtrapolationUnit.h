//-------------------------------------------------
//
/**  \class L1MuDTExtrapolationUnit
 *
 *   Extrapolation Unit:
 *
 *   The Extrapolation Unit attempts to join
 *   track segment pairs of different stations.
 *   it contains 12 Single Extrapolation Units
 *   to perform all extrapolations in its 
 *   own wheel and 6 Single Extrapolation Units
 *   to perform all extrapolations
 *   in the adjacent wheel (next wheel neighbour)
 *
 *
 *   $Date: 2006/06/01 00:00:00 $
 *   $Revision: 1.1 $
 *
 *   N. Neumeister            CERN EP
 */
//
//--------------------------------------------------
#ifndef L1MUDT_EXTRAPOLATION_UNIT_H
#define L1MUDT_EXTRAPOLATION_UNIT_H

//---------------
// C++ Headers --
//---------------

#include <utility>
#include <map>
#include <bitset>

//----------------------
// Base Class Headers --
//----------------------

#include "DataFormats/L1DTTrackFinder/interface/L1AbstractProcessor.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "L1Trigger/DTTrackFinder/src/L1MuDTExtParam.h"
class L1MuDTSectorProcessor;
class L1MuDTSEU;

//              ---------------------
//              -- Class Interface --
//              ---------------------

class L1MuDTExtrapolationUnit : public L1AbstractProcessor {

  public:

    typedef pair<Extrapolation, unsigned int> SEUId;
    typedef map<SEUId, L1MuDTSEU*, less<SEUId> > SEUmap;

    /// constructor
    L1MuDTExtrapolationUnit(const L1MuDTSectorProcessor& );

    /// destructor
    virtual ~L1MuDTExtrapolationUnit();

    /// run Extrapolation Unit
    virtual void run();
    
    /// reset Extrapolation Unit
    virtual void reset();

     /// reset a single extrapolation
    void reset(Extrapolation ext, unsigned int startAdr, unsigned int relAdr );
 
    /// get extrapolation address from a given ERS
    unsigned short int getAddress(Extrapolation ext, unsigned int startAdr, int id) const;
    
    /// get extrapolation quality from a given ERS
    unsigned short int getQuality(Extrapolation ext, unsigned int startAdr, int id) const;

    /// get Extrapolator table for a given SEU
    const bitset<12>& getEXTable(Extrapolation ext, unsigned int startAdr) const;

    /// get Quality Sorter table for a given SEU
    const bitset<12>& getQSTable(Extrapolation ext, unsigned int startAdr) const;
        
    /// return number of successful extrapolations
    int numberOfExt() const;
    
    /// print all successful extrapolations
    void print(int level=0) const;
    
    /// return station of start and target track segment for a given extrapolation
    static pair<int,int> which_ext(Extrapolation ext);

  private:

    const L1MuDTSectorProcessor& m_sp;   // reference to Sector Processor

    mutable SEUmap m_SEUs;               // Single Extrapolation Units
 
};

#endif
