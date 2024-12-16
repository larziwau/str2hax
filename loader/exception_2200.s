    .globl exception_2200_start, exception_2200_end

exception_2200_start:
    mfcr    0           ; stw 0,0x2080(0)
    mfxer   0           ; stw 0,0x2084(0)
    mflr    0           ; stw 0,0x2088(0)
    mfctr   0           ; stw 0,0x208c(0)
    mfsrr0  0           ; stw 0,0x2090(0)
    mfsrr1  0           ; stw 0,0x2094(0)
    mfdar   0           ; stw 0,0x2098(0)
    mfdsisr 0           ; stw 0,0x209c(0)

    mfmsr   0           ; ori 0,0,0x2030
    mtsrr1  0

    lis     0,exception_handler@h
    ori     0,0,exception_handler@l
    mtsrr0  0
    rfi

exception_2200_end:
