#
# wlcovpy python module setup
# written by: Mario A. Rodriguez-Meza
# date: 01.05.2026

from setuptools import setup, Extension

from Cython.Distutils import build_ext as cython_build_ext

import numpy as nm
import os
import subprocess
import subprocess as sbp
import os.path as osp
import sys

def get_gsl_config(flag):
    try:
        res = subprocess.check_output(["gsl-config", flag], text=True)
        return res.strip()
    except (FileNotFoundError, subprocess.CalledProcessError):
        return ""

GCCPATH_STRING = sbp.Popen(
    ['gcc', '-print-libgcc-file-name'],
    stdout=sbp.PIPE
).communicate()[0]
GCCPATH = osp.normpath(osp.dirname(GCCPATH_STRING)).decode()

root_folder = os.path.dirname(os.path.abspath(__file__))
include_folder = os.path.join(root_folder, "include")
general_lib_folder = os.path.join(root_folder, "general_lib")
getparam_folder = os.path.join(root_folder, "getparam")
source_folder = os.path.join(root_folder, "source")
class_lib_folder = os.path.join(root_folder, "addons", "class_lib")
pxd_folder = os.path.join(root_folder, "addons", "pxd")
wlcovpy_folder = os.path.join(root_folder, "python")
#
addons_folder = os.path.join(root_folder, "addons")
addons_include_folder = os.path.join(addons_folder, "addons_include")
addons_include_include_folder = os.path.join(addons_include_folder, "include")
addons_include_addons_folder = os.path.join(addons_include_folder, "addons")
addons_include_startrun_folder = os.path.join(addons_include_folder, "source", "startrun")
addons_include_wlcov_folder = os.path.join(addons_include_folder, "source", "wlcov")
addons_include_wlcovio_folder = os.path.join(addons_include_folder, "source", "wlcovio")
cosmo_lib_folder = os.path.join(addons_folder, "cosmo_lib")
#

makefile_settings = os.path.join(root_folder, "Makefile_settings")
addons_settings = os.path.join(root_folder, "addons", "Makefile_addons_settings")

def read_makefile_setting(path, key, default="0"):
    try:
        with open(path, "r") as f:
            for line in f:
                line = line.strip()
                if line.startswith(key):
                    return line.split("=", 1)[1].strip()
    except FileNotFoundError:
        pass
    return os.environ.get(key, default)

#B gsl definition
def parse_gsl_config():
    cflags = get_gsl_config("--cflags").split()
    libs_flags = get_gsl_config("--libs").split()

    include_dirs = [
        flag[2:] for flag in cflags
        if flag.startswith("-I") and len(flag) > 2
    ]

    library_dirs = [
        flag[2:] for flag in libs_flags
        if flag.startswith("-L") and len(flag) > 2
    ]

    libraries = [
        flag[2:] for flag in libs_flags
        if flag.startswith("-l") and len(flag) > 2
    ]

    return include_dirs, library_dirs, libraries


gsl_include_dirs, gsl_library_dirs, gsl_libraries = parse_gsl_config()

if os.environ.get("GSL_INCLUDE"):
    gsl_include_dirs = [os.environ["GSL_INCLUDE"]]

if os.environ.get("GSL_LIB"):
    gsl_library_dirs = [os.environ["GSL_LIB"]]

if not gsl_include_dirs:
    gsl_include_dirs = ["/usr/local/include"]

if not gsl_library_dirs:
    gsl_library_dirs = ["/usr/local/lib"]

if not gsl_libraries:
    gsl_libraries = ["gsl", "gslcblas", "m"]

USEGSL = read_makefile_setting(makefile_settings, "USEGSL", "1")
if USEGSL != "1":
    raise RuntimeError("wlcov requires USEGSL=1")

liblist = ["wlcov"] + gsl_libraries
#E to this

if sys.platform.startswith("linux"):
    liblist += ["mvec"]

#B OpenMP definitions
OPENMPMACHINE = read_makefile_setting(
    os.path.join(root_folder, "Makefile_settings"),
    "OPENMPMACHINE",
    default="0",
)

openmp_compile_args = []
openmp_link_args = []

if OPENMPMACHINE == "1":
    openmp_compile_args += ["-fopenmp"]
    openmp_link_args += ["-fopenmp", "-lgomp"]
#E

#B definitions addons macros
ADDONSON = read_makefile_setting(makefile_settings, "ADDONSON", "0")
CLASSLIBON = read_makefile_setting(addons_settings, "CLASSLIBON", "0")
COSMOLIBON = read_makefile_setting(addons_settings, "COSMOLIBON", "0")
if CLASSLIBON == "1":
    PXDON = read_makefile_setting(addons_settings, "PXDON", "0")
else:
    PXDON = "0"

define_macros = [
    ("__WLCOVDIR__", f'"{root_folder}"'),
]

define_macros.append(("USEGSL", None))

if ADDONSON == "1":
    define_macros.append(("ADDONS", None))

    if CLASSLIBON == "1":
        define_macros.append(("CLASSLIB", None))

    if COSMOLIBON == "1":
        define_macros.append(("COSMOLIB", None))

    if PXDON == "1":
        define_macros.append(("PXD", None))
#E


#B generator function
def indent_block(text, spaces=8):
    prefix = " " * spaces
    return "\n".join(prefix + line if line.strip() else line
                     for line in text.strip("\n").splitlines())


def generate_cwlcovpy_pxd():
    template_path = os.path.join(wlcovpy_folder, "cwlcovpy.pxd.in")
    output_path = os.path.join(wlcovpy_folder, "cwlcovpy.pxd")

    cmdline_classlib_fields = ""
    cmdline_cosmolib_fields = ""
    global_classlib_fields = ""
    global_cosmolib_fields = ""

    if ADDONSON == "1" and CLASSLIBON == "1":
        cmdline_classlib_fields = indent_block("""
char base_path[1000]
ErrorMsg error_message
""")

        global_classlib_fields = indent_block("""
ErrorMsg error_message
""")

    if ADDONSON == "1" and COSMOLIBON == "1":
        cmdline_cosmolib_fields = indent_block("""
double h
double Omega0_b
double Omega0_cdm
double Omega0_nu
double ns
double w
double zbin
int chiQuadSteps
int Wg
char * prefix
char * fnamePS
char * fWgchi
""")

        global_cosmolib_fields = indent_block("""
double Omega0_m
double Omega0_de
double Dpz0
double k_data[10000]
double pkz0_data[10000]
unsigned char prefixFlag
unsigned char fnamePSFlag
unsigned char fWgchiFlag
double sigma8
int n_ps_data
int n_chi_data
double chi_data[10000]
double Wg_chi_data[10000]
""")

    with open(template_path, "r") as f:
        text = f.read()

    text = text.replace("{{CMDLINE_CLASSLIB_FIELDS}}", cmdline_classlib_fields)
    text = text.replace("{{CMDLINE_COSMOLIB_FIELDS}}", cmdline_cosmolib_fields)
    text = text.replace("{{GLOBAL_CLASSLIB_FIELDS}}", global_classlib_fields)
    text = text.replace("{{GLOBAL_COSMOLIB_FIELDS}}", global_cosmolib_fields)

    with open(output_path, "w") as f:
        f.write(text)

# This makes the .pxd match the active C macros. But your wlcovpy.pyx also accesses addon-dependent fields/functions. So if you want to support CLASSLIBON=0 or PXDON=0, you must also guard Python wrapper code that assumes those features exist.
if ADDONSON != "1" or CLASSLIBON != "1" or PXDON != "1":
    raise RuntimeError("wlcovpy requires ADDONSON=1, CLASSLIBON=1, PXDON=1")
# and let only COSMOLIBON be optional if the wrapper does not directly require its fields.
#

#E generator function


with open(os.path.join(class_lib_folder, 'common.h'), 'r') as v_file:
    for line in v_file:
        if line.find("_VERSION_") != -1:
            VERSION = line.split()[-1][2:-1]
            break

wlcovpy_ext = Extension(
    "wlcovpy",
        [
            os.path.join(wlcovpy_folder, "wlcovpy.pyx"),
            os.path.join(source_folder, "abi_check.c"),
        ],
    include_dirs=[
        nm.get_include(),
        include_folder,
        class_lib_folder,
        general_lib_folder,
        getparam_folder,
        pxd_folder,
        source_folder,
        #
        cosmo_lib_folder,
        addons_folder,
        addons_include_folder,
        addons_include_include_folder,
        addons_include_addons_folder,
        addons_include_startrun_folder,
        addons_include_wlcov_folder,
        addons_include_wlcovio_folder,
        #
        *gsl_include_dirs,
    ],
    define_macros=define_macros,
    libraries=liblist,
    library_dirs=[
        root_folder,
        GCCPATH,
        *gsl_library_dirs,
    ],
    extra_compile_args=openmp_compile_args,
    extra_link_args=openmp_link_args + ['-lz'],
)

wlcovpy_ext.cython_directives = {
    'language_level': "3" if sys.version_info.major >= 3 else "2"
}

class build_ext(cython_build_ext):
    def run(self):
        generate_cwlcovpy_pxd()
        subprocess.check_call(["make", "libwlcov.a"], cwd=root_folder)
        super().run()

setup(
    name='wlcovpy',
    version=VERSION,
    description='Python interface to the covariance code wlcov',
    url='http://github.com/rodriguezmeza/wlcov.git',
    cmdclass={'build_ext': build_ext},
    ext_modules=[wlcovpy_ext],
)
