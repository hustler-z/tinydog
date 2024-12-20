// @Hustler's Project

#include <console.h>
#include <string.h>
#include <print.h>

int (*__cin__)(void);
void (*__cout__)(char c);

extern u64 __cmd_list__;
extern u64 __cmd_auto__;
static const cmd_t *volatile cmd_list = (cmd_t *)&__cmd_list__;
static const cmd_t *volatile cmd_auto = (cmd_t *)&__cmd_auto__;

static volatile int __cmd_exec_status;

static volatile int total_num_commands = 0;
static volatile int curr_command_ptr = 0;
static char cmd_history[NUM_HISTORY_ENTRIES][LINE_BUFF_SIZE];

static volatile bool __echo = ECHO_INIT_VALUE;

void set_cin(int (*func)(void)) {
    __cin__ = func;
}

void set_cout(void (*func)(char)) {
    __cout__ = func;
}

void setup(void) {

}

void loop(void) {

}

void prepend_prompt() {

}

int active_prompt() {
    return true;
}

static void delete(void) {
    __cout__(BACK_SPACE);
    __cout__(SPACE);
    __cout__(BACK_SPACE);
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
        tiny_print("%s", cmd_buf);

        return;
    }

    memset(cmd_buf, 0, LINE_BUFF_SIZE);

    curr_command_ptr--;
    index = (curr_command_ptr % NUM_HISTORY_ENTRIES);
    memcpy(cmd_buf, &cmd_history[index], LINE_BUFF_SIZE);
    *char_count = strlen(cmd_buf);

    tiny_print("%s", cmd_buf);
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

    tiny_print("%s", cmd_buf);
}

static void add_command_to_history(const char *cmd_str) {
    int index;

    if (cmd_str == NULL || strcmp(cmd_str, "") == 0)
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
        tiny_print("%s\n", cmd_history[index % NUM_HISTORY_ENTRIES]);

    return 0;
}

PUSH_CMD_LIST(history, "Show command history", show_history);

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
            tiny_print("\n%s", cmd_list[i].name);
        }

        i++;
    }

    if (match_count == 1) {
        memcpy(cmd_buf, cmd_list[last_match].name, LINE_BUFF_SIZE);
        *char_count = strlen(cmd_buf);
    }

    if (match_count) {
        tiny_print("\n");
        prepend_prompt();
        tiny_print(PROMPT);
        tiny_print("%s", cmd_buf);
    }
}

static int parse_line(char **argv, char *line_buff, int argument_size) {
    int argc = 0;
    int pos = 0;
    int length = strlen(line_buff);

    while (pos <= length) {
        if (line_buff[pos] != '\t' && line_buff[pos] != SPACE &&
            line_buff[pos] != END_OF_LINE)
            argv[argc++] = &line_buff[pos];

        for (; line_buff[pos] != '\t' && line_buff[pos] != SPACE &&
            line_buff[pos] != END_OF_LINE;
            pos++)
            ;

        if (line_buff[pos] == '\t' || line_buff[pos] == SPACE)
            line_buff[pos] = END_OF_LINE;

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
        tiny_print("\"%s\": command not found. "
                   "Use \"help\" to list all command.\n",
            argv[0]);
        __cmd_exec_status = -1;
    }
}

static void shell(void) {
    int s, argc, i;
    int count = 0;
    int special_key = 0;
    char c;

    char line_buff[LINE_BUFF_SIZE];
    char *argv[MAX_ARG_COUNT];

    for (i = 0; i < LINE_BUFF_SIZE; i++)
        line_buff[i] = 0;

    for (i = 0; i < MAX_ARG_COUNT; i++)
        argv[i] = NULL;

    prepend_prompt();
    tiny_print(PROMPT);

    while (true) {
        if (!active_prompt())
            continue;

        s = __cin__();
        if (s != -1) {
            c = (char)s;

            if (c == CARRIAGE_RETURN || c == NEW_LINE) {
                line_buff[count] = END_OF_LINE;
                __cout__(NEW_LINE);
                break;
            }

            if (c == DELETE || c == BACK_SPACE) {
                if (!__echo) {
                    delete ();
                    delete ();
                }

                if (count == 0)
                    continue;

                count--;

                line_buff[count] = END_OF_LINE;
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
                if (!__echo) {
                    clear_prompt(count + 4);
                } else {
                    clear_prompt(count);
                }

                if (c == 'A') {
                    handle_up_arrow(line_buff, &count);
                } else {
                    handle_down_arrow(line_buff, &count);
                }
                special_key = 0;
                continue;
            } else if (c == TAB) {
                handle_tab(line_buff, &count);
                continue;
            } else {
                line_buff[count] = c;
                count++;
            }

            if (__echo && c != DELETE && c != BACK_SPACE)
                __cout__(c);
        } else
            loop();
    }

    add_command_to_history(line_buff);

    argc = parse_line(argv, line_buff, MAX_ARG_COUNT);

    if (argc > 0)
        execute(argc, argv);
}

static void exec_auto_cmds(void) {
    int i;

    for (i = 0; cmd_auto[i].name != NULL; i++)
        cmd_auto[i].xcall(0, NULL);
}

static int build_info(int argc, char **argv) {
    tiny_print("Build: [" SHELL_VERSION ":" USER_REPO_VERSION "] - [" BUILD_USER
               "@" BUILD_HOST "] - " __DATE__ " - " __TIME__ "\n");
    return 0;
}

int console_init(void) {
    exec_auto_cmds();
    setup();

    while (1)
        shell();

    /* shall never reach when console enabled */
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

xmd get_function_addr(char *cmd_str) {
    int i;

    for (i = 0; cmd_list[i].name != NULL; i++) {
        if (!strcmp(cmd_str, cmd_list[i].name))
            return cmd_list[i].xcall;
    }

    return NULL;
}

int help(int argc, char **argv) {
    int i = 0;
    bool verbose = true;

    if (argc > 1 && (!strcmp(argv[1], "-l")))
        verbose = false;
    else
        tiny_print("use: help -l for list only.\n\n");

    while (cmd_list[i].name != NULL) {
        tiny_print(cmd_list[i].name);

        if (verbose) {
            tiny_print("\n\t");
            tiny_print(cmd_list[i].help);
        }

        tiny_print("\n");

        i++;
    }

    return 0;
}

int cmd_exec_status(int argc, char **argv) {
    tiny_print("%d\n", __cmd_exec_status);

    return 0;
}

PUSH_CMD_AUTO(version, "Prints details of the build", build_info);
PUSH_CMD_LIST(help, "Prints all available commands", help);
PUSH_CMD_LIST(status, "Returns exit status of last executed command",
        cmd_exec_status);

__attribute__((__section__(".cmd_end"))) static volatile cmd_t noop_cmd = {
    NULL, NULL, NULL,
};
