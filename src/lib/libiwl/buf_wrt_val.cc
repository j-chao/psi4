/*
 *@BEGIN LICENSE
 *
 * PSI4: an ab initio quantum chemistry software package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *@END LICENSE
 */

/*!
  \file
  \ingroup IWL
*/
#include <cstdio>
#include <cmath>
#include <libciomr/libciomr.h>
#include "iwl.h"
#include "iwl.hpp"
#include "libparallel/ParallelPrinter.h"
namespace psi {
  
void IWL::write_value(int p, int q, int r, int s, double value, int printflag,
    std::string out, int dirac)
{
   boost::shared_ptr<psi::PsiOutStream> printer=(out=="outfile"?outfile:
         boost::shared_ptr<OutFile>(new OutFile(out)));

    int idx;
    Label *lblptr;
    Value *valptr;

    lblptr = labels_;
    valptr = values_;

    if (fabs(value) > cutoff_) {
        idx = 4 * idx_;
        if(dirac) {
            lblptr[idx++] = (Label) p;
            lblptr[idx++] = (Label) r;
            lblptr[idx++] = (Label) q;
            lblptr[idx++] = (Label) s;
        }
        else {
            lblptr[idx++] = (Label) p;
            lblptr[idx++] = (Label) q;
            lblptr[idx++] = (Label) r;
            lblptr[idx++] = (Label) s;
        }
        valptr[idx_] = (Value) value;

        idx_++;

        if (idx_ == ints_per_buf_) {
            lastbuf_ = 0;
            inbuf_ = idx_;
            put();
            idx_ = 0;
        }

        if (printflag) {
            if(dirac) {
                printer->Printf( ">%d %d %d %d = %20.10f\n",
                    p, r, q, s, value);
            }
            else {
                printer->Printf( ">%d %d %d %d = %20.10f\n",
                    p, q, r, s, value);
            }
        }
    }
}

/*!
** iwl_buf_wrt_val()
**
** Write to an Integrals With Labels formatted buffer.
** The buffer must have been initialized with iwl_buf_init().  Don't
** forget to call iwl_buf_flush() when finished with all writes to the
** buffer to ensure that all contents are written to disk.
**
** This function writes only a particular value and its indices to the
** given iwl buffer.  This is useful when index rearragements are
** necessary (e.g. conversion from Mulliken to Dirac notation).  This
** is not as nice as being able to write entire arrays of values to the
** buffer, but may be necessary at times.
** Daniel Crawford, Novemeber 1995
** \ingroup IWL
*/
void iwl_buf_wrt_val(struct iwlbuf *Buf, int p, int q, int r, int s,
                     double value, int printflag, std::string out, int dirac)
{
   boost::shared_ptr<psi::PsiOutStream> printer=(out=="outfile"?outfile:
         boost::shared_ptr<OutFile>(new OutFile(out)));
  int idx;
  Label *lblptr;
  Value *valptr;

  lblptr = Buf->labels;
  valptr = Buf->values;

  if (fabs(value) > Buf->cutoff) {
    idx = 4 * Buf->idx;
    if(dirac) {
      lblptr[idx++] = (Label) p;
      lblptr[idx++] = (Label) r;
      lblptr[idx++] = (Label) q;
      lblptr[idx++] = (Label) s;
    }
    else {
      lblptr[idx++] = (Label) p;
      lblptr[idx++] = (Label) q;
      lblptr[idx++] = (Label) r;
      lblptr[idx++] = (Label) s;
    }
    valptr[Buf->idx] = (Value) value;

    Buf->idx++;
     
    if (Buf->idx == Buf->ints_per_buf) {
      Buf->lastbuf = 0;
      Buf->inbuf = Buf->idx;
      iwl_buf_put(Buf);
      Buf->idx = 0;
    }
     
    if (printflag) {
      if(dirac) {
	printer->Printf( ">%d %d %d %d = %20.10f\n",
		p, r, q, s, value);
      }
      else {
	printer->Printf( ">%d %d %d %d = %20.10f\n",
		p, q, r, s, value);
      }
    }
  } 
}

}

