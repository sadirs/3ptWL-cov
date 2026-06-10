#  ============================================================================
#         1          2          3          4        ^ 5          6          7

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import os, argparse, gc, numpy as np
from pathlib import Path
import matplotlib.pyplot as plt
#B for cBalls
import sys
# Determine the absolute path of the target (wlcovpy) directory
#   these two lines won´t be necessary if wlcovpy is in the searching path
#target_directory = os.path.abspath('/opt/homebrew/anaconda3/lib/python3.13/site-packages/')
# Append the directory to sys.path
#sys.path.append(target_directory)
from wlcovpy import wlcov
#E

# ----------------------------
# File utilities
# ----------------------------
def add_noise_to_column(input_file: str, output_file: str, noise: float):
    with open(input_file) as infile, open(output_file, "w") as outfile:
        for line in infile:
            if not line.strip():
                outfile.write("\n")
                continue

            parts = line.split()
            if len(parts) != 2:
                outfile.write(line)
                continue

            try:
                val = float(parts[1]) + noise
                outfile.write(f"{parts[0]}\t{val}\n")
            except ValueError:
                outfile.write(line)

# ----------------------------
# Mask builder (vectorized)
# ----------------------------
def build_mask(dim, rows, diagonals, symm=True):
    mask = np.ones((dim, dim), dtype=bool)

    mask[:rows, :] = False
    mask[:, :rows] = False

    for d in range(diagonals):
        mask[np.abs(np.subtract.outer(np.arange(dim), np.arange(dim))) == d] = False

    if symm:
        mask = np.triu(mask)

    return mask

# ----------------------------
# Index extraction
# ----------------------------
def get_valid_indices(mask):
    rows, cols = np.where(mask)
    return np.column_stack((rows, cols))

def calculate_integral(theta1, theta2, thetap1, thetap2,
                       m, mp, ppp, inputfile):
    Cov = wlcov()
    Cov.set({'theta1':theta1})
    Cov.set({'thetap1':thetap1})
    Cov.set({'theta2':theta2})
    Cov.set({'thetap2':thetap2})
    Cov.set({'clsfile':inputfile})
    Cov.set({'m':m})
    Cov.set({'mp':mp})
    Cov.set({'ppp':ppp})
    Cov.set({'options':''})
    Cov.set({'verbose':0})
    Cov.set({'verbose_log':0})
    cputime=Cov.Run()
    val = Cov.getIntegral()
    Cov.clean_all()
    gc.collect()

    return val

# -------- compute_cov_noise --------
def compute_cov_noise(rows, diagonals, dim,
                m, mp, ppp, noise,
                thtdata=[],
                nthreads=1, outputfile=None, inputfile=None):

    # Step 1: prepare noisy file ONCE
    temp_file="Cls_temp.txt"
    add_noise_to_column(inputfile, temp_file, noise)

    # Step 2: build mask + indices
    mask = build_mask(dim, rows, diagonals)
    indices = get_valid_indices(mask)

    n = len(indices)
    cov = np.zeros((n, n))

    # Step 3: compute matrix
    for i in range(n):
        if i % 5 == 0:
            print(f"Progress: {i}/{n}")

        for j in range(i + 1):  # exploit symmetry
            idx1 = indices[i]
            idx2 = indices[j]

            th1, th2 = thtdata[idx1]
            th1p, th2p = thtdata[idx2]

            val = calculate_integral(
                th1, th2, th1p, th2p,
                m, mp, ppp,
                temp_file)

            cov[i, j] = val
            cov[j, i] = val
            
    if outputfile is not None:
        np.savetxt(outputfile, cov, delimiter=" ")
        print(f"Saving covariance matrix to: {outputfile}")

    return cov

def main():
    ap = argparse.ArgumentParser(description="Compute covariance with noise.")
    ap.add_argument("--Cls-file", type=Path, required=True,
                    help="Path to the Cls power spectrum.")
    ap.add_argument("--output", default="analytic_covariance_22_noise.txt",
                    help="Output file to save covariance (default: '...').")
    ap.add_argument("--theta-array", default=None,
                    help="array the triangle sides.")
    ap.add_argument("--threads", type=int, default=1,
                    help="OpenMP number of threads. By now there is no OpenMP version.")
    ap.add_argument("--rows", type=int, default=7, help="rows.")
    ap.add_argument("--diagonals", type=int, default=4, help="diagonals.")
    ap.add_argument("--dim", type=int, default=20,
                    help="dimensions. Must be the same as theta-array.")
    ap.add_argument("--m", type=int, default=2, help="multipole index.")
    ap.add_argument("--mp", type=int, default=2, help="multipole index.")
    ap.add_argument("--ppp", type=int, default=20, help="points per decade.")
    ap.add_argument("--noise", type=float, default=6.1e-11,
                    help="Poissonian noise term added to the Cls power spectrum.")
    args = ap.parse_args()

    ofilename = "analytic_covariance_22_noise.pdf"

    #B Theta array
    if args.theta_array == None:         # if none build one
        thtdata = np.array([0.00232712, 0.00275673, 0.00326565, 0.00386852, 0.00458269,
            0.00542871, 0.00643091, 0.00761812, 0.00902451, 0.01069053,
            0.01266412, 0.01500206, 0.0177716 , 0.02105243, 0.02493894,
            0.02954293, 0.03499688, 0.04145769, 0.04911122, 0.05817769])
        print('thtdata:')
        print(thtdata)
    else:
        thtdata = np.loadtxt(args.theta_array)
        print('thtdata:')
        print(thtdata)
    #E

    # Computing convergence covariance matrix with noise
    covariance = compute_cov_noise(args.rows, args.diagonals, args.dim,
            args.m, args.mp, args.ppp, args.noise, thtdata,
            nthreads=args.threads, outputfile=args.output,
            inputfile=args.Cls_file)
    print()

    plt.figure(figsize=(6,5))
    plt.imshow(np.log10(abs(covariance)))
    plt.colorbar(label="Covariance")
    plt.title("Covariance Matrix")
    plt.xlabel("Index")
    plt.ylabel("Index")
    plt.savefig(ofilename,dpi=300)
    # if you want plot to be shown, uncomment this line
    #plt.show()

if __name__ == "__main__":
    main()
