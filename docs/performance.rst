Performance and Parallelization
===============================

The runtime is dominated by repeated trapezoidal integrations over multipole
and radial grids.  Production settings should be chosen deliberately and
recorded with the run outputs.

Integration Controls
--------------------

``ellmin`` and ``ellmax``
    Bound the multipole integration.  Increasing ``ellmax`` increases the
    number of integration samples.

``ppp``
    Points per Bessel period.  Larger values sample oscillatory Bessel factors
    more densely and increase runtime.  Use convergence tests before adopting a
    production value.

``r`` and angular scales
    The ``F``-type integrals derive their step size from the largest relevant
    angular scale.  Very small angular scales can increase the number of
    samples required over a fixed ``ell`` range.

``Nr``, ``rmin``, and ``rmax``
    Control the final radial integral.  Larger ``Nr`` values improve radial
    sampling but multiply the number of calls to the expensive inner
    multipole integrations.

OpenMP
------

OpenMP is controlled at compile time:

.. prompt:: bash

   make clean
   make PYTHON=python3 OPENMPMACHINE=1 all

At runtime:

.. prompt:: bash

   export OMP_NUM_THREADS=8
   ./wlcov numberThreads=8 clsfile=tests/input/Cls_ep2.txt \
      ellmax=2000 ppp=20 Nr=50

When OpenMP is disabled, ``numberThreads`` is accepted but has no effect.

Embarrassingly Parallel Workflows
---------------------------------

Covariance-matrix workflows often require many independent ``wlcov`` calls.
Even without OpenMP, these calls can be distributed over a job scheduler or a
Python process pool.  Keep each task self-contained:

* write outputs to unique directories;
* log the exact command and parameter values;
* avoid sharing temporary files such as ``Cls_temp.txt`` between concurrent
  jobs unless the filename is made unique per process.

Convergence Checks
------------------

For a production analysis, run a small convergence grid:

* hold geometry fixed and vary ``ppp``;
* hold ``ppp`` fixed and vary ``Nr``;
* hold ``ppp`` fixed and vary ``ellmax``;
* compare final ``integral=...`` values against an acceptance tolerance;
* record the selected settings in the analysis notes or paper repository.

The compact examples in this documentation use small ``ellmax`` and ``ppp``
values so they run quickly; they are smoke tests, not science settings.
