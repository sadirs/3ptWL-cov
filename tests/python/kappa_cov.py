#  ============================================================================
#         1          2          3          4        ^ 5          6          7

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import os, argparse, gc, numpy as np
from pathlib import Path
#B for cBalls
import sys
# Determine the absolute path of the target (wlcovpy) directory
#   these two lines won´t be necessary if wlcovpy is in searching path
#target_directory = os.path.abspath('/opt/homebrew/anaconda3/lib/python3.13/site-packages/')
# Append the directory to sys.path
#sys.path.append(target_directory)
from wlcovpy import wlcov
#E

# -------- wlcov --------
def wlcov_routine(ellmin, ellmax, r, theta1, thetap1, theta2, thetap2,
                m, mp, ppp,
                nthreads=1,
                outdir=None, clsfile=None):
    Cov = wlcov()
    Cov.set({'r':r})
    Cov.set({'theta1':theta1})
    Cov.set({'thetap1':thetap1})
    Cov.set({'theta2':theta2})
    Cov.set({'thetap2':thetap2})
    Cov.set({'clsfile':clsfile})
    if outdir == None:
        Cov.set({'rootDir':'Output'})
    else:
        Cov.set({'rootDir':outdir})
    Cov.set({'ellmax':ellmax})
    Cov.set({'ellmin':ellmin})
    Cov.set({'m':m})
    Cov.set({'mp':mp})
    Cov.set({'ppp':ppp})
    Cov.set({'numberThreads':nthreads})
    Cov.set({'options':''})
    Cov.set({'verbose':2})
    Cov.set({'verbose_log':2})

    print('Running...')
    cputime=Cov.Run()
    val = Cov.getIntegral()

    print('MainLoop cputime=',cputime,' sec.')
    print('cleaning all...')
    Cov.clean_all()
    print('done.')
    gc.collect()
    return val

def main():
    ap = argparse.ArgumentParser(description="Gaussian piece of the 3PCF modeling with wlcov.")
    ap.add_argument("--Cls-file", type=Path, required=True,
                    help="Path to the Cls power spectrum.")
    ap.add_argument("--outdir", default="Output",
                    help="Output directory (default: 'Output').")
    ap.add_argument("--threads", type=int, default=1)
    ap.add_argument("--r", type=float, default=0.116355, help="radial distance.")
    ap.add_argument("--theta1", type=float, default=0.1, help="theta1 distance.")
    ap.add_argument("--thetap1", type=float, default=0.1, help="thetap1 distance.")
    ap.add_argument("--theta2", type=float, default=0.1, help="theta2 distance.")
    ap.add_argument("--thetap2", type=float, default=0.1, help="thetap2 distance.")
    ap.add_argument("--ellmin", type=float, default=1)
    ap.add_argument("--ellmax", type=float, default=2000.0)
    ap.add_argument("--m", type=int, default=0)
    ap.add_argument("--mp", type=int, default=0)
    ap.add_argument("--ppp", type=int, default=20)
    args = ap.parse_args()

    # Computing convergence covariance matrix
    val = wlcov_routine(args.ellmin, args.ellmax, args.r,
            args.theta1, args.thetap1, args.theta2, args.thetap2,
            args.m, args.mp, args.ppp,
            nthreads=args.threads,
            outdir=args.outdir, clsfile=args.Cls_file)
    print()
    print('Integral = ', val)
    print()

if __name__ == "__main__":
    main()
