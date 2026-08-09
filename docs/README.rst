3ptWL-cov: Weak-Lensing Covariance Computation
==============================================

3ptWL-cov is a C code for computing the Gaussian contribution to
weak-lensing three-point covariance terms in a harmonic basis on the sphere
within the Limber approximation.  The repository contains a command-line
executable, a static C library, a Cython wrapper, and small Python examples.

Authors
-------

* **Sofia Samario-Nava** (ICF-UNAM, Mexico), ssamario@icf.unam.mx
* **Alejandro Aviles** (ICF-UNAM, Mexico), aviles@icf.unam.mx

**Other people who contributed to this code:**

* **Sadi Ramirez Solano** (ICF-UNAM, Mexico) - `sadi@icf.unam.mx <mailto:sadi@icf.unam.mx>`_
* **Mario A. Rodriguez-Meza** (ININ, Mexico)

Related Projects
----------------

* `3ptWL-mod <https://github.com/sadirs/3ptWL-mod>`_: models weak-lensing
  three-point correlation functions using perturbative and halo-model methods.
* `cTreeBalls <https://github.com/rodriguezmeza/cTreeBalls>`_: measures two- and
  three-point correlation functions from point catalogs and scalar fields.

Installing and Getting Started
------------------------------

After installing GSL, install the Python interface from PyPI::

    python3 -m pip install wlcovpy

This builds and installs ``wlcovpy`` without a separate ``make`` command.

The optional source build creates the executable ``wlcov``, the static library
``libwlcov.a``, and the Python extension ``wlcovpy``::

    make clean
    make PYTHON=python3 all

To check that the code runs, type from the repository root::

    ./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_quick \
       ellmax=25 ppp=4 Nr=8 rmin=0.00232711 rmax=0.02 \
       verbose=0 verbose_log=0

The run writes a used-values parameter file under ``Output_quick`` and prints
the numerical integration results to standard output.

Configuration
-------------

Build-time switches are controlled by ``Makefile_settings`` and
``Makefile_machine``.  The most important settings are ``USEGSL``,
``OPENMPMACHINE``, and ``ADDONSON``.  Runtime parameters can be supplied as
``name=value`` command-line tokens or in a parameter file.

Python
------

The Python wrapper is imported as::

    from wlcovpy import wlcov

A compact wrapper example is available in ``docs/examples/python_wrapper.py``.

License
-------

3ptWL-cov is distributed under the MIT license.  If you use this program in
research that results in publications, please cite Sofia Samario et al.,
arXiv:2506.19811, and record the code version, compiler, GSL version, and
runtime configuration used for the analysis.

Acknowledgements
----------------

We acknowledge financial support from grants DGAPA-PAPIIT IA101825 and
SECIHITI CBF2023-2024-162.
