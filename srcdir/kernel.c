// Copyright 2014,2015 Salaheldin Saleh. me@s-saleh.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "kernels.h"
#include "util.h"

void vTriad(double * restrict a, double * restrict b, double * restrict c, double * restrict d, int n){
    
    int i;

    #if OMP
    #pragma omp parallel
    {
    #endif

    #if LIKWID
    likwid_markerStartRegion("vTriad_IN");
    #endif

    #if IACA 
    IACA_START
    #endif

    #if OMP
    #pragma omp for
    #endif

    #pragma vector aligned
    #pragma simd
    //#pragma unroll(8)
    for (i=0; i<n;++i){

        a[i] = b[i] + c[i] * d[i];

    }
    #if IACA 
    IACA_END
    #endif

    #if LIKWID
    likwid_markerStopRegion("vTriad_IN"); 
    #endif

    #if OMP
        }
    #endif
}

