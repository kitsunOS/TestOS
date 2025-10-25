global isr_stub_table

extern except_handle_exception
extern idt_handle_irq

; TODO: Print stacktrace
%macro ISR_EXCEPTION 1
  isr%1:
    cli
    pusha
    mov eax, [esp + 36]
    push eax
    push %1
    call except_handle_exception
    add esp, 8
    popa
    add esp, 4
    iret
%endmacro

%macro ISR_EXCEPTION_NOPARAM 1
  isr%1:
    cli
    pusha
    push 0
    push %1
    call except_handle_exception
    add esp, 8
    popa
    iret
%endmacro

%macro ISR_IRQ 2
  isr%1:
    cli
    pusha
    push %2
    call idt_handle_irq
    add esp, 4
    popa
    iret
%endmacro

%macro ISR_NOP 1
  isr%1:
    iret
%endmacro

%macro DD_ISR 1
  dd isr%1
%endmacro

section .text

ISR_EXCEPTION_NOPARAM 0
ISR_EXCEPTION_NOPARAM 1
ISR_EXCEPTION_NOPARAM 2
ISR_EXCEPTION_NOPARAM 3
ISR_EXCEPTION_NOPARAM 4
ISR_EXCEPTION_NOPARAM 5
ISR_EXCEPTION_NOPARAM 6
ISR_EXCEPTION_NOPARAM 7
ISR_EXCEPTION 8
ISR_EXCEPTION_NOPARAM 9
ISR_EXCEPTION 10
ISR_EXCEPTION 11
ISR_EXCEPTION 12
ISR_EXCEPTION 13
ISR_EXCEPTION 14
ISR_EXCEPTION_NOPARAM 15
ISR_EXCEPTION_NOPARAM 16
ISR_EXCEPTION 17
ISR_EXCEPTION_NOPARAM 18
ISR_EXCEPTION_NOPARAM 19
ISR_EXCEPTION_NOPARAM 20
ISR_EXCEPTION 21
ISR_EXCEPTION_NOPARAM 22
ISR_EXCEPTION_NOPARAM 23
ISR_EXCEPTION_NOPARAM 24
ISR_EXCEPTION_NOPARAM 25
ISR_EXCEPTION_NOPARAM 26
ISR_EXCEPTION_NOPARAM 27
ISR_EXCEPTION_NOPARAM 28
ISR_EXCEPTION_NOPARAM 29
ISR_EXCEPTION_NOPARAM 30
ISR_EXCEPTION_NOPARAM 31


%assign i 32
%rep 16
  %assign j (i-32)
  ISR_IRQ i, j
  %assign i i+1
%endrep

%assign i 48
%rep 208
  ISR_NOP i
  %assign i i+1
%endrep

section .data
isr_stub_table:
  %assign i 0
  %rep 256
    DD_ISR i
    %assign i i+1
  %endrep