NIfTIMind C libraries
=====================

Reference implementation of NIfTI Mind extensions (ref: 
http://www.ncbi.nlm.nih.gov/pubmed/20206274). The original version was not in 
the public domain, so this version is based on the nifti reference 
implementation, and in the public domain.

Nifti-1 C libraries
-------------------

Version 2.0.0 beta release Jul  2010
Version 1.1.0 beta release Aug  2008
Version 1.0.0 beta release Dec  2007
Version 0.6 beta release Aug  2007
Version 0.5 beta release May  2007
Version 0.4 beta release Sept. 2006
Version 0.3 beta release April 2006
Version 0.2 beta release August 12, 2005
Version 0.1 beta release March 11, 2005

niftilib code is released into the public domain.


Library directories
-------------------
znzlib   -- low level library for handling read/write of compressed files.

niftilib -- core i/o routines for reading and writing nifti-1 format files.
	    Primarily routines to read/write and manipulate the header field
	    information, including orientation matrices.  Volume-wise,
            timecourse-wise, access to image data.  

nifticdf -- functions to compute cumulative distributions and their inverses

fsliolib -- i/o routines for reading and writing nifti-1 format files, higher
            level than niftilib, includes routines for reading the data blob by
            volume, timecourse, etc., and, addresses image orientation issues.
            *** work in progress, subject to significant revision.....

utils    -- directory containing library utility programs (nifti_tool)




Destination directories
-----------------------
bin      -- destination directory for installed programs
include  -- destination directory for library header files
lib      -- destination directory for compiled libraries
docs     -- destination directory Doxygen html (created via "make doc")



Example directories
-------------------
examples  -- directory containing sample code using nifti reference library
real_easy -- code snippets to read nifti-1 files, not using nifti ref. lib.


Other directories
-----------------
Testing   -- directory containing code to test the libraries
packaging -- spec file for building RPMs, and template package
             description for Dev-Cpp (http://www.bloodshed.net/devcpp.html)



Instructions to build
---------------------

command  -- "make all" 

            results will be left in the directories: bin/ include/ lib/

command  -- "make help"

	    will show more build options


For more information
--------------------

See the NIfTIMind webpage at https://github.com/yiuin/NIfTIMind 
See the niftilib webpage at http://niftilib.sourceforge.net/
See the NIFTI webpage at http://nifti.nimh.nih.gov/

