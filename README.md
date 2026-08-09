# 3ptWL-cov

[![Documentation Status](https://readthedocs.org/projects/3ptwl-cov/badge/?version=latest)](https://3ptwl-cov.readthedocs.io/en/latest/?badge=latest)
[![PyPI version](https://img.shields.io/pypi/v/wlcovpy.svg)](https://pypi.org/project/wlcovpy/)
[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/sadirs/3ptWL-cov/blob/main/docs/examples/wlcovpy_covariance_colab.ipynb)

**3ptWL-cov** computes the Gaussian contribution to weak-lensing three-point
covariance terms in a harmonic basis on the sphere under the Limber
approximation.  The repository provides a C command-line executable, a static
library, a Cython wrapper (`wlcovpy`), and small Python examples.

Documentation: [3ptWL-cov documentation](https://3ptwl-cov.readthedocs.io/en/latest/)

## Authors

- **Sofia Samario-Nava** (ICF-UNAM, Mexico), ssamario@icf.unam.mx
- **Alejandro Aviles** (ICF-UNAM, Mexico), aviles@icf.unam.mx

**Other people who contributed to this code:**

- **Sadi Ramirez Solano** (ICF-UNAM, Mexico) - [sadi@icf.unam.mx](mailto:sadi@icf.unam.mx)
- **Mario A. Rodriguez-Meza** (ININ, Mexico)

## Install with pip

Install the native build dependencies first. On Debian or Ubuntu:

```bash
sudo apt-get update
sudo apt-get install build-essential libgsl-dev python3-dev
```

Then install the Python interface from PyPI:

```bash
python3 -m pip install wlcovpy
```

For a no-checkout example, open the covariance tutorial in Colab:
[Open in Colab](https://colab.research.google.com/github/sadirs/3ptWL-cov/blob/main/docs/examples/wlcovpy_covariance_colab.ipynb)

This installs the `wlcovpy` module and compiles its native extension; no
separate `make` command is needed. For a version-matched reproduction of the
current documentation, install `python3 -m pip install "wlcovpy==1.0.1"`.
To install the unreleased development version, use
`python3 -m pip install "git+https://github.com/sadirs/3ptWL-cov.git"`. From an
existing source checkout, use `python3 -m pip install .`. The standalone
`wlcov` command-line executable is built separately with `make`, as described
below.

## Native C build (optional)

Build the standalone executable and static library from a source checkout:

```bash
make clean
make PYTHON=python3 all
```

The Makefile uses `gsl-config` by default.  For a custom GSL installation:

```bash
make PYTHON=python3 GSL_INCLUDE=/path/to/gsl/include GSL_LIB=/path/to/gsl/lib all
```

Native build artifacts:

- `wlcov`: command-line executable
- `libwlcov.a`: static C library
- `wlcovpy`: Cython extension installed by `pip install .`

## Native CLI quick run

```bash
./wlcov clsfile=tests/input/Cls_ep2.txt rootDir=Output_quick \
   r=0.01 theta1=0.01 theta2=0.012 thetap1=0.011 thetap2=0.013 \
   m=0 mp=0 ellmin=1 ellmax=25 ppp=4 Nr=8 rmin=0.00232711 rmax=0.02 \
   verbose=0 verbose_log=0
```

The main input table is supplied with `clsfile` and must contain two columns:
`ell` and `C_ell`.

## Python Wrapper

```python
from wlcovpy import wlcov

model = wlcov(default=False)
model.set({
    "clsfile": "tests/input/Cls_ep2.txt",
    "rootDir": "Output_python",
    "r": 0.01,
    "theta1": 0.01,
    "theta2": 0.012,
    "thetap1": 0.011,
    "thetap2": 0.013,
    "m": 0,
    "mp": 0,
    "ppp": 4,
    "ellmin": 1.0,
    "ellmax": 25.0,
    "Nr": 8,
    "rmin": 0.00232711,
    "rmax": 0.02,
    "numberThreads": 1,
    "verbose": 0,
    "verbose_log": 0,
    "options": "",
})
try:
    model.Run()
    integral = model.getIntegral()
finally:
    model.clean_all()
```

See `docs/examples/python_wrapper.py`, `tests/python/`, the
[self-contained Colab notebook](docs/examples/wlcovpy_covariance_colab.ipynb),
and the source-checkout notebook in `tests/notebooks/example.ipynb` for
runnable examples. The source-checkout notebook saves its figures in
`tests/notebooks/plots/`.

## Configuration

Main build switches live in `Makefile_settings`:

- `USEGSL = 1`: required for current GSL Bessel routines
- `OPENMPMACHINE = 0`: set to `1` for OpenMP builds
- `ADDONSON = 1`: required for the current wrapper path

Addon switches live in `addons/Makefile_addons_settings`:

- `CLASSLIBON = 1`
- `PXDON = 1`
- `COSMOLIBON = 0` by default

## Citation

If you use this program in research that results in publications, please cite
Sofia Samario et al., arXiv:2506.19811, and record the code version, compiler,
GSL version, Python environment, and runtime configuration used for the
analysis.

## License

3ptWL-cov is distributed under the MIT license.  See [LICENSE](LICENSE).

## Acknowledgements

We acknowledge financial support from grants DGAPA-PAPIIT IA101825 and SECIHITI CBF2023-2024-162
