# Dockerfile
FROM llvm-base

# 1. Setup Environment Variables
ENV PATH="/usr/local/llvm-16/bin:$PATH"
ENV LD_LIBRARY_PATH="/usr/local/llvm-16/lib:$LD_LIBRARY_PATH"

RUN apt-get update && apt-get install -y \
    graphviz \
    gcc-9 \
    g++-9 \
    && rm -rf /var/lib/apt/lists/*

# 2. Install Rust 1.69.0
ENV RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo \
    PATH=/usr/local/cargo/bin:$PATH

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- \
    -y --default-toolchain 1.69.0 --profile minimal

# 3. Set up your workspace
WORKDIR /app

# Default command
CMD ["/bin/bash"]
