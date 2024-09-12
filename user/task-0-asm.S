.global main

main:
    li a0, 1 # 1 = stdout
    la a1, helloworld # load adress of helloworld
    li a2, 13 # len of string
    li a7, 16 # 16 write syscall code
    ecall

    li a7, 2 # 2 - exit syscall 
    ecall

.data
helloworld:      .ascii "Hello, world\n"
