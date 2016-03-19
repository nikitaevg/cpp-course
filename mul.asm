%define SYSCALL_WRITE 0x2000004
%define SYSCALL_READ 0x2000003
%define SYSCALL_EXIT  0x2000001

section         .text
;rdi, rsi, rbp, rcx, rbx
                global          start
start:

                sub             rsp, 2 * 8 * 128        ;free memory for two bigint
                lea             rdi, [rsp + 8 * 128]    ;move rdi to the first bigint
                mov             rcx, 128                ;length
                call            read_long
                mov             rdi, rsp
                call            read_long
                lea             rdi, [rsp + 8 * 128]    ;move rdi to the beginning of the second operand
                mov             rsi, rsp                ;move rsi to the beginning of the first operand
                sub             rsp, 8 * 128 * 4        ;free space for answer
                mov             r15, rdi
                mov             rdi, rsp
                add             rcx, rcx
                call            set_zero
                mov             rdi, r15
                mov             rcx, 128
                mov             r15, rsp

                call            add_long_long
                ;call            copy

                mov             rdi, r15
                call            write_long

                mov             al, 0x0a;next line code
                call            write_char;printf("\n")

                jmp             exit

; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    r15 -- address of temporary long number
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                mov             rbp, rcx        ; second counter
                xor             r12, r12        ; rsi adder
                mov             r13, rcx

.loop1:
                xor             rbx, rbx        ;carry
                xor             r11, r11        ;rdi adder
                clc
.loop2:
                mov             rax, [rdi + r11];mul
                mov             r10, [rsi + r12];mul
                mul             r10             ;mul   rdx:rax
                add             rax, rbx        ;add old carry
                adc             rdx, 0          ;add to new carry carry from prev op
                lea             r14, [r11 + r12];calc adder
                add             [r15 + r14], rax;add value
                adc             rdx, 0          ;add to new carry carry from prev op
                mov             rbx, rdx        ;write carry
                add             r11, 8          ;mov adder for rdi++
                dec             rcx             ;dec cnt
                jnz             .loop2


                mov             rcx, r13        ;repair rcx (counter for inner loop)
                add             r12, 8          ;add to adder for rsi
                dec             rbp             ;dec cnt
                jnz             .loop1


                pop             rcx
                pop             rsi
                pop             rdi
                ret


; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx;
.loop:
                add             [rdi], rax  ;rdi += rax         ;rdx -- carry
                adc             rdx, 0      ;rdx += carry
                mov             rax, rdx    ;rax = rdx;
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi ;carry
.loop:
                mov             rax, [rdi]          ;take last digit
                mul             rbx                 ;multiply it by rbx
                add             rax, rsi            ;add carry(old)
                adc             rdx, 0              ;rdx += carry(new)
                mov             [rdi], rax          ;write multiplied value
                add             rdi, 8              ;goto next digit
                mov             rsi, rdx            ;write carry
                dec             rcx                 ;dec counter
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder; rdi % rbx
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8];move rdi to the higher digit
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]              ; rax = higher digit
                div             rbx                     ; rax = rdx:rax / rbx rdx = rdx:rax % rbx
                mov             [rdi], rax              ; higher digit = rax / rbx
                sub             rdi, 8                  ; goto next digit
                dec             rcx;
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
;rep repeats operation while rcx is not a null
;stosq set rax to rdi and rdi++
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq
                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax            ;check for sign
                js              exit                ;exit program if rax<0 (error occured)
                cmp             rax, 0x0a           ;new line code
                je              .done               ;input is over
                cmp             rax, '0'            ;is a digit
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short      ;result *= 10
                call            add_long_short      ;result += char
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx                 ; rax *= 20
                mov             rbp, rsp            ; local backup of rsp in rbp
                sub             rsp, rax            ; stack_pointer -= 20 * rcx

                mov             rsi, rbp            ; rsi = rsp rsi works like the pointer where to write the char

.loop:
                mov             rbx, 10
                call            div_long_short      ; take rdi % 10
                add             rdx, '0'            ; make it char
                dec             rsi                 ;goto next address to write char
                mov             [rsi], dl           ;write char (part of rdx)
                call            is_zero
                jnz             .loop

                mov             rdx, rbp            ;rdx = rsp
                sub             rdx, rsi            ;rdx = rsp - rsi (curr pntr) ;calculate size
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1              ;free space for char
                mov             rax, SYSCALL_READ   ;read
                xor             rdi, rdi            ;stdout
                mov             rsi, rsp            ;where to write
                mov             rdx, 1              ;length
                syscall

                cmp             rax, 1              ;if OK. in rax the size of read part is placed
                jne             .error
                xor             rax, rax            ;rax = 0
                mov             al, [rsp]           ;al (half half half of rax) = read char
                add             rsp, 1              ;return stack pointer to prev pointer

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret


; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, SYSCALL_WRITE
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, SYSCALL_EXIT
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, SYSCALL_WRITE
                mov             rdi, 1
                syscall

                pop             rax
                ret


                section         .data
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
