import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_performance(csv_path):
    if not os.path.exists(csv_path):
        print(f"Error: {csv_path} not found.")
        return

    df = pd.read_csv(csv_path)
    
    plt.figure(figsize=(12, 6))

    # Plot FPS
    plt.subplot(1, 2, 1)
    plt.plot(df['frame_id'], df['fps'], label='FPS', color='green')
    plt.title('SLAM Real-time Performance (FPS)')
    plt.xlabel('Frame ID')
    plt.ylabel('FPS')
    plt.grid(True)
    plt.legend()

    # Plot Latency
    plt.subplot(1, 2, 2)
    plt.plot(df['frame_id'], df['detect_ms'], label='Detection (ms)', color='blue')
    plt.plot(df['frame_id'], df['match_ms'], label='Matching (ms)', color='red')
    plt.title('Computation Latency')
    plt.xlabel('Frame ID')
    plt.ylabel('Time (ms)')
    plt.grid(True)
    plt.legend()

    plt.tight_layout()
    plt.savefig('data/performance_plot.png')
    plt.show()

if __name__ == "__main__":
    plot_performance('data/metrics.csv')
