API Reference
=============

This page summarizes the callable interfaces and the main implementation
functions in the current ``wlcov`` source tree.  The stable user interfaces are
the command-line executable and the ``wlcovpy.wlcov`` Cython class.  The C
functions are useful for developers who are extending the code or checking the
wrapper declarations.

Command-Line Interface
----------------------

Executable
    ``./wlcov``

Help
    ``./wlcov --help``

Parameter file
    ``./wlcov paramfile=path/to/parameters.txt``

Power-spectrum input
    ``clsfile=path/to/two-column-table.txt``

See :doc:`user/command-line` for the complete parameter table.

Python Module
-------------

.. py:module:: wlcovpy

.. py:class:: wlcov(default=False)

   Cython wrapper around the compiled C core.  The wrapper builds a CLASS-style
   in-memory parameter file, validates that all supplied parameters were read,
   calls the C initialization and integration routines, and exposes selected
   results back to Python.

   :param bool default: If true, initialize wrapper defaults before user
      parameters are added.  The current default is ``False``.

   .. py:attribute:: pars

      Dictionary of parameters currently staged for the next run.

   .. py:attribute:: state

      Lightweight readiness flag.  It currently returns ``True``.

   .. py:method:: set(*pars, **kwargs)

      Add or update runtime parameters.  Parameters may be supplied as one
      mapping positional argument or as keyword arguments.

      .. code-block:: python

         model.set({"clsfile": "tests/input/Cls_ep2.txt", "ellmax": 25})
         model.set(verbose=0, verbose_log=0)

   .. py:method:: Run(level=["EndRun"])

      Execute the requested C routine chain.  The default runs input parsing,
      common initialization, parameter-file writing, thread setup,
      ``MainLoop``, and ``EndRun``.  Returns measured ``MainLoop`` CPU time.

   .. py:method:: clean()

      Clear the Python-side parameter dictionary.

   .. py:method:: struct_cleanup()

      Release allocated C-side command and global-data state when a run has
      partially or fully initialized those structures.

   .. py:method:: clean_all()

      Release allocated C-side state and clear Python-side parameters.

   .. py:method:: abi_sizes()

      Return the ``cmdline_data`` and ``global_data`` sizes reported by the C
      library.  The wrapper checks these sizes during construction to catch
      stale ``cwlcovpy.pxd`` declarations.

   .. py:method:: getNThreads()

      Return the parsed thread count after ``Run`` has initialized the C
      structures.

   .. py:method:: getIntegral()

      Return the final radial integral stored in ``global_data.intval``.

Wrapper Exceptions
~~~~~~~~~~~~~~~~~~

.. py:exception:: CosmoError

   Base wrapper exception.

.. py:exception:: CosmoSevereError

   Raised for invalid wrapper calls, unread parameters, ABI mismatches, or
   string values that cannot be safely copied into C buffers.

.. py:exception:: CosmoComputationError

   Raised when an initialized C stage returns ``FAILURE``.

C Entry Points
--------------

The public C prototypes are declared in ``include/protodefs.h`` and related
addon headers.

.. list-table::
   :header-rows: 1
   :widths: 28 72

   * - Function
     - Purpose
   * - ``StartRun``
     - Initialize command-line and global structures from process arguments or
       a parameter file.
   * - ``StartRun_Common``
     - Shared initialization used by both the executable and ``wlcovpy``:
       default parameters, input table loading, output directory setup, and
       logging.
   * - ``PrintParameterFile``
     - Write the used-values parameter file into the configured output
       directory.
   * - ``SetNumberThreads``
     - Configure OpenMP threads when compiled with ``OPENMPMACHINE=1``.  In a
       serial build it accepts the call and leaves execution single-threaded.
   * - ``StartOutput``
     - Open output/log paths after runtime directories are prepared.
   * - ``MainLoop``
     - Execute the weak-lensing covariance numerical workflow and store the
       final integral in ``gd->intval``.
   * - ``EndRun``
     - Close logs, print final timing information, and release state that must
       not outlive a run.
   * - ``EndRun_FreeMemory_cmd``
     - Free dynamically allocated command-parameter strings.
   * - ``EndRun_FreeMemory_gd``
     - Free dynamically allocated global-data strings.
   * - ``sizeof_cmdline_data``
     - Return the C compiler's size for ``struct cmdline_data`` so the Cython
       wrapper can detect ABI drift.
   * - ``sizeof_global_data``
     - Return the C compiler's size for ``struct global_data`` so the Cython
       wrapper can detect ABI drift.

Numerical Integration Helpers
-----------------------------

The following functions are implemented in ``source/wlcov.c``.  They are
currently internal implementation details rather than a stable public C library
API, but they are useful orientation points for contributors.

.. list-table::
   :header-rows: 1
   :widths: 28 72

   * - Function
     - Purpose
   * - ``Cls``
     - Interpolate the tabulated angular power spectrum ``C_ell`` from
       ``clsfile`` using log-log interpolation.
   * - ``xi_integrand``
     - Evaluate the integrand for the two-point function term
       ``ell * C_ell * J0(r ell)``.
   * - ``xi``
     - Numerically integrate ``xi_integrand`` across ``ellmin`` to ``ellmax``.
   * - ``fmmp_integrand``
     - Evaluate the coupled ``m, mp`` Bessel contribution used by ``fmmp``.
   * - ``fmmp``
     - Integrate the coupled multipole contribution for a pair of angular
       scales.
   * - ``fm_integrand``
     - Evaluate the single-``m`` Bessel contribution used by ``fm``.
   * - ``fm``
     - Integrate the single-``m`` contribution.
   * - ``fmp_integrand``
     - Evaluate the single-``mp`` Bessel contribution used by ``fmp``.
   * - ``fmp``
     - Integrate the single-``mp`` contribution.
   * - ``to_integrate``
     - Combine ``xi``, ``fmmp``, ``fm``, and ``fmp`` into the covariance
       radial integrand.
   * - ``Bessel_Jn``
     - Thin wrapper around the GSL integer-order Bessel function routine.
   * - ``max_of_two`` / ``max_of_three``
     - Small utility functions used when selecting integration scales.

Input and Parser Helpers
------------------------

The CLASS-style parser lives in ``addons/class_lib`` and is used by the Python
wrapper and parameter-file path.

.. list-table::
   :header-rows: 1
   :widths: 32 68

   * - Function
     - Purpose
   * - ``input_find_file``
     - Resolve a parameter file path against the current run context.
   * - ``input_set_root``
     - Set the root path used for resolving relative input files.
   * - ``input_read_from_file``
     - Read a parsed ``file_content`` object into ``cmdline_data`` and
       ``global_data``.
   * - ``input_read_parameters``
     - Dispatch parameter parsing across the core code and enabled addons.
   * - ``input_read_parameters_general``
     - Parse common parameters such as ``clsfile``, ``rootDir``,
       ``numberThreads``, and verbosity controls.
   * - ``input_default_params``
     - Populate command defaults when the wrapper is constructed.
   * - ``parser_read_file``
     - Load a text parameter file into key/value records.
   * - ``parser_read_int`` / ``parser_read_double`` / ``parser_read_string``
     - Read typed scalar values from the parsed records.
   * - ``parser_read_list_of_doubles`` / ``parser_read_list_of_integers`` /
       ``parser_read_list_of_strings``
     - Read typed list values from parameter records.
   * - ``parser_check_options``
     - Validate an option string against an allowed option list.

Optional Cosmology Addon
------------------------

``addons/cosmo_lib`` is present in this source tree but disabled by default
with ``COSMOLIBON = 0`` in ``addons/Makefile_addons_settings``.  When enabled,
it adds cosmology parameters and helper functions for growth factors, distance
grids, lensing kernels, and ``C_ell`` construction.

.. list-table::
   :header-rows: 1
   :widths: 28 72

   * - Function
     - Purpose
   * - ``Initial``
     - Initialize cosmology-addon state.
   * - ``background``
     - Compute background quantities for the configured cosmology.
   * - ``Cls_kappa``
     - Build a convergence power-spectrum table from cosmological inputs.
   * - ``read_inputpk``
     - Read the linear matter power-spectrum table.
   * - ``read_inputWgchi``
     - Read a galaxy-window table as a function of comoving distance.
   * - ``Dplusf`` / ``Dplusf_func``
     - Evaluate the linear growth factor and its integrand.
   * - ``chiArray_all``
     - Build distance-grid arrays for quadrature.
   * - ``compute_gL``
     - Compute the lensing kernel used by the addon.
   * - ``power``
     - Interpolate or evaluate the configured power spectrum.

Example-Script Functions
------------------------

The scripts in ``tests/python`` are examples rather than installed package
modules, but they are documented here because they are useful starting points
for analysis workflows.

.. list-table::
   :header-rows: 1
   :widths: 36 64

   * - Function
     - Purpose
   * - ``kappa_cov.wlcov_routine``
     - Run one compact wrapper calculation and return ``getIntegral()``.
   * - ``covariance_example.add_noise_to_column``
     - Copy a two-column ``C_ell`` table while adding a constant noise term to
       the second column.
   * - ``covariance_example.build_mask``
     - Build a boolean mask for selected covariance-matrix entries.
   * - ``covariance_example.get_valid_indices``
     - Convert a mask into row/column index pairs.
   * - ``covariance_example.calculate_integral``
     - Run one wrapper integral for a selected pair of angular scales.
   * - ``covariance_example.compute_cov_noise``
     - Build a symmetric covariance matrix by repeatedly calling
       ``calculate_integral``.
