.. 3ptWL-cov documentation master file

3ptWL-cov
=========

**3ptWL-cov** computes the Gaussian contribution to weak-lensing three-point
covariance terms in a harmonic basis on the sphere under the Limber
approximation.  The project provides:

* a compiled C command-line executable, ``wlcov``;
* a static library, ``libwlcov.a``;
* a Cython wrapper, ``wlcovpy``;
* small Python utilities for scripted covariance workflows.

The documentation is organized in the style of scientific Python projects such
as the `emcee documentation`_: start with installation and the quickstart, then
move into the user guide, tutorials, API reference, and troubleshooting pages.
Where possible, the conventions also follow LSST/DESC scientific-software
practice: reproducible commands, explicit environments, lightweight tests, and
developer notes for future maintainers.

Basic Usage
-----------

After installing GSL, install the Python interface from PyPI:

.. prompt:: bash

   python3 -m pip install wlcovpy

No separate ``make`` step is needed for Python use. To build the standalone C
executable from a source checkout:

.. prompt:: bash

   make clean
   make PYTHON=python3 all

Run a compact command-line calculation:

.. prompt:: bash

   ./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_quick \
      ellmax=25 ppp=4 Nr=8 rmin=0.00232711 rmax=0.02 \
      verbose=0 verbose_log=0

Or use the Python wrapper:

.. code-block:: python

   from wlcovpy import wlcov

   model = wlcov(default=False)
   model.set({
       "clsfile": "tests/input/Cls_ep2.txt",
       "rootDir": "Output_python",
       "r": 0.01,
       "theta1": 0.01,
       "theta2": 0.012,
       "thetap1": 0.011,
       "thetap2": 0.013,
       "m": 0,
       "mp": 0,
       "ppp": 4,
       "ellmin": 1.0,
       "ellmax": 25.0,
       "Nr": 8,
       "rmin": 0.00232711,
       "rmax": 0.02,
       "numberThreads": 1,
       "verbose": 0,
       "verbose_log": 0,
       "options": "",
   })
   cputime = model.Run()
   model.clean_all()

How to Use This Guide
---------------------

If you are installing or running the 3ptWL-cov executable for the first time, read
:doc:`install` and :doc:`quickstart`.  If you are preparing production runs,
read the parameter and output pages before scaling up the integration range.
Developers should also read :doc:`development` and :doc:`troubleshooting`,
which summarize the validated build path and current caveats.

.. toctree::
   :maxdepth: 2
   :caption: User Guide

   overview
   install
   quickstart
   user/command-line
   user/configuration
   user/inputs
   user/outputs
   user/python
   performance

.. toctree::
   :maxdepth: 2
   :caption: Tutorials

   tutorials/index

.. toctree::
   :maxdepth: 2
   :caption: Reference

   api
   troubleshooting
   development
   citing
