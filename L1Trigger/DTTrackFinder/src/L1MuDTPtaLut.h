//-------------------------------------------------
//
/**  \class L1MuDTPtaLut
 *
 *   Look-up tables for pt-assignment
 *
 *
 *   $Date: 2006/06/01 00:00:00 $
 *   $Revision: 1.1 $
 *
 *   N. Neumeister            CERN EP
 */
//
//--------------------------------------------------
#ifndef L1MUDT_PTA_LUT_H
#define L1MUDT_PTA_LUT_H

//---------------
// C++ Headers --
//---------------

#include <vector>
#include <map>

//----------------------
// Base Class Headers --
//----------------------


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//              ---------------------
//              -- Class Interface --
//              ---------------------


class L1MuDTPtaLut {

  public:

    /// constructor
    L1MuDTPtaLut();

    /// destructor
    virtual ~L1MuDTPtaLut();

    /// reset pt-assignment look-up tables
    void reset();

    /// load pt-assignment look-up tables
    int load();

    /// print pt-assignment look-up tables
    void print() const;

    /// get pt-value for a given address
    int getPt(int pta_ind, int address) const;

    /// get pt-assignment LUT threshold
    int getPtLutThreshold(int pta_ind) const;

  private:

    /// set precision for look-up tables
    void setPrecision();

  private:

    typedef map<int, int, less<int> > LUT;

    vector<LUT*> pta_lut;
    vector<int>  pta_threshold;

    unsigned short int nbit_phi;
    
};

#endif
