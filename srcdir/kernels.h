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

#ifndef KERNELS
#define KERNELS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <math.h>

// If defined, include the right header

#if OMP
#include <omp.h>
#endif

#if LIKWID
#include <likwid.h>
#endif

#if IACA
#include "iacaMarks.h"
#endif

// Registering a new kernel
/* Modification point (1) */
enum KER {  
	KER_VTRIAD         = 1,
	KER_STREAD    	   = 2,
};

// Registering repeatation times for the kernels based on maximum number of
// elements fit to L1, L2 or L3
/* Modification point (3) - optional */
enum MemLim {  
	L1		= 10000,
	L23		= 1000000
};

// Check if we want to provide array size and repeation from command line
#if EXT == 1
// Everything will be set on run time
#else
	// Set the size of an array 
	#ifndef NSIZE
		#error ArraySize Required (set NSIZE)
	#endif

	// Set repeat the experiment
	#ifndef NITER
		#error Repeat Number Required (set NITR) - type '0' for default
	#endif

	// If you have many kernels in your application, you can choose 
	// the kernel to benchmark
	#ifndef KERNEL
		#error Kernel Number Required (set KERNEL)
	#endif
#endif


// List of kernels. You can add more definitions here.
/* Modification point (2) */
void vTriad(double * restrict a, double * restrict b, double * restrict c, double * restrict d, int n);
void sTriad(double * restrict a, double * restrict b, double * restrict c, int n);

#endif