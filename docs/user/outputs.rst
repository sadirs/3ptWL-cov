Outputs and File Formats
========================

The current ``wlcov`` C core writes numerical results to standard output and
provenance files under ``rootDir``.  Downstream scripts should treat the stdout
format as a lightweight interface and record the used-values file for
reproducibility.

Standard Output
---------------

A compact run prints:

* the input angular parameters in radians and arcminutes;
* ``ellmin``, ``ellmax``, ``clsfile``, ``m``, ``mp``, and ``ppp``;
* intermediate terms such as ``xi`` and ``F``-type integrals;
* ``integrand(r=...)=...`` for the selected radius;
* ``integral=...`` for the final radial integration;
* CPU timings for major blocks.

Example parser:

.. code-block:: python

   import re
   import subprocess

   result = subprocess.run(
       [
           "./wlcov",
           "clsfile=tests/input/Cls_ep2.txt",
           "ellmax=25",
           "ppp=4",
           "Nr=8",
           "rmin=0.00232711",
           "rmax=0.02",
           "verbose=0",
           "verbose_log=0",
       ],
       check=True,
       capture_output=True,
       text=True,
   )

   match = re.search(r"integral=\\s*([+-]?\\d*\\.?\\d+(?:[eE][+-]?\\d+)?)", result.stdout)
   integral = float(match.group(1))

Provenance Files
----------------

Every run with a non-empty ``rootDir`` writes a parameter record:

.. code-block:: text

   rootDir/parameters_null-wlcov-usedvalues

For parameter-file runs, the basename changes to match the input parameter file
name:

.. code-block:: text

   rootDir/<parameter-file-name>-usedvalues

Logs
----

When ``verbose_log`` is positive, ``wlcov`` also writes:

.. code-block:: text

   rootDir/tmp/wlcov.log

Use ``verbose=0 verbose_log=0`` for automated test runs where only stdout and
used-values files are needed.

Notebook and Python Utility Outputs
-----------------------------------

The main notebook example, ``tests/notebooks/example.ipynb``, saves diagnostic
figures in:

.. code-block:: text

   tests/notebooks/plots/inputs_and_mask.png
   tests/notebooks/plots/covariance_summary.png

When executed, the notebook also writes a compact covariance matrix:

.. code-block:: text

   tests/notebooks/analytic_covariance_example.txt

The script version, ``tests/python/covariance_example.py``, demonstrates the
same small covariance-matrix workflow from the command line.  It writes:

.. code-block:: text

   tests/analytic_covariance_quickstart.txt
   tests/analytic_covariance_quickstart.pdf

The text file is a whitespace-delimited numeric matrix written by
``numpy.savetxt``.  The PDF and PNG plots are diagnostic images of
``log10(abs(covariance))`` and related matrix summaries.

Recommended Production Output Practice
--------------------------------------

For publication workflows, store generated stdout, used-values files, input
tables, plotting scripts, and environment metadata together.  If the stdout
format becomes a stable public interface, prefer adding a structured output
option such as CSV, HDF5, or FITS rather than parsing free-form console output.
