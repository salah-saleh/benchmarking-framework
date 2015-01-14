# Copyright 2014,2015 Salaheldin Saleh. me@s-saleh.com

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# iaca -64 -arch SNB -o .txt -analysis THROUGHPUT .o
#likwid - omp - Mem - Kernel - 

# Set Preprocessors
#(LIKWID 0: off - 1: on)
#(LIKEXT markers on the repeation loop 0: no - 1: yes)
LIKWID = 0
LIKEXT = 0
#(IACA 0: off - 1: on)
IACA   = 0
#(OMP 0: off - 1: on)
OMP    = 0
#(RANDSEED 0: off - 1: on)
RANDSEED=0
# Print results to a file (0: off - 1: on)
PRTF	=0
#(1: use command line - 2: use preprocessors)
EXT	   = 1 	
# Arrays memory size	
NSIZE  = 0
# Repeat kernels (0: use defaults which are defined inside the program)
NITER  = 0
# Select Kernel (1)
KERNEL = 22

# Set Compiler
CC=icc

# Preprocessors as flags
DEFS = -DNSIZE=$(NSIZE) -DNITER=$(NITER) -DKERNEL=$(KERNEL) -DIACA=$(IACA) -DOMP=$(OMP) -DLIKWID=$(LIKWID) -DEXT=$(EXT) \
-DLIKEXT=$(LIKEXT) -DRANDSEED=$(RANDSEED) -DPRTF=$(PRTF)
# Compilation flags
CMPF = -restrict -fno-alias -O3 -xAVX -opt-streaming-stores never -openmp -nolib-inline -I/apps/likwid/3.1.1/include 
# Inline functions in te same file
INLF = #-finline-functions -finline-limit=1000 
# Vectorization Report
VECF = -vec-report3
LNKF = -lrt -lm
ASMF = -finline-functions -finline-limit=1000  -fsource-asm
# Make sure to load likwid module
LIKL=-L/usr/local/lib -L/apps/likwid/3.1.1/lib -llikwid 

# Object files dir
objdir=objdir
# Assembly files dir
asmdir=asmdir
# Source files dir
srcdir=srcdir


SOURCES := $(wildcard $(srcdir)/*.c)
OBJ     := $(patsubst $(srcdir)/%.c,$(objdir)/%.o, $(SOURCES))
ASM     := $(patsubst $(srcdir)/%.c,$(asmdir)/%.s, $(SOURCES))

all: dir output
asm: dir assemb

dir: $(objdir) $(asmdir) $(srcdir)

$(objdir):
	@mkdir -p $@
$(asmdir):
	@mkdir -p $@
$(srcdir):
	@mkdir -p $@

output: $(OBJ)
	$(CC) $(CMPF) $^ $(LNKF) $(LIKL) -o $@

$(objdir)/%.o: $(srcdir)/%.c
	$(CC) $(CMPF) $(INLF) -c $^ $(DEFS) $(VECF) -o $@

assemb: $(ASM)

$(asmdir)/%.s: $(srcdir)/%.c
	$(CC) $(CMPF) $(ASMF) -S $^ $(DEFS) $(VECF) -o $@


.PHONY: clean cleanasm cleanall all asm
clean:
	rm -rf $(objdir)/*.o output

cleanasm:
	rm -rf $(asmdir)/*.s

cleanall:
	 rm -rf $(asmdir)/*.s $(objdir)/*.o output
