import time
import random
import sys

# CONFIGURATION
OUTPUT_FILE = "huge_test.log"
TARGET_SIZE_GB = 1  # How many Gigabytes do you want?
CHUNK_SIZE = 10000  # Write 10k lines at a time for speed

# DATA POOLS
LOG_LEVELS = ["INFO", "INFO", "INFO", "WARN", "DEBUG", "ERROR"]
MESSAGES = [
    "User logged in successfully",
    "Database connection established",
    "Cache miss for key: user_session_123",
    "Failed to connect to microservice B",
    "Payment gateway timeout (5000ms)",
    "NullPointerException in Module X",
    "Disk usage warning: 85% full",
    "Background job completed in 120ms"
]

def generate_log_line():
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
    level = random.choice(LOG_LEVELS)
    msg = random.choice(MESSAGES)
    # Simulate a realistic log format
    # Example: 2025-10-25 14:30:01 [ERROR] Payment gateway timeout (5000ms)
    return f"{timestamp} [{level}] {msg}\n"

def main():
    print(f" Generating {TARGET_SIZE_GB}GB log file: {OUTPUT_FILE}...")
    start_time = time.time()
    
    bytes_written = 0
    target_bytes = TARGET_SIZE_GB * 1024 * 1024 * 1024
    
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        while bytes_written < target_bytes:
            chunk = []
            # Build a large chunk in memory first (faster disk I/O)
            for _ in range(CHUNK_SIZE):
                chunk.append(generate_log_line())
            
            data = "".join(chunk)
            f.write(data)
            bytes_written += len(data)
            
            # Progress bar logic
            percent = (bytes_written / target_bytes) * 100
            sys.stdout.write(f"\rProgress: {percent:.2f}%")
            sys.stdout.flush()

    duration = time.time() - start_time
    print(f"\n✅ Done! File created in {duration:.2f} seconds.")

if __name__ == "__main__":
    main()