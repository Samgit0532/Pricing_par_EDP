# Black–Scholes PDE Pricer (Finite Differences)

This project implements a **Black–Scholes pricer** based on the numerical resolution of the Black–Scholes partial differential equation using an **explicit finite-difference scheme**.

It supports several financial products (European and American options, forwards, spreads, straddles) and provides:
- an **interactive application** allowing the user to choose a product and input parameters,
- a **test executable** checking pricing consistency (put–call parity, American dominance, forward pricing, etc.).

---

## 1. Build and run with CMake

### Requirements
- CMake ≥ 3.16  
- A C++ compiler with **C++17** support (e.g. `g++`)

### Build
From the root of the project:

```bash
cmake -S . -B build
cmake --build build -j
```

### Run the interactive application
```bash
./build/bs_app
```

### Run the test suite
```bash
./build/bs_tests
```

The test executable performs automatic sanity checks on prices and Greeks.

---

## 2. Build and run with g++ (without CMake)

### Requirements
- `g++` with C++17 support

### Compile the interactive application
```bash
g++ -std=c++17 -O2 -O2 -I./src src/main.cpp src/solvers/Solver.cpp -o bs_app
```

Run:
```bash
./bs_app
```

### Compile the test executable
```bash
g++ -std=c++17 -O2 -I./src src/tests/TestPricing.cpp src/solvers/Solver.cpp -o bs_tests
```

Run:
```bash
./bs_tests
```

---

