.global main

main:
    la a0, helloworld 
    call printf

    li a7, 2 # 2 - exit syscall 
    ecall

.data
helloworld:      .ascii "Hello, world\n"
