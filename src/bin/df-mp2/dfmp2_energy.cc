/*
 *
 *
 */

#include <cstdio>
#include <cstdlib>
#include <string>

#include <libipv1/ip_lib.h>

#include "dfmp2_energy.h"
#include "dfmp2.h"

using namespace std;
using namespace psi;

namespace psi { namespace dfmp2 {
     
DFMP2Energy::DFMP2Energy(Options & options, shared_ptr<PSIO> psio, shared_ptr<Chkpt> chkpt) 
    : Wavefunction(options, psio, chkpt)
{
    
}

double DFMP2Energy::compute_E()
{
    double energy;
    
    psi::dfmp2::DFMP2 dfmp2_energy(options_, psio_, chkpt_);
    energy = dfmp2_energy.compute_energy();
    
    return energy;
}

}}
