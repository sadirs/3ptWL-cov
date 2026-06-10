Troubleshooting and Common Errors
=================================

``gsl_sf_bessel_J0`` or ``gsl_sf_bessel_Jn`` Undefined at Link Time
-------------------------------------------------------------------

Cause:
    GSL libraries were not linked after the object files, ``gsl-config`` is not
    on ``PATH``, or the GSL include/library directories are not discoverable.

Fix:
    Use the current Makefile and either expose ``gsl-config`` or pass GSL paths
    explicitly:

    .. prompt:: bash

       make clean
       make PYTHON=python3 GSL_INCLUDE=/usr/include \
            GSL_LIB=/usr/lib/x86_64-linux-gnu all

``python: command not found``
-----------------------------

Cause:
    The Makefile default uses ``PYTHON ?= python``.  Some systems only provide
    ``python3``.

Fix:
    Pass ``PYTHON=python3`` to ``make``.

``No module named Cython``
--------------------------

Cause:
    The wrapper build imports Cython from ``setup.py``.

Fix:
    Install Python build dependencies before running ``make all``:

    .. prompt:: bash

       python3 -m pip install --user numpy Cython scipy

Undefined ``SetNumberThreads`` or ``get_nthreads`` Symbols
----------------------------------------------------------

Cause:
    Older builds did not compile the thread-control and PXD helper symbols
    required by the Cython wrapper.

Fix:
    Rebuild from a checkout containing the current fixes:

    .. prompt:: bash

       make clean
       make PYTHON=python3 all

``wlcovpy requires ADDONSON=1, CLASSLIBON=1, PXDON=1``
------------------------------------------------------

Cause:
    ``setup.py`` generates ``python/cwlcovpy.pxd`` from the active addon
    settings.  The current wrapper expects ``ADDONSON``, ``CLASSLIBON``, and
    ``PXDON`` to be enabled.

Fix:
    Keep ``ADDONSON = 1`` in ``Makefile_settings`` and ``CLASSLIBON = 1`` in
    ``addons/Makefile_addons_settings``.  ``PXDON`` is enabled automatically
    when ``CLASSLIBON`` is on.

ABI Mismatch During Wrapper Import
----------------------------------

Cause:
    The compiled static library and generated ``python/cwlcovpy.pxd`` were
    built with different structure declarations or addon settings.

Fix:
    Rebuild from a clean tree:

    .. prompt:: bash

       make clean
       make PYTHON=python3 all

Parameter File Tags Not Recognized
----------------------------------

Cause:
    Parameter files must use full parameter names.  Aliases such as ``root`` or
    ``in`` are command-line conveniences only.

Fix:
    Use names such as ``rootDir`` and ``clsfile`` in parameter files.

``integral`` Is Zero or NaN
---------------------------

Possible causes:

* ``ellmin`` and ``ellmax`` fall outside the tabulated ``clsfile`` range;
* ``C_ell`` contains non-positive values and the log interpolation becomes
  invalid;
* the input table is not sorted by increasing ``ell``;
* the integration grid is too coarse for the angular scales.

Fix:
    Validate the input table and rerun a convergence check over ``ppp`` and
    ``ellmax``.

Build Warnings
--------------

The current C build can emit warnings related to unchecked ``system`` calls,
``sprintf`` format safety, and long logfile paths.  They do not block the
validated examples, but they should be addressed before hardening the package
for automated production pipelines.
