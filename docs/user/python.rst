Python Wrapper
==============

The Cython extension module is named ``wlcovpy`` and exposes the ``wlcov``
class.  The wrapper drives the same C core as the command-line executable.

Import
------

.. code-block:: python

   from wlcovpy import wlcov

Minimal Run
-----------

.. literalinclude:: ../examples/python_wrapper.py
   :language: python

Run it from the repository root:

.. prompt:: bash

   python3 docs/examples/python_wrapper.py

Wrapper Semantics
-----------------

``wlcov(default=False)`` constructs an empty model.  Set all required runtime
parameters with ``set`` before calling ``Run``.

``wlcov(default=True)`` pre-populates verbosity and thread parameters, but a
scientific run should still set the geometry, integration range, input file,
and output directory explicitly.

``Run`` returns the measured ``MainLoop`` CPU time.  The final radial integral
is available with ``getIntegral`` after a successful run, and the C routines
also print run diagnostics according to the verbosity settings.

Cleaning Up
-----------

Call ``clean_all`` after a run to release C-side state:

.. code-block:: python

   model = wlcov(default=False)
   try:
       model.set(parameters)
       cputime = model.Run()
   finally:
       model.clean_all()

Example Utilities
-----------------

``tests/python/kappa_cov.py`` runs one compact wrapper calculation and prints
the returned integral.  ``tests/python/covariance_example.py`` provides
convenience functions that call the wrapper repeatedly and assemble covariance
matrices in NumPy:

* ``add_noise_to_column`` creates a noisy copy of a two-column input table;
* ``calculate_integral`` runs one ``wlcovpy`` calculation and returns
  ``getIntegral``;
* ``build_mask`` and ``get_valid_indices`` define matrix entries to compute;
* ``compute_cov_noise`` computes a symmetric covariance matrix and optionally
  writes it with ``numpy.savetxt``.

See :doc:`../tutorials/covariance-matrix` for the main notebook example in
``tests/notebooks/example.ipynb`` and the matching command-line script run.

Current Limitations
-------------------

The Python wrapper is intentionally thin.  It does not yet expose individual
intermediate arrays or structured numerical results.  For production Python
workflows, capture stdout or add dedicated C/Python accessors for the quantities
needed by downstream analysis.
