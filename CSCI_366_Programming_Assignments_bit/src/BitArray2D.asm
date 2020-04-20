
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col
        ; add your code here


        mov rax,rdx
        mov rdx, 0
        imul rax,rsi
        add rax,rcx
        ;sal rax,4
        mov rsi, 8
        idiv rsi
        add rdi, rax
        ;add rax,rdi
        mov rsi, 1
        mov rbx, 7
        sub rbx, rdx
        mov rcx, rbx
        sal rsi, cl
        mov rax, rdi
        or [rax], rsi
        ;mov rax, 1
        ;mov rax,rdx
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax

get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame
        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col
        ; add your code here - for now returning 0
        mov rax, rdx
        mov rdx, 0
        imul rax, rsi
        add rax, rcx
        ;sal rax,4
        mov rsi, 8
        idiv rsi
        add rdi, rax
        ;add rax,rdi
        mov rsi, 1
        mov rbx, 7
        sub rbx, rdx
        mov rcx, rbx
        sal rsi, cl


        mov rax, rdi
        and [rax], rsi

        ;cmp rdi, 0

        setg al
        movsx rax, al

;        cmp rax,1
        ;mov rax, 1

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
