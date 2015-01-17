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
#include "timing.h"

int main (int argc, char** argv){

    // Init likwid
    #if LIKWID
        likwid_markerInit(); 
        #if OMP
            #pragma omp parallel
            likwid_markerThreadInit();
        #endif
    #endif

    // Timers
    double wct_start,wct_end,cput_start,cput_end, dummy;

    int i,        // Arrays counter
    j,            // Index for repeating the experiment  
    z,            // Outer loop blocking 
    eSize,        // Evaluated size (command line - Preprocessor)
    eIter,        // Evaluated Repeation (command line - Preprocessor)
    eKernel,      // Evaluated Kernel (command line - Preprocessor)
    eNarr,        // Evaluated eNarr (command line - Preprocessor)
    reminder;     // Used for adding extra elements in the arrays So that we always start from a 32 aligned element  


    // Evaluate Size and repeatation (command line - Preprocessor)
    #if EXT == 1 // If command line
        if (argc < 2 || argc > 4) // Wrong number of arguments
        {
            printf("Please enter Size - Repeation(set to default by typing '0') - Kernel.\n");
            return 0;
        }
        else // Set arrays size - repeation - kernel to benchmark
        {
            eSize = atoi(argv[1]);
            eIter  = atoi(argv[2]);
            eKernel = atoi(argv[3]);
        }
    #else // Set with preprocessors
        eSize   = NSIZE;
        eIter   = NITER;
        eKernel = KERNEL;
    #endif

    if (eIter == 0) // case user asked for default
    {
        if (eSize < L1) // L1
            eIter = 1000000;
        else if (eSize < L23)  
            eIter = 10000;
        else             // Main Mem
            eIter = 10;
    }

    // Print info about the kernel in benchmark
    /* Modification point (1): Add another 'case' for your krenel
                        set the number of arrays inside your kernel */
    switch (eKernel) 
    {
        case KER_VTRIAD:
            printf("Kernel:                 vTriad\n");
            printf("# arrays alloc:         4\n");
            eNarr = 4;
        break;

        case KER_STRIAD:
            printf("Kernel:                 sTriad\n");
            printf("# arrays alloc:         3\n");
            eNarr = 3;
        break;
    }

    // Check if the arrays' size is not a multible of 64. This is important for alligned loads
    if( (eSize%64) != 0)
        reminder = (eSize%64);
    else
        reminder = 0;

	// Allocate one big chunk of memory
    double * __restrict__ __attribute__((aligned(64))) oneBigArr;
    posix_memalign((void**)&oneBigArr,64, (eSize + reminder) * eNarr * sizeof(double) );
    
    // Allocate array of arrays equivelent to eNarr
    double ** __restrict__ __attribute__((aligned(64))) arrOfArrs = (double **)malloc( eNarr * sizeof(double *) );
    
	// Generate random numbers between 0-100
    #if RANDSEED == 1
    srand(time(NULL));
    printf("Random seed\n");
    #else
    srand(1);
    printf("Constant seed\n");
    #endif

    printf("Begin Intialization\n");
    for (i=0; i<eSize*eNarr;++i){
        oneBigArr[i] = (double)(rand())/RAND_MAX;
    }
    printf("End Intialization\n");

    
    // Reassign the one big array to smaller alligned arrays
    for (i=0; i<eNarr;++i){
        arrOfArrs[i] = &(oneBigArr[i * (eSize + reminder)]);
    }

    // Print info about the App
    // Print total mem allocated
    printf("Size of Mem Alloc:      %i Bytes\n", (eSize + reminder) * eNarr * sizeof(double));
    // Experiment repeated
    printf("Experiment repeated:    %i times\n", eIter);
    #if LIKWID
    printf("LIKWID API is enabled\n");
        #if LIKEXT
        printf("LIKWID is measing the external loop\n");
        #else
        printf("LIKWID is measing the internal loop\n");
        #endif
    #else
    printf("LIKWID API is disabled\n");
    #endif
    #if OMP
    printf("OMP API is enabled\n");
    #pragma omp parallel
    {
    #pragma omp single
    printf("Number of threads:      %u\n", omp_get_num_threads());
    }
    #else
    printf("OMP API is disabled\n");
    #endif
    #if IACA
    printf("IACA API is enabled\n");
    #else
    printf("IACA API is disabled\n");
    #endif



    // Kernels Benchmarking Starts Here  //
    printf("Begin kernel execution\n");

    // Start timers
    timing(&wct_start, &cput_start);

    switch(eKernel)
    {
        case KER_VTRIAD:
            #if LIKWID
            #if LIKEXT
            /* Modification the name to your kernel name */
            likwid_markerStartRegion("vTriad_OUT");
            #endif
            #endif
            // Repeat the execution of kernel
            for(j=0; j<eIter; ++j){
                /* Modification the arguments to your kernel */
                vTriad(arrOfArrs[0], arrOfArrs[1], arrOfArrs[2], arrOfArrs[3], eSize);
            }
            #if LIKWID
            #if LIKEXT
            /* Modification the name to your kernel name */
            likwid_markerStopRegion("vTriad_OUT"); 
            #endif
            #endif   
        break;

        case KER_STRIAD:
            #if LIKWID
            #if LIKEXT
            /* Modification the name to your kernel name */
            likwid_markerStartRegion("sTriad_OUT");
            #endif
            #endif
            // Repeat the execution of kernel
            for(j=0; j<eIter; ++j){
                /* Modification the arguments to your kernel */
                sTriad(arrOfArrs[0], arrOfArrs[1], arrOfArrs[2], eSize);
            }
            #if LIKWID
            #if LIKEXT
            /* Modification the name to your kernel name */
            likwid_markerStopRegion("vTriad_OUT"); 
            #endif
            #endif   
        break;
    }

    // Stop timers
    timing(&wct_end, &cput_end);

    // Kernels Benchmarking Ends Here  //
    printf("End kernel execution\n");



    // Print total time of App and Cy/CL
    printf("Elapsed total time:     %.16lf seconds\n", (wct_end-wct_start));
    printf("Time per 1 experiment:  %.16lf seconds\n", (wct_end-wct_start)/(double)eIter);
    printf("Cycles/CL:              %.16lf\n", 8.*2.7*(wct_end-wct_start)*(1000000000./(double)eIter/(double)eSize)  );



    // If printfile, put results to a file
    #if PRTF == 1
    printf("Results printed to a file\n");
    FILE * fp;
    fp = fopen("results.txt", "w");
    for (i=0; i<eSize*eNarr; ++i){
        fprintf(fp, "%.16lf\n",oneBigArr[i]);
    }
    fclose(fp);
    #endif

    // Free memory
    free(oneBigArr);
    free(arrOfArrs);

    // Close likwid markers
    #if LIKWID
        likwid_markerClose();   
    #endif

    return 0;
}
