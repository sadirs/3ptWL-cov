Minimal Command-Line Workflow
=============================

This tutorial runs the compiled executable on the included test
``C_ell`` table.

1. Build ``wlcov`` from the repository root:

   .. prompt:: bash

      make clean
      make PYTHON=python3 all

2. Run a compact calculation:

   .. prompt:: bash

      ./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_tutorial \
         r=0.01 theta1=0.01 theta2=0.012 thetap1=0.011 thetap2=0.013 \
         m=0 mp=0 ellmin=1 ellmax=25 ppp=4 Nr=8 rmin=0.00232711 rmax=0.02 \
         verbose=0 verbose_log=0

3. Confirm output provenance:

   .. prompt:: bash

      test -f Output_tutorial/parameters_null-wlcov-usedvalues

4. Re-run the same calculation through a parameter file:

   .. prompt:: bash

      ./wlcov paramfile=docs/examples/minimal.params

The parameter-file output appears under ``Output_paramfile`` as configured in
``docs/examples/minimal.params``.
