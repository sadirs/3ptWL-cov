# 3ptWL-cov

[![Documentation Status](https://readthedocs.org/projects/3ptwl-cov/badge/?version=latest)](https://3ptwl-cov.readthedocs.io/en/latest/?badge=latest)

**3ptWL-cov** computes the Gaussian contribution to weak-lensing three-point
covariance terms in a harmonic basis on the sphere under the Limber
approximation.  The repository provides a C command-line executable, a static
library, a Cython wrapper (`wlcovpy`), and small Python examples.

Documentation: [3ptWL-cov documentation](https://3ptwl-cov.readthedocs.io/en/latest/)

## Authors

- **Sofia Samario-Nava**  
  ssamario@icf.unam.mx
- **Alejandro Aviles**  
  aviles@icf.unam.mx

Contributor:

- Mario A. Rodriguez-Meza  
  marioalberto.rodriguezmeza@gmail.com

## Build

Install GSL first, then build from the repository root:

```bash
python3 -m pip install --user numpy Cython scipy
make clean
make PYTHON=python3 all
```

The Makefile uses `gsl-config` by default.  For a custom GSL installation:

```bash
make PYTHON=python3 GSL_INCLUDE=/path/to/gsl/include GSL_LIB=/path/to/gsl/lib all
```

Build artifacts:

- `wlcov`: command-line executable
- `libwlcov.a`: static C library
- `wlcovpy`: Cython extension installed by `pip install .`

## Quick Run

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

See `docs/examples/python_wrapper.py`, `tests/python/`, and the main notebook
example in `tests/notebooks/example.ipynb` for runnable examples.  The notebook
saves its figures in `tests/notebooks/plots/`.

## Documentation

The documentation is built with Sphinx:

```bash
python3 -m pip install --user -r docs/requirements.txt
cd docs
make html
make man
make latexpdf
```

Generated outputs:

- `docs/_build/html/index.html`
- `docs/_build/man/wlcov.1`
- `docs/_build/latex/3ptWL-cov.pdf`

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
