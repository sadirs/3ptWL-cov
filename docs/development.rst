Developer Guide
===============

This project is small enough to keep the development workflow lightweight, but
scientific reproducibility still benefits from consistent conventions.  The
recommendations below follow the spirit of the LSST DESC coding guidelines and
the `LSST DM Developer Guide`_: document user-facing behavior, test examples,
keep changes modular, and record enough environment information to reproduce
results.

Repository Layout
-----------------

.. list-table::
   :header-rows: 1
   :widths: 28 72

   * - Path
     - Purpose
   * - ``main/``
     - C executable entry point.
   * - ``source/``
     - Core initialization, I/O, and numerical integration routines.
   * - ``include/``
     - Public C headers and runtime parameter structures.
   * - ``addons/``
     - CLASS-style parser, optional cosmology support, addon include hooks, and
       Cython PXD helpers.
   * - ``general_lib/``
     - Shared allocation, logging, timing, filesystem, and string-safety
       helpers.
   * - ``getparam/``
     - Command-line ``name=value`` parameter parser.
   * - ``python/``
     - Cython wrapper source.
   * - ``tests/input/``
     - Small input data used by smoke tests and documentation.
   * - ``tests/python/``
     - Python examples and utility workflows.
   * - ``docs/``
     - Sphinx documentation.

Validation Checklist
--------------------

Before opening a pull request, run:

.. prompt:: bash

   make clean
   make PYTHON=python3 all
   ./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_check \
      ellmax=25 ppp=4 Nr=8 rmin=0.00232711 rmax=0.02 \
      verbose=0 verbose_log=0
   cd tests
   python3 python/kappa_cov.py --Cls-file input/Cls_ep2.txt \
      --outdir Output_python --ellmax 25 --ppp 4
   python3 python/covariance_example.py --Cls-file input/Cls_ep2.txt \
      --theta-array input/theta_array.txt --dim 20 --rows 7 --diagonals 4
   cd ..
   python3 -m build --sdist
   python3 -m twine check dist/*
   python3 -m sphinx -b html docs docs/_build/html

Remove generated outputs before committing unless they are intentional test
fixtures.

Adding Parameters
-----------------

Adding a runtime parameter currently requires coordinated edits in:

* ``include/cmdline_defs.h`` for command-line help and defaults;
* ``include/cmdline_data.h`` for the runtime structure;
* ``source/startrun.c`` for command-line parsing, parameter-file parsing,
  checking, and used-values output;
* ``addons/class_lib/input.c`` for Cython-wrapper input parsing;
* ``python/cwlcovpy.pxd.in`` if the parameter changes the Cython-visible
  structure layout;
* documentation tables and examples.

If the parameter affects Python workflows, update ``python/wlcovpy.pyx`` and
add a compact example or test.

Documentation Style
-------------------

Write documentation for users first, then developers:

* start pages with the task or concept the reader needs;
* use explicit commands that can be copied and run;
* state working directories for examples;
* prefer full parameter names in docs;
* document units and file formats at the point of use;
* keep known limitations visible rather than hiding them in code comments.

Publishing ``wlcovpy``
----------------------

Releases use PyPI Trusted Publishing, so no long-lived PyPI token is stored in
GitHub. The publisher configured on PyPI must use these values:

* PyPI project name: ``wlcovpy``;
* GitHub owner: ``sadirs``;
* repository: ``3ptWL-cov``;
* workflow: ``publish.yml``;
* environment: ``pypi``.

Update ``_VERSION_`` in ``addons/class_lib/common.h``, the displayed version in
``include/cmdline_defs.h``, and ``release`` in ``docs/conf.py``. Validate the
source distribution, merge the release changes into ``main``, and publish a
GitHub release tagged with the same version. Publishing the GitHub release
triggers ``.github/workflows/publish.yml``.

Remaining Hardening Work
------------------------

The current documentation and examples are functional, but several engineering
items remain good candidates for follow-up issues:

* add an automated test runner for the compact CLI and Python examples;
* move structured numerical results from stdout into machine-readable outputs;
* replace unchecked ``system`` calls and unsafe ``sprintf`` patterns;
