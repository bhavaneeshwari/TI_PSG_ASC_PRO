#include "wrapper_fns.h"

#include <stdlib.h>
#include <string.h>
#include "../user_logic/reg_io.h"

void genWrapperFn(funcSpec spec, void **args, buffer_t *buf)
{
    (void)buf;   /* not used for write functions */

    uint32_t ret = spec.funcName(args);
    reg_set_result(ret);
}

void readWrapperFn(funcSpec spec, void **args, buffer_t *buf)
{

    uint8_t last = (uint8_t)(spec.num_args);
    args[last] = &buf->buf;

    uint32_t ret = spec.funcName(args);

    uint32_t result_packed = 0;
    if (buf->buf != NULL) {
        uint32_t copy_len = (buf->len < 4u) ? buf->len : 4u;
        memcpy(&result_packed, buf->buf, copy_len);
    }

    reg_set_result(result_packed);


    (void)ret;
}
