Author: Ziv Yaniv (zivy@isis.georgetown.edu)

This is an updated version of the original RANSAC code posted on my
web site. Originally implemented circa 2000. Project implementation
included a Makefile for unix systems and a VC++ 6.0 project for
windows systems.

Currently, 2010, I manage multi-platform code using the CMake
(http://www.cmake.org) configuration tool. There were also changes to the file
names and code. The code is now published under a BSD license (see
license.txt).

This archive contains an implementation of the Random Sample Consensus (RANSAC)
framework for robust parameter estimation. The code comes in two flavors, a
static function call approach (RANSAC.h) and a multi-threaded approach
(RANSACMT.h). The former only requires that the user invoke a static function
and pass it the relevant arguments while the latter requires object
instantiation and optionally setting the number of threads to use.

The archive includes the following files:
1. Cmakelists.txt - input file for the Cmake program which will generate platform
   specific configuration files (Makefile on linux/unix, solution file for
   visual studio on windows...).
2. Source files:
   Point2D.{cxx,h} - Primitive 2D point class used in the example.
   ParameterEstimator.h - A virtual class, the ancestor of all parameter
                          estimators.
   LineParamEstimator.{cxx,h} - Class which implements the virtual functions
                                defined by the ParameterEstimator class, for 2D
                                lines.
   RANSAC.{h,txx} - Template implementation of the RANSAC algorithm using a static
                    function approach.
   RANSACMT.{h,txx} - Template implementation of the RANSAC algorithm using a
                      multi threaded approach.
   main.cxx - An example which shows how to use the RANSAC static function
              implementation.
   mainMT.cxx - An example which shows how to use the multi threaded RANSAC
                implementation. By default the number of threads is equivalent
                to the number of cores on the machine. To manually set this value
                uncomment lines 89-90 and set the appropriate value.

   multiThreading directory - A platform/OS independent library for
                              multi-threading. Implementation appropriated with
                              minor modifications from the Insight Registration
                              and Segmentation toolkit (ITK - www.itk.org).

Notes:
1. The math underlying the code which performs a least squares fit for the line
   is described in the summary on Principle Component Analysis (PCA) found on my web
   site (look under lesson handouts). Read the orthogonal least squares section and the
   section about eigenvectors/eigenvalues of symmetric 2X2 matrices.
2. For a detailed description of the RANSAC algorithm see:
   * Martin A. Fischler, Robert C. Bolles, ``Random Sample Consensus:
     A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography'',
     Communications of the ACM, Vol. 24(6), 1981.
   **Richard I. Hartely, Andrew Zisserman, "Multiple View Geometry in Computer Vision",
     Cambridge University Press, 2000.
3. This template implementation has been successfully used to estimate the parameters of:
   a. lines
   b. circles
   c. ellipses
   d. homographies

   This archive is intended to be self contained. As a consequence it only
   includes the 2D line parameter estimator as it is the only estimator that
   does not require the availability of linear algebra packages or non-linear
   optimization code.
