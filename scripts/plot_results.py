# NOTE: liberal use of generative ai

import csv
from pathlib import Path
import matplotlib.pyplot as plt
 
RESULTS_DIR = Path(__file__).parent.parent / "results" 
R_VALUES = [3, 4, 5]
 
 
def read_csv(path):
    p, ber, bler = [], [], []
    with open(path, newline="") as f:
        reader = csv.reader(f)
        next(reader)  # skip header
        for row in reader:
            p.append(float(row[0]))
            ber.append(float(row[1]))
            bler.append(float(row[2]))
    return p, ber, bler
 
 
def plot_metric(r, p_c, classical_vals, p_n, neural_vals, metric_name, ylabel, out_path):
    fig, ax = plt.subplots(figsize=(6, 4))
    ax.plot(p_c, classical_vals, marker="o", label="Classical (syndrome lookup)")
    ax.plot(p_n, neural_vals, marker="s", label="Neural (syndrome-based)")
    ax.set_xlabel("Channel bit-flip probability p")
    ax.set_ylabel(ylabel)
    ax.set_title(f"Hamming({2**r - 1},{2**r - 1 - r}) — {metric_name} vs p")
    ax.legend()
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    fig.savefig(out_path, dpi=150)
    plt.close(fig)
    print(f"wrote {out_path}")
 
 
def main():
    for r in R_VALUES:
        classical_path = RESULTS_DIR / f"classical_r{r}.csv"
        neural_path = RESULTS_DIR / f"neural_r{r}.csv"
 
        if not classical_path.exists() or not neural_path.exists():
            print(f"skipping r={r}: missing CSV(s)")
            continue
 
        p_c, ber_c, bler_c = read_csv(classical_path)
        p_n, ber_n, bler_n = read_csv(neural_path)
 
        plot_metric(r, p_c, ber_c, p_n, ber_n,
                    "Bit Error Rate", "Bit error rate",
                    RESULTS_DIR / "plots" / f"ber_r{r}.png")
        plot_metric(r, p_c, bler_c, p_n, bler_n,
                    "Block Error Rate", "Block error rate",
                    RESULTS_DIR / "plots" / f"bler_r{r}.png")
 
 
if __name__ == "__main__":
    main()

