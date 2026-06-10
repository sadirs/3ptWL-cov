Quickstart
==========

This page gives a short, tested workflow for running ``wlcov`` from a source
checkout.  It uses the small test power-spectrum file included in the
repository.

Compile
-------

.. prompt:: bash

   python3 -m pip install --user numpy Cython scipy
   make clean
   make PYTHON=python3 all

Run the CLI
-----------

.. prompt:: bash

   ./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_quick \
      r=0.01 theta1=0.01 theta2=0.012 thetap1=0.011 thetap2=0.013 \
      m=0 mp=0 ellmin=1 ellmax=25 ppp=4 Nr=8 rmin=0.00232711 rmax=0.02 \
      verbose=0 verbose_log=0

The executable prints the intermediate integrals, the covariance integrand, the
radial integral, and timing information to standard output.  It also writes a
parameter provenance file:

.. prompt:: bash

   ls Output_quick
   cat Output_quick/parameters_null-wlcov-usedvalues

Run With a Parameter File
-------------------------

For reproducible runs, store parameters in a text file:

.. literalinclude:: examples/minimal.params
   :language: text

Then run:

.. prompt:: bash

   ./wlcov paramfile=docs/examples/minimal.params

Run the Python Wrapper
----------------------

.. prompt:: bash

   python3 docs/examples/python_wrapper.py

The wrapper runs the same C core and returns the measured ``MainLoop`` CPU time
to Python.  Numerical values are currently emitted by the C layer to standard
output.

Next Steps
----------

* :doc:`user/inputs` explains the two-column ``C_ell`` input table.
* :doc:`user/command-line` lists all runtime parameters.
* :doc:`performance` explains ``ppp``, ``ellmin``, ``ellmax``, and OpenMP.
* :doc:`tutorials/index` contains longer workflows.
