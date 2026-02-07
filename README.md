# RISC-V AudioMark Challenge: Q15 AXPY Optimization

This repository contains a fully verified, Vector-Length Agnostic (VLA) implementation of the Q15 AXPY kernel  using RISC-V Vector (RVV) intrinsics.

## 1. Design Choices
The implementation prioritizes **numerical accuracy** and **architectural portability** through the following design decisions:

### A. Precision Preservation (Widening)
To match the scalar reference's 32-bit accumulation behavior and prevent intermediate overflow, I utilized **widening arithmetic**:
- **`vwmul.vx` (Widening Multiply):** Expands 16-bit inputs ($b$) into 32-bit products ($b \cdot \alpha$).
- **`vwadd.wv` (Widening Add):** Accumulates the 16-bit source ($a$) directly into the 32-bit product register.
*Rationale:* Doing this math in 16-bit would result in significant data loss before saturation. Widening ensures bit-exactness with the standard `int32_t` scalar reference.

### B. Hardware Saturation (Narrowing)
Instead of using costly branching logic for saturation, I leveraged the RISC-V **`vnclip`** instruction:
- **`vnclip.wx`:** Performs narrowing right-shift with saturation.
- **GCC 15 Compliance:** Explicitly configured the rounding mode to `__RISCV_VXRM_RNU` (Round-to-Nearest-Up) to satisfy strict GCC 15.2.0 intrinsic API requirements and ensure deterministic rounding behavior compatible with Q15 fixed-point standards.

### C. Portability (VLA)
- **Vector-Length Agnostic:** The loop utilizes `vsetvl` to dynamically adapt to the hardware's VLEN.
- **Type Safety:** Used `ptrdiff_t` for loop counters and `size_t` for vector lengths, ensuring the code compiles and runs correctly on both **RV32** (`ilp32d`) and **RV64** (`lp64d`) ABIs.

## 2. Measured Results & Verification

### Verification Methodology
The solution was verified against a scalar "Golden Model" using a randomized test harness.
- **Compiler:** `riscv64-elf-gcc 15.2.0` (Static Linking)
- **Simulator:** QEMU User Mode (CPU: `rv64,v=true,vlen=128`)
- **Correctness:** **PASSED** (Bit-exact match on 1024-element random vectors).

### Performance Logic
By using `LMUL=4` (grouping 4 vector registers), the implementation processes up to **32 elements per instruction** (on VLEN=128 hardware), significantly reducing loop overhead compared to the scalar baseline which processes 1 element per iteration.
