Quickstart
==========

This page gives short, tested paths for using the published Python package or
running ``wlcov`` from a source checkout.

Run Online in Google Colab
--------------------------

The quickest route needs no clone and no local compiler setup:

.. image:: https://colab.research.google.com/assets/colab-badge.svg
   :target: https://colab.research.google.com/github/sadirs/3ptWL-cov/blob/main/docs/examples/wlcovpy_covariance_colab.ipynb
   :alt: Open the covariance-matrix tutorial in Google Colab

The notebook installs the system GSL dependency and the published
``wlcovpy==1.0.1`` package, downloads inputs from the ``v1.0.1`` release, and
checks that the compact covariance result is finite, symmetric, and ``6 x 6``.

Install the Python Package Locally
----------------------------------

On Debian or Ubuntu:

.. prompt:: bash

   sudo apt-get update
   sudo apt-get install build-essential libgsl-dev python3-dev
   python3 -m pip install "wlcovpy==1.0.1"

The Python package compiles its native extension during installation; a
separate source checkout or ``make`` command is not required.

Build From a Source Checkout
----------------------------

The remaining commands use the small test power-spectrum file included in the
repository and also build the optional standalone executable.

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
* :doc:`tutorials/covariance-matrix` documents the Colab and source-checkout
  covariance workflows in detail.
* :doc:`tutorials/index` contains the other longer workflows.
