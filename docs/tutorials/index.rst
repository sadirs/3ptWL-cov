Tutorials
=========

These tutorials are intentionally compact and runnable on a development
machine.  They demonstrate the current public interfaces without requiring a
large production covariance run.

The covariance-matrix workflow is also available as a self-contained Google
Colab notebook backed by the published PyPI package:

.. image:: https://colab.research.google.com/assets/colab-badge.svg
   :target: https://colab.research.google.com/github/sadirs/3ptWL-cov/blob/main/docs/examples/wlcovpy_covariance_colab.ipynb
   :alt: Open the covariance-matrix tutorial in Google Colab

The public Colab source lives at
``docs/examples/wlcovpy_covariance_colab.ipynb``. The repository-local
development notebook lives at ``tests/notebooks/example.ipynb`` and saves its
figures under ``tests/notebooks/plots``. The covariance-matrix tutorial also
renders copies from ``docs/images/notebooks``, plus the R2D2 covariance plot
used to document the paper-data reproduction workflow.

.. toctree::
   :maxdepth: 1

   minimal-cli
   python-wrapper
   covariance-matrix
