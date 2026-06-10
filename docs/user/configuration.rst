Build Configuration
===================

``wlcov`` uses a Makefile-based build.  User-facing settings live in
``Makefile_settings`` and machine-specific compiler/linker settings live in
``Makefile_machine``.

Makefile Settings
-----------------

.. list-table::
   :header-rows: 1
   :widths: 24 16 60

   * - Setting
     - Default
     - Meaning
   * - ``USEGSL``
     - ``1``
     - Enables GSL-backed Bessel-function routines.  Keep enabled for current code.
   * - ``OPENMPMACHINE``
     - ``0``
     - Enables OpenMP runtime thread control when set to ``1``.
   * - ``ADDONSON``
     - ``1``
     - Enables the CLASS-style input parser, addon include hooks, and Cython
       support.  Keep enabled for ``wlcovpy``.

Addon Settings
--------------

Addon switches live in ``addons/Makefile_addons_settings``.

.. list-table::
   :header-rows: 1
   :widths: 24 16 60

   * - Setting
     - Default
     - Meaning
   * - ``CLASSLIBON``
     - ``1``
     - Enables the CLASS-style parser and error-message infrastructure used by
       the Python wrapper.
   * - ``PXDON``
     - ``1``
     - Enables Cython helper functions and declarations.  It requires
       ``CLASSLIBON = 1``.
   * - ``COSMOLIBON``
     - ``0``
     - Enables the optional cosmology addon in ``addons/cosmo_lib``.  This is
       present in the tree but disabled by default.

Compiler and Linker Variables
-----------------------------

The main variables in ``Makefile_machine`` are:

.. list-table::
   :header-rows: 1
   :widths: 24 56

   * - Variable
     - Purpose
   * - ``CC``
     - C compiler.  The default is ``gcc``.
   * - ``PYTHON``
     - Python executable used to install ``wlcovpy``.  Override with ``PYTHON=python3`` if ``python`` is unavailable.
   * - ``OPTFLAG``
     - Optimization flags, currently ``-O3 -ffast-math``.
   * - ``OMPFLAG``
     - OpenMP compiler flag, currently ``-fopenmp``.
   * - ``GSL_CONFIG``
     - Path to ``gsl-config``.  Defaults to ``gsl-config`` from ``PATH``.
   * - ``GSL_INCLUDE``
     - Optional include directory for GSL headers.
   * - ``GSL_LIB``
     - Optional library directory for GSL libraries.

Recommended Build Pattern
-------------------------

Prefer command-line overrides for machine-local paths so repository defaults
remain portable:

.. prompt:: bash

   make clean
   make PYTHON=python3 all

For a local GSL installation:

.. prompt:: bash

   export GSL_DIR="$HOME/local/gsl"
   export GSL_INCLUDE="$GSL_DIR/include"
   export GSL_LIB="$GSL_DIR/lib"
   make PYTHON=python3 \
        GSL_INCLUDE="${GSL_INCLUDE}" \
        GSL_LIB="${GSL_LIB}" all

Build Artifacts
---------------

``make all`` builds the executable and static library and installs ``wlcovpy``
via ``pip install .``.  The wrapper build generates ``python/cwlcovpy.pxd``
from ``python/cwlcovpy.pxd.in`` so its C struct declarations match the active
addon settings.

For a clean release tree, run:

.. prompt:: bash

   make clean

and remove generated example outputs before committing.
