#include "user_logic/uart_driver.h"
#include "user_logic/uart_parser.h"
#include "user_logic/reg_io.h"
#include "core_logic/demapper.h"
#include "func_reg_32.h"

#define RX_BUF_LEN  128


static void wait_for_idle(void)
{
    while (reg_get_command() != CMD_IDLE)
        dispatcher_poll();   /* drive core_logic in the same thread */
}

int main(void)
{
    char rx_buf[RX_BUF_LEN];

    uart_init();

    uart_sendline("=== AFE SPI Register Bridge Ready ===");
    uart_sendline("Format: OPCODE:<hex> ARG:<hex> [ARG:<hex> ...]");
    uart_sendline("Example (raw write): OPCODE:00 ARG:01 ARG:00A0 ARG:5F");
    uart_sendline("Example (burst read): OPCODE:03 ARG:00 ARG:1000 ARG:04");

    /* Ensure the system starts in a known idle state */
    reg_set_command(CMD_IDLE);
    reg_set_status(STATUS_EXEC_PASS);
    reg_set_result(0);

    while (1) {
        /* ── Prompt ──────────────────────────────────────────────── */
        uart_sendstr("> ");

        /* ── Receive one command line from PC ────────────────────── */
        int len = uart_getline(rx_buf, RX_BUF_LEN);
        if (len == 0) continue;   /* blank line — ignore */

        /* ── user_logic: parse → write registers → CMD_START ──────── */
        int parse_ok = parser_handle_line(rx_buf);
        if (parse_ok != 0) {
            /* parser already sent an error message over UART */
            continue;
        }

        /* ── core_logic: poll until execution completes ────────────── */
        wait_for_idle();

        /* ── Report result to PC ─────────────────────────────────── */
        uint8_t  status = reg_read8(REG_STATUS_CODE);
        uint32_t result = reg_get_result();

        if (status == STATUS_EXEC_PASS) {
            uart_sendstr("STATUS: PASS  RESULT: ");
            uart_sendhex(result);
            uart_sendchar('\r');
            uart_sendchar('\n');
        } else {
            uart_sendline("STATUS: FAIL");
        }
    }

    return 0;
}
