#include <arch/early_print.h>
#include "except.h"

static string_t EXCEPTION_TEXTS[] = {
    S("Division Error"),
    S("Debug"),
    S("Non-Maskable Interrupt"),
    S("Breakpoint"),
    S("Overflow"),
    S("Bound Range Exceeded"),
    S("Invalid Opcode"),
    S("Device Not Available"),
    S("Double Fault"),
    S("Coprocessor Segment Overrun"),
    S("Invalid TSS"),
    S("Segment Not Present"),
    S("Stack-Segment Fault"),
    S("General Protection Fault"),
    S("Page Fault"),
    S("Reserved"),
    S("x87 Floating-Point Exception"),
    S("Alignment Check"),
    S("Maching Check"),
    S("SIMD Floating-Point Exception"),
    S("Alignment Check"),
    S("Maching Check"),
    S("SIMD Floating-Point Exception"),
    S("Virtualization Exception"),
    S("Control Protection Exception"),
    S("Reserved"),
    S("Hypervisor Injection Exception"),
    S("VMM Communication Exception"),
    S("Security Exception"),
    S("Reserved"),
    S("Triple Fault"),
    S("FPU Error Interrupt")
};

static void panic(string_t str);

void except_handle_exception(u8 id, u8 code) {
    if (id > 31) {
        panic(S("Invalid Exception"));
    } else {
        panic(EXCEPTION_TEXTS[id]);
    }
    while (true);
}

static void panic(string_t str) {
  early_print(S("["));
  early_print_set_color(EP_RED, EP_BLACK);
  early_print(S("FAIL"));
  early_print_reset_color();
  early_print(S("]: "));
  early_println(str);

  asm volatile(
    "cli\n\t"
    "hlt\n\t"
  );
  while (true);
}