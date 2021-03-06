// Copyright (c) 2019, XMOS Ltd, All rights reserved

#include <stdarg.h>
#include <stdint.h>

#include "rtos_support.h"

void soc_peripheral_function_code_tx(
        chanend c,
        uint32_t code)
{
    uint32_t state = rtos_interrupt_mask_all();

    /* TODO: Only output 8bit code + end token */
    chan_out_word(c, code);

    rtos_interrupt_mask_set(state);
}

void soc_peripheral_varlist_tx(
        chanend c,
        int num_args,
        ...)
{
    transacting_chanend_t tc;
    va_list ap;
    int i;

    uint32_t state = rtos_interrupt_mask_all();

    chan_init_transaction_master(&c, &tc);

    va_start(ap, num_args);
    for (i = 0; i < num_args; i++) {
        int arg_size = va_arg(ap, int);
        void *arg_ptr = va_arg(ap, void *);

        t_chan_out_buf_byte(&tc, arg_ptr, arg_size);
    }
    va_end(ap);

    chan_complete_transaction(&c, &tc);

    rtos_interrupt_mask_set(state);
}

void soc_peripheral_function_code_rx(
        chanend c,
        uint32_t *code)
{
    uint32_t state = rtos_interrupt_mask_all();

    /* TODO: Only input 8bit code + end token */
    chan_in_word(c, code);

    rtos_interrupt_mask_set(state);
}

void soc_peripheral_varlist_rx(
        chanend c,
        int num_args,
        ...)
{
    transacting_chanend_t tc;
    va_list ap;
    int i;

    uint32_t state = rtos_interrupt_mask_all();

    chan_init_transaction_slave(&c, &tc);

    va_start(ap, num_args);
    for (i = 0; i < num_args; i++) {
        int arg_size = va_arg(ap, int);
        void *arg_ptr = va_arg(ap, void *);

        t_chan_in_buf_byte(&tc, arg_ptr, arg_size);
    }
    va_end(ap);

    chan_complete_transaction(&c, &tc);

    rtos_interrupt_mask_set(state);
}
