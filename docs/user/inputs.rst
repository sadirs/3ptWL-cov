Inputs and Parameter Files
==========================

``wlcov`` has two kinds of input:

* runtime parameters, supplied on the command line or in a parameter file;
* a two-column angular power-spectrum table.

Runtime Parameter Files
-----------------------

Parameter files use ``name = value`` syntax:

.. literalinclude:: ../examples/minimal.params
   :language: text

Run the file from the repository root:

.. prompt:: bash

   ./wlcov paramfile=docs/examples/minimal.params

Unlike command-line options, parameter files should use full parameter names
rather than aliases.  Comment lines may begin with ``#`` or ``%``.

Current caveat: keep ``Nr``, ``rmin``, and ``rmax`` on the command line or in
the Python wrapper for now.  The compact parameter-file example intentionally
uses the compiled defaults for those radial-integration controls.

Power-Spectrum Table
--------------------

``clsfile`` must point to a whitespace-delimited table with two numeric
columns:

.. code-block:: text

   ell  C_ell

The current interpolation routine assumes:

* rows are sorted by increasing ``ell``;
* ``ell`` values are positive;
* ``C_ell`` values are positive over the sampled range because interpolation is
  performed in ``log10(C_ell)``;
* values outside the tabulated ``ell`` range evaluate to zero.

The repository includes a small test table:

.. prompt:: bash

   head tests/input/Cls_ep2.txt

Units and Conventions
---------------------

Angular quantities ``r``, ``theta1``, ``theta2``, ``thetap1``, and ``thetap2``
are read in radians.  The executable prints the same quantities in arcminutes
as a convenience check.

``ellmin`` and ``ellmax`` bound the multipole integration.  ``ppp`` controls
the trapezoidal integration spacing through a points-per-period convention; see
:doc:`../performance` before increasing production ranges.

``Nr``, ``rmin``, and ``rmax`` control the final radial integration.  The code
samples ``Nr`` logarithmically spaced radii from ``rmin`` to ``rmax`` and
integrates the radial covariance contribution with a trapezoidal rule.

Reproducibility Notes
---------------------

Each run writes a used-values file under ``rootDir``.  Treat that file as part
of the run record along with:

* the exact command or parameter file;
* the input power-spectrum file;
* the ``wlcov`` commit hash;
* compiler, GSL, Python, NumPy, and Cython versions;
* OpenMP settings when enabled.
