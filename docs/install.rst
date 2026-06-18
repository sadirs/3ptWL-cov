Installation
============

The Python interface can be installed directly with ``pip``. A source checkout
can additionally build the standalone C executable and static library.

Prerequisites
-------------

Required build tools:

* a POSIX-like shell environment;
* ``make``;
* a C compiler such as ``gcc``;
* the `GNU Scientific Library`_ headers and libraries;
* Python 3 with ``pip``;
* ``git`` when installing directly from GitHub.

Recommended documentation tools:

* ``sphinx``;
* ``sphinx-rtd-theme``;
* ``sphinx-prompt``.

Install the Python Interface
----------------------------

On Debian or Ubuntu, install the native compiler and GSL dependency:

.. prompt:: bash

   sudo apt-get install build-essential libgsl-dev

Install ``wlcovpy`` directly from GitHub:

.. prompt:: bash

   python3 -m pip install "git+https://github.com/sadirs/3ptWL-cov.git"

The pip build installs NumPy and SciPy when needed, generates the matching
Cython declarations, builds the native library internally, and installs the
``wlcovpy`` extension. No separate ``make`` command is required.

From an existing source checkout, the equivalent command is:

.. prompt:: bash

   python3 -m pip install .

Build the Native Executable
---------------------------

To also build the standalone C executable and static library:

.. prompt:: bash

   make clean
   make PYTHON=python3 all

The build creates:

* ``wlcov``: command-line executable;
* ``libwlcov.a``: static C library used by the wrapper;
* ``wlcovpy``: importable Cython extension installed by ``pip``.

Custom GSL Installations
------------------------

The Makefile uses ``gsl-config`` by default.  If GSL is installed outside
standard compiler paths, either set ``GSL_CONFIG`` or override the include and
library directories directly:

.. prompt:: bash

   export GSL_DIR="$HOME/local/gsl"
   export GSL_INCLUDE="$GSL_DIR/include"
   export GSL_LIB="$GSL_DIR/lib"
   make PYTHON=python3 \
        GSL_INCLUDE="${GSL_INCLUDE}" \
        GSL_LIB="${GSL_LIB}" all

The Python extension also reads ``GSL_INCLUDE`` and ``GSL_LIB`` when generating
the Cython extension.  The wrapper build calls ``make libwlcov.a`` so the
static library and generated ``python/cwlcovpy.pxd`` match the active addon
settings.

OpenMP Builds
-------------

OpenMP support is controlled in ``Makefile_settings`` with
``OPENMPMACHINE = 1``.  It can also be enabled from the command line:

.. prompt:: bash

   make clean
   make PYTHON=python3 OPENMPMACHINE=1 all

Set ``OMP_NUM_THREADS`` or pass ``numberThreads=<N>`` at runtime.  When
``OPENMPMACHINE = 0``, ``numberThreads`` is accepted for API consistency but
does not change execution.

Verify the Installation
-----------------------

Check the command-line interface:

.. prompt:: bash

   ./wlcov --help

Run the compact C example:

.. prompt:: bash

   ./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_check \
      ellmax=25 ppp=4 verbose=0 verbose_log=0

Check the Python wrapper:

.. prompt:: bash

   cd tests
   python3 python/kappa_cov.py --Cls-file input/Cls_ep2.txt \
      --outdir Output_python --ellmax 25 --ppp 4

Build the Documentation
-----------------------

The documentation follows the standard Sphinx builder convention:

.. prompt:: bash

   python3 -m pip install --user -r docs/requirements.txt
   cd docs
   make html
   make man
   make latexpdf

The generated artifacts are:

* ``docs/_build/html/index.html``;
* ``docs/_build/man/wlcov.1``;
* ``docs/_build/latex/3ptWL-cov.pdf``.

``make latexpdf`` requires a LaTeX installation with ``latexmk`` and
``pdflatex`` available on ``PATH``.
