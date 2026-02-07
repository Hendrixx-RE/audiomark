#include <riscv_vector.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void q15_axpy_rvv(const int16_t *a, const int16_t *b, int16_t *y, int n,
                  int16_t alpha) {
  size_t vl;
  ptrdiff_t count = n;
  while (count > 0) {
    vl = __riscv_vsetvl_e16m4((size_t)count);
    vint16m4_t va = __riscv_vle16_v_i16m4(a, vl);
    vint16m4_t vb = __riscv_vle16_v_i16m4(b, vl);
    vint32m8_t vprod_wide = __riscv_vwmul_vx_i32m8(vb, alpha, vl);
    vint32m8_t vacc_wide = __riscv_vwadd_wv_i32m8(vprod_wide, va, vl);
    vint16m4_t vres =
        __riscv_vnclip_wx_i16m4(vacc_wide, 0, __RISCV_VXRM_RNU, vl);
    __riscv_vse16_v_i16m4(y, vres, vl);
    a += vl;
    b += vl;
    y += vl;
    count -= vl;
  }
}
