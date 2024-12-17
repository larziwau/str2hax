    .section .start,"ax"

start:
    bl getstart

getstart:
    mflr 4
    subi 4,4,8

    mfmsr 3
    rlwinm 3,3,0,17,15
    ori 3,3,0x2000
    mtmsr 3
    isync

    lis 3,main@h
    ori 3,3,main@l
    addi 5,3,-4
    addi 4,4,end-start

    li 0,0
    ori 0,0,0x8000
    mtctr 0
0:
    lwzu 0,4(4)
    stwu 0,4(5)
    bdnz 0b

    li 0,0x1000
    mtctr 0
    mr 5,3
0:
    dcbst 0,5
    sync
    icbi 0,5
    addi 5,5,0x20
    bdnz 0b

    sync
    isync

    mtctr 3
    bctr

end:
