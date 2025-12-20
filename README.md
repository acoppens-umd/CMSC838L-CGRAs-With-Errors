# Morpher-v2: Rust Edition

## A Modification by Alex Coppens & Jeremy Sha

![Morpher Cover](https://user-images.githubusercontent.com/12274945/198943201-17e9ff67-62b3-445f-bd04-feac08da1601.png)

[![Actions Status](https://github.com/ecolab-nus/morpher-v2/workflows/Build%20and%20Test/badge.svg)](https://github.com/ecolab-nus/morpher-v2/actions)

Welcome to **Morpher**, an open-source framework that provides comprehensive support for modeling diverse Coarse-Grained Reconfigurable Array (CGRA) architectures. The framework offers the following key features:

- **Architecture Description**: Morpher enables users to design architecture characteristics through its Architecture Description Language (ADL).
  
- **Complex Kernels**: It efficiently handles the mapping of complex compute kernels, going beyond simple test cases.
  
- **RTL Generation**: Morpher automatically generates Verilog RTL code for custom CGRAs.

- **Functionality Verification**: The framework validates the functionality of the architecture through Verilator/C++-based simulations.

**Morpher Framework:**
![Morpher Framework](https://github.com/ecolab-nus/morpher-v2/assets/12274945/80329bad-dc35-42b2-93a4-843d7c1f4550)

# Build and Run Guide for Morpher-v2: Rust Edition

## Prerequisites

- An environment running **Ubuntu 22.04**.

## Setup and Build Steps

1. **Clone and Setup the Repository**:
```bash
git clone [YOUR_REPOSITORY_LINK] 
cd [YOUR_REPOSITORY_NAME]       
```

2. **Setup LLVM (Requires 16.0.0)**:

```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout 08d094a0e457360ad8b94b017d2dc277e697ca76
mkdir llvm-build && cd llvm-build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=TRUE -DLLVM_TARGETS_TO_BUILD="X86" ../llvm
make -j$(nproc)
```
3. **Add LLVM to PATH**:

```bash
export PATH=$PATH:[YOUR_PATH_TO_LLVM]/llvm-project/llvm-build/bin
```
4. **Setup Python**:

```bash
sudo apt install python3.8
python3 -m venv venv
source venv/bin/activate
```
5. **Install Dependencies**:

```bash
echo "deb [arch=amd64] http://archive.ubuntu.com/ubuntu focal main universe" | sudo tee -a /etc/apt/sources.list
sudo apt update
sudo apt -y install g++-7 
pip install -r python_requirements.txt
sudo apt-get install gcc-multilib g++-multilib
sudo apt install build-essential 
sudo snap install --classic rustup
```
6. **Build**:

```bash
bash build_all.sh
```
7. **Run**:

```bash
bash eval.sh csr_csr csr_csr_matmul
```
