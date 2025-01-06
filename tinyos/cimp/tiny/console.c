// @Hustler's Project

#include <asm/barrier.h>
#include <asm/reg.h>
#include <asm/gic.h>
#include <asm/io.h>

#include <hal/uart.h>

#include <compiler.h>
#include <console.h>
#include <string.h>
#include <print.h>
#include <panic.h>
#include <time.h>
#include <util.h>
#include <cpu.h>

/**
 * @Hustler
 *
 * TODO Console Implementation
 */
static char (*__cin__)(void);
static void (*__cout__)(int c);

extern u64 __cmd_list__;
extern u64 __cmd_auto__;
static const cmd_t *volatile cmd_list = (cmd_t *)&__cmd_list__;
static const cmd_t *volatile cmd_auto = (cmd_t *)&__cmd_auto__;

static volatile int __cmd_exec_status;

static volatile int total_num_commands = 0;
static volatile int curr_command_ptr = 0;
static char cmd_history[NUM_HISTORY_ENTRIES][LINE_BUFF_SIZE];

static volatile bool __echo = ECHO_INIT_VALUE;

void setup(void) {

}

void loop(void) {
    while (true)
        active_wait();
}

void prepend_prompt() {
    u32 el;
    char el_tag;

    el = current_el();

    switch ((el >> 2) & 0x3) {
    case 0:
        el_tag = '0';
        break;
    case 1:
        el_tag = '1';
        break;
    case 2:
        el_tag = '2';
        break;
    case 3:
        el_tag = '3';
        break;
    default:
        el_tag = 'n';
        break;
    }

    __print__("[EL%c] ", el_tag);
}

int active_prompt() {
    return true;
}

static void delete(void) {
    __cout__(BACK_SPACE);
    __cout__(SPACE);
    __cout__(BACK_SPACE);
}

static void new_line(void) {
    __cout__(CARRIAGE_RETURN);
    __cout__(NEW_LINE);
}

static void clear_prompt(int char_count) {
    while (char_count) {
        delete();

        char_count--;
    }
}

static void handle_up_arrow(char *cmd_buf, int *char_count) {
    int index;

    if (curr_command_ptr < total_num_commands - NUM_HISTORY_ENTRIES ||
        curr_command_ptr == 0) {
        __print__("%s", cmd_buf);

        return;
    }

    memset(cmd_buf, 0, LINE_BUFF_SIZE);

    curr_command_ptr--;
    index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
    memcpy(cmd_buf, &cmd_history[index], LINE_BUFF_SIZE);
    *char_count = strlen(cmd_buf);

    __print__("%s", cmd_buf);
}

static void handle_down_arrow(char *cmd_buf, int *char_count) {
    int index;

    *char_count = 0;
    memset(cmd_buf, 0, LINE_BUFF_SIZE);

    if (curr_command_ptr == total_num_commands)
        return;

    curr_command_ptr++;
    index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
    memcpy(cmd_buf, &cmd_history[index], LINE_BUFF_SIZE);
    *char_count = strlen(cmd_buf);

    __print__("%s", cmd_buf);
}

static void add_command_to_history(const char *cmd_str) {
    int index;

    if (cmd_str == NULL || !strcmp(cmd_str, ""))
        return;

    index = total_num_commands % NUM_HISTORY_ENTRIES;
    memcpy(&cmd_history[index], cmd_str, LINE_BUFF_SIZE);
    total_num_commands++;
    curr_command_ptr = total_num_commands;
}

static int show_history(int argc, char **argv) {
    u32 end_index = total_num_commands-1;
    u32 beg_index = 0;
    if (total_num_commands > NUM_HISTORY_ENTRIES)
        beg_index = total_num_commands - NUM_HISTORY_ENTRIES;

    for (u32 index = beg_index; index <= end_index; ++index)
        __print__("%s\n", cmd_history[index % NUM_HISTORY_ENTRIES]);

    return 0;
}

PUSH_CMD_LIST(history, "show command history", show_history);

static int prefix_match(char *sub, int len, const char *str) {
    int i;

    if (sub == NULL || str == NULL || len <= 0 || len > strlen(str))
        return false;

    for (i = 0; i < len; ++i) {
        if (sub[i] != str[i])
            return false;
    }

    return true;
}

static void handle_tab(char *cmd_buf, int *char_count) {
    int i = 0;
    int match_count = 0;
    int last_match = -1;

    if (cmd_buf == NULL || char_count <= 0)
        return;

    while (cmd_list[i].name != NULL) {
        if (prefix_match(cmd_buf, *char_count, cmd_list[i].name)) {
            match_count++;
            last_match = i;
            __print__("\n%s", cmd_list[i].name);
        }

        i++;
    }

    if (match_count == 1) {
        memcpy(cmd_buf, cmd_list[last_match].name, LINE_BUFF_SIZE);
        *char_count = strlen(cmd_buf);
    }

    if (match_count) {
        __print__("\n");
        prepend_prompt();

        __print__(PROMPT);
        __print__("%s", cmd_buf);
    }
}

static int parse_line(char **argv, char *line_buf, int argument_size) {
    int argc = 0;
    int pos = 0;
    int length = strlen(line_buf);

    while (pos <= length) {
        if (line_buf[pos] != '\t' && line_buf[pos] != SPACE &&
            line_buf[pos] != END_OF_LINE)
            argv[argc++] = &line_buf[pos];

        for (; line_buf[pos] != '\t' && line_buf[pos] != SPACE &&
            line_buf[pos] != END_OF_LINE;
            pos++)
            ;

        if (line_buf[pos] == '\t' || line_buf[pos] == SPACE)
            line_buf[pos] = END_OF_LINE;

        pos++;
    }

    return argc;
}

static void execute(int argc, char **argv) {
    int match_found = false, i;

    for (i = 0; cmd_list[i].name != NULL; i++) {
    if (strcmp(argv[0], cmd_list[i].name) == 0) {
        __cmd_exec_status = cmd_list[i].xcall(argc, &argv[0]);
        match_found = true;
        break;
    }
    }

    if (match_found == false) {
        __print__("\"%s\": command not found. "
                   "use \"help\" to list all command.\n",
            argv[0]);
        __cmd_exec_status = -1;
    }
}

static void shell(void) {
    int s, argc, i;
    int count = 0;
    int special_key = 0;
    char c;

    char line_buf[LINE_BUFF_SIZE];
    char *argv[MAX_ARG_COUNT];

    memset((void *)line_buf, 0, LINE_BUFF_SIZE);

    for (i = 0; i < MAX_ARG_COUNT; i++)
        argv[i] = NULL;

    prepend_prompt();
    __print__(PROMPT);

    while (true) {
        if (!active_prompt())
            continue;

        s = __cin__();
        if (s != -1) {
            c = (char)s;

            if (c == CARRIAGE_RETURN || c == NEW_LINE) {
                line_buf[count] = END_OF_LINE;

                // @Hustler
                new_line();

                break;
            }

            if (c == DELETE || c == BACK_SPACE) {
                if (!__echo) {
                    delete();
                    delete();
                }

                if (count == 0)
                    continue;

                count--;

                line_buf[count] = END_OF_LINE;
                delete();
            } else if (c == ESCAPE) {
                special_key = 1;
                continue;
            } else if (c == SQUARE_BRACKET_OPEN && special_key == 1) {
                special_key = 2;
                continue;
            } else if ((c == 'C' || c == 'D') && special_key != 0) {
                special_key = 0;
                continue;
            } else if ((c == 'A' || c == 'B') && special_key == 2) {
                if (!__echo)
                    clear_prompt(count + 4);
                else
                    clear_prompt(count);

                if (c == 'A')
                    handle_up_arrow(line_buf, &count);
                else
                    handle_down_arrow(line_buf, &count);

                special_key = 0;
                continue;
            } else if (c == TAB) {
                handle_tab(line_buf, &count);
                continue;
            } else {
                line_buf[count] = c;
                count++;
            }

            if (__echo && c != DELETE && c != BACK_SPACE)
                __cout__(c);
        } else
            loop();
    }

    add_command_to_history(line_buf);

    argc = parse_line(argv, line_buf, MAX_ARG_COUNT);

    if (argc > 0)
        execute(argc, argv);
}

static void exec_auto_cmds(void) {
    int i;

    for (i = 0; cmd_auto[i].name != NULL; i++)
        cmd_auto[i].xcall(0, NULL);
}

int console_init(void) {
    tiny_print("console kicking off\n");

    // @Hustler
    __cin__ = __debug_uart_getc;

    __cout__ = __debug_uart_putc;

    // @Hustler     - This will automatically
    //                execute.
    exec_auto_cmds();

    setup();

    while (1)
        shell();

    /**
     * shall never reach when console enabled
     */
    return 0;
}

int exec(char *cmd_str) {
    int argc;

    // TODO: this takes too much stack space. Optimize!
    char *argv[MAX_ARG_COUNT];

    argc = parse_line(argv, cmd_str, MAX_ARG_COUNT);

    if (argc > 0)
        execute(argc, argv);

    return __cmd_exec_status;
}

xmd xcall_addr(char *cmd_str) {
    int i;

    for (i = 0; cmd_list[i].name != NULL; i++) {
        if (!strcmp(cmd_str, cmd_list[i].name))
            return cmd_list[i].xcall;
    }

    return NULL;
}

// ####################################################

static int build_info(int argc, char **argv) {
    __print__("tiny shell "SHELL_VERSION"."USER_REPO_VERSION" "
               "- "__DATE__" - "__TIME__"\n");

    return 0;
}
PUSH_CMD_AUTO(version, "present details of the build", build_info);

// ####################################################

int help(int argc, char **argv) {
    int i = 0;
    bool verbose = true;

    if (argc > 1 && (!strcmp(argv[1], "-l")))
        verbose = false;
    else
        __print__("use: help -l for list only\n");

    while (cmd_list[i].name != NULL) {
        __print__("<%02d> %s", i, cmd_list[i].name);

        if (verbose) {
            __print__("\n     ");
            __print__(cmd_list[i].help);
        }

        __print__("\n");

        i++;
    }

    return 0;
}
PUSH_CMD_LIST(help, "present all available commands", help);

// ####################################################

int exec_status(int argc, char **argv) {
    __print__("exit code: %d\n", __cmd_exec_status);

    return 0;
}
PUSH_CMD_LIST(status, "present exit status of last executed command",
        exec_status);

// ####################################################

int tiny_boot(int argc, char **argv) {
    int cpu_id;

    if (argc < 3) {
        __print__("usage: %s <smc/hvc> <cpu-id>\n", argv[0]);
        return -1;
    }

    cpu_id = atoi(argv[2]);

    if (!strcmp(argv[1], "smc"))
        smc_boot_cpu(cpu_id);
    else if (!strcmp(argv[1], "hvc"))
        hvc_boot_cpu(cpu_id);
    else {
        __print__("usage: %s <method> <cpu-id>\n", argv[0]);
        return -1;
    }

    return 0;
}
PUSH_CMD_LIST(boot, "continue booting", tiny_boot);

// ####################################################

extern char __tiny_start[], __tiny_end[];
extern char __text_start[], __text_end[];
extern char __data_start[], __data_end[];
extern char __rodata_start[], __rodata_end[];
extern char __console_start[], __console_end[];
extern char __bss_start[], __bss_end[];
extern char __stack_start[], __stack_end[];
extern char __heap_start[], __heap_end[];

int memmap_dump(int argc, char **argv) {
    __print__("memory section address dump:\n");

    __print__("   tiny: 0x%08lx - 0x%08lx [%u Kb]\n",
            (u64)__tiny_start,
            (u64)__tiny_end,
            (((u64)__tiny_end - (u64)__tiny_start) / 1024));
    __print__("   text: 0x%08lx - 0x%08lx\n",
            (u64)__text_start,
            (u64)__text_end);
    __print__("   data: 0x%08lx - 0x%08lx\n",
            (u64)__data_start,
            (u64)__data_end);
    __print__(" rodata: 0x%08lx - 0x%08lx\n",
            (u64)__rodata_start,
            (u64)__rodata_end);
    __print__("console: 0x%08lx - 0x%08lx\n",
            (u64)__console_start,
            (u64)__console_end);
    __print__("    bss: 0x%08lx - 0x%08lx\n",
            (u64)__bss_start,
            (u64)__bss_end);
    __print__("  stack: 0x%08lx - 0x%08lx\n",
            (u64)__stack_start,
            (u64)__stack_end);
    __print__("   heap: 0x%08lx - 0x%08lx\n",
            (u64)__heap_start,
            (u64)__heap_end);

    return 0;
}
PUSH_CMD_LIST(mmd, "present memory section dump", memmap_dump);

// ####################################################

int mem_readl(int argc, char **argv) {
    u32 addr, data;

    if (argc < 2) {
        __print__("usage: %s <addr> (hex)\n", argv[0]);
        return -1;
    }

    addr = atoh(argv[1]);

    data = readl((xmem_t)addr);

    __print__("<R> 0x%08x: 0x%08x\n", addr, data);

    return 0;
}
PUSH_CMD_LIST(mrl, "read value from memory", mem_readl);

// ####################################################

int mem_writel(int argc, char **argv) {
    u32 addr, data;

    if (argc < 3) {
        __print__("usage: %s <addr> (hex) <val> (hex)\n",
                argv[0]);
        return -1;
    }

    addr = atoh(argv[1]);
    data = atoh(argv[2]);

    __print__("<R> 0x%08x: 0x%08x\n", addr,
            readl((xmem_t)addr));

    writel(data, (xmem_t)addr);

    __print__("<W> 0x%08x: 0x%08x\n", addr,
            readl((xmem_t)addr));

    return 0;
}
PUSH_CMD_LIST(mwl, "write value to memory", mem_writel);

// ####################################################

int memdump(int argc, char **argv) {
    u32 addr, length, i, data;

    if (argc < 3) {
        __print__("usage: %s <addr> (hex) <num_words> (decimal)\n",
                argv[0]);
        return -1;
    }

    addr = atoh(argv[1]);
    length = atoi(argv[2]);

    for (i = 0, data = 0; i < length; i++) {
        data = readl((xmem_t)addr);

        __print__("<R> 0x%08x: 0x%08x\n", addr, data);

        addr += 4;
    }

    return 0;
}
PUSH_CMD_LIST(md, "present memory dump", memdump);

// ####################################################

// TODO
int tiny_reboot(int argc, char **argv) {

    reboot(1000);

    return 0;
}
PUSH_CMD_LIST(reboot, "reboot tiny", tiny_reboot);

// ####################################################

// CAUTION
int off_cpu(int argc, char **argv) {
    if (argc < 2) {
        __print__("usage: %s <authorization code>\n",
                argv[0]);
        return -1;
    }

    if (!strcmp(argv[1], "502")) {
        __print__("cpu off ...\n");

        cpu_off();
    }

    return 0;
}
PUSH_CMD_LIST(off, "turn off cpu", off_cpu);

// ####################################################

/**
 * Note
 *
 * On executing an HVC instruction, the PE records the
 * exception as a Hypervisor Call exception in the
 * ESR_ELx, using the EC value 0x16, and the value of
 * immediate argument.
 */

// ####################################################

int get_active_intid(int argc, char **argv) {

    __print__("active interrupt id: %u\n",
            arm_gic_get_active());

    return 0;
}
PUSH_CMD_LIST(irqalive, "present active interrupt",
              get_active_intid);

// ####################################################

extern void dump_stack(void);

int backtrace(int argc, char **argv) {

    dump_stack();

    return 0;
}
PUSH_CMD_LIST(trace, "dump current backtrace", backtrace);

// ####################################################

int fire(int argc, char **argv) {



    return 0;
}
PUSH_CMD_LIST(fire, "experimental fire cases", fire);

// ####################################################

static volatile cmd_t noop_cmd
    __attribute__((__section__(".cmd_end"))) = {
    NULL, NULL, NULL,
};

// ####################################################
