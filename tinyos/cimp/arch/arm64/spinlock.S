// @Hustler's Project

#include <asm/asm.h>

FUNC(__spin_trylock__)
    mov   x2, x0
    mov   x1, #1
    ldaxr x0, [x2]
    cbnz  x0, 1f
    stxr  w0, x1, [x2]
1:
    ret
END(__spin_trylock__)

// @Hustler
FUNC(__spin_lock__)
    mov   x1, #1
    sevl
1:
    wfe
    ldaxr x2, [x0]
    cbnz  x2, 1b
    stxr  w2, x1, [x0]
    cbnz  w2, 1b
    ret
END(__spin_lock__)

// @Hustler
FUNC(__spin_unlock__)
    stlr  xzr, [x0]
    ret
END(__spin_unlock__)
