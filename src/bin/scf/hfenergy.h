/*
 *  hfenergy.h
 *  matrix
 *
 *  Created by Justin Turney on 4/9/08.
 *  Copyright 2008 by Justin M. Turney, Ph.D.. All rights reserved.
 *
 */

#ifndef HFENERGY_H
#define HFENERGY_H

#include <libpsio/psio.hpp>
#include <libmints/wavefunction.h>
#include <psi4-dec.h>

namespace psi { namespace scf {
 
class HFEnergy : public Wavefunction {
public:
    HFEnergy(Options & options, shared_ptr<PSIO> psio, shared_ptr<Chkpt> chkpt);
    virtual ~HFEnergy() {}
    
    double compute_energy();
};

}}

#endif
