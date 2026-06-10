#!/usr/bin/env python3
"""Run a compact wlcov calculation through the Python wrapper."""

from wlcovpy import wlcov


model = wlcov(default=False)
model.set(
    {
        "clsfile": "tests/input/Cls_ep2.txt",
        "rootDir": "Output_python_example",
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
    }
)

try:
    cputime = model.Run()
finally:
    model.clean_all()

print(f"MainLoop CPU time: {cputime:.6g} s")
