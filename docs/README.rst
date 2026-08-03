3ptWL-cov: Weak-Lensing Covariance Computation
==============================================

Project Team
------------

:Authors: Sofia Samario-Nava and Alejandro Aviles
:Contributors: Sadi Ramirez Solano (sadi@icf.unam.mx) and
   Mario A. Rodriguez-Meza
:Repository: `Source code and issue tracking
   <https://github.com/sadirs/3ptWL-cov>`_

We acknowledge financial support from grants DGAPA-PAPIIT IA101825 and
SECIHITI CBF2023-2024-162.

Related Projects
----------------

* `3ptWL-mod <https://github.com/sadirs/3ptWL-mod>`_: models weak-lensing
  three-point correlation functions using perturbative and halo-model methods.
* `cTreeBalls <https://github.com/rodriguezmeza/cTreeBalls>`_: measures two- and
  three-point correlation functions from point catalogs and scalar fields.

Introduction
------------

3ptWL-cov is a C code for computing the Gaussian contribution to
weak-lensing three-point covariance terms in a harmonic basis on the sphere
within the Limber approximation.  The repository contains a command-line
executable, a static C library, a Cython wrapper, and small Python examples.

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

Documentation Builds
--------------------

From the repository root, install documentation dependencies and build the
HTML, man, and PDF outputs::

    python3 -m pip install --user -r docs/requirements.txt
    cd docs

    make html
    make man
    make latexpdf

The generated artifacts are written to:

* ``docs/_build/html/index.html``
* ``docs/_build/man/wlcov.1``
* ``docs/_build/latex/3ptWL-cov.pdf``

The legacy hand-written man page is also kept in ``docs/man`` and can be
converted to HTML with::

    cd docs/man
    ./makehtml.sh

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
