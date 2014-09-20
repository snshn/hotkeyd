/*
    hotkeyd.c

    Forked from Benjamin Bolton's keyd
    license: GPLv3
*/

#include <linux/input.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int is_white_space(int c)
{
    switch(c) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\v':
        case '\f':
        case 0:
            return 1;
    }

    return 0;
}

#include "log.h"
#include "keys.h"
#include "input-dev.h"

#define DEF_CONFIG     "/etc/hotkeyd.conf"
#define CALL_BIN       "/bin/sh"
#define COUNT_MODS     9

struct hot_key {
    int    key,mods;
    char  *command;
    struct hot_key *next;
};

struct hot_key *first;
int verbose_flag = 1;

void new_hot_key(const char *txt)
{
    struct hot_key *tmp;
    int i;

    if(!txt) {
        return;
    }

    if(first) {
        tmp = first;
        while(tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = (struct hot_key*)malloc(sizeof(struct hot_key));
        tmp = tmp->next;
        if(!tmp) {
            log_err("warning: Failed to allocate hot_key pointer\n");
            return;
        }
    } else {
        tmp = (struct hot_key*)malloc(sizeof(struct hot_key));
        first = tmp;
        if(!tmp) {
            log_err("warning: Failed to allocate hot_key pointer\n");
            return;
        }
    }

    tmp->next = NULL;
    tmp->mods = 0;

    while(!is_white_space(txt[0])) {
        i = get_mod_value(&txt);

        if(i == 0) {
            break;
        }

        tmp->mods |= i;

        if(!is_white_space(txt[0])) {
            txt++;
        }
    }

    tmp->key = get_key_value(&txt);

    while(!is_white_space(txt[0])) txt++;

    while(txt[0] != 0 && is_white_space(txt[0])) txt++;

    i = strlen(txt)+1;
    tmp->command = (char*)malloc(sizeof(char) * i);
    tmp->command[0] = 0;

    strcpy(tmp->command, txt);

    while(is_white_space(tmp->command[i])) {
        tmp->command[i] = 0;
        i--;
    }

    if(verbose_flag) {
        log_msg("New command \"%s\" with key %s value %i and mods %i:\n", tmp->command, get_key_name(tmp->key), tmp->key, tmp->mods);
        for(i = 0; i < COUNT_MODS; i++) {
            if((tmp->mods >> i) & 1) {
                log_msg("  %s value %i\n", mod_mapped[i].name, mod_mapped[i].map);
            }
        }
    }
}

struct hot_key *get_hot_key(int key, int mods)
{
    struct hot_key *tmp = first;
    int i, c;

    if (key < 1) {
        return NULL;
    }

    while(tmp) {
        if (key == tmp->key && mods == tmp->mods) {
            return tmp;
        }
        tmp = tmp->next;
    }

    return NULL;
}

static struct option long_opts[] = {
    {"help",      no_argument,        0, 'h'},
    {"input",     required_argument,  0, 'i'},
    {"config",    required_argument,  0, 'c'},
    {"test",      no_argument,        0, 't'},
    {"quiet",     no_argument,        0, 'q'},
    {"output",    required_argument,  0, 'o'},
    {0,           0,                  0, 0}
};

void on_close(int sig)
{
    struct hot_key *tmp = first, *tmp2;
    first = NULL;

    if(verbose_flag) {
        log_msg("\nCleaning up\n");
    }

    while(tmp) {
        tmp2 = tmp->next;

        if(tmp->command){
            free(tmp->command);
        }

        free(tmp);

        tmp = tmp2;
    }

    exit(0);
}

void run_command(const char *command)
{
    pid_t pID = fork();

    if(pID == 0) {
        execl(CALL_BIN, CALL_BIN, "-c", command, NULL);
        exit(-1);
    } else if(pID < 0) {
        log_err("Failed to fork process!\n");
    }
}

int main(int argc, char *argv[])
{
    int input_stream, test_flag = 0, c, opt_index, mods = 0, i, j, free_input = 0;
    FILE *fp;
    char *input = NULL;
    input_stream = open(input, O_RDONLY);
    const char *config = DEF_CONFIG, *tmpc;
    char *line = NULL, *tmp;
    size_t len = 0;
    first = NULL;
    struct hot_key *hk;
    struct input_event ev;

    while(1) {
        c = getopt_long(argc, argv, "i:c:tqh", long_opts, &opt_index);

        if(c == -1) {
            break;
        }

        switch(c) {
            case 'i':
                input = optarg;
                break;

            case 'c':
                config = optarg;
                break;

            case 't':
                test_flag = 1;
                break;

            case 'q':
                verbose_flag = 0;
                break;

            case 'h':
            case '?':
                printf("usage: %s [options]\n\nOptions:\n -h --help      Show this help\n -i --input     Input to connect to\n", argv[0]);
                printf(" -c --config    Config file with hotkeys\n                Default: %s\n -t --test      Use to find the name's to use for keys\n", DEF_CONFIG);
                printf("                Note: does not load config file\n -q --quiet     Do not output messages\n\n");
                return (c != 'h');
        }
    }

    if(!test_flag) {
        fp = fopen(config, "r");

        if(!fp) {
            log_err("Failed to open config file %s\n", config);
            return 1;
        }

        while(getline(&line, &len, fp) != -1) {
            if(line) {
                tmp = line;

                while(tmp[0] != 0 && is_white_space(tmp[0])) tmp++;

                if(tmp[0] != 0 && tmp[0] != '#')
                    new_hot_key(tmp);
            }
        }

        if(line) free(line);

        fclose(fp);
        fp = NULL;
    }
    
    signal(SIGINT,  on_close);
    signal(SIGTERM, on_close);
    signal(SIGCHLD, SIG_IGN);
    
    if(!input) {
        input = default_device();

        if(!input) {
            log_err("Failed to determine default device!\n");
            return -1;
        } else {
            free_input = 1;
        }
    }

    input_stream = open(input, O_RDONLY);

    if(input_stream == -1) {
        log_err("Failed to open %s\n", input);
        if(free_input) {
            free(input);
        }
        return 1;
    } else {
        log_msg("Successfully opened %s\n", input);
    }

    if(free_input) {
        free(input);
    }

    if(test_flag) {
        log_msg("Press ctrl+c to close:\n");
    }

    while(read(input_stream, &ev, sizeof(struct input_event)) > 0)
    {
        if(ev.type != EV_KEY) continue;

        if(ev.value == 1 || ev.value == 2) {
            i = get_mod_value_from_map(ev.code);

            if(i > 0) {
                mods |= i;
            } else if(test_flag) {
                log_msg("Key name: ");

                j = 0;

                for(i = 0; i < COUNT_MODS; i++) {
                    if((mods >> i) & 1) {
                        if(j) {
                            log_msg("+%s", mod_mapped[i].name);
                        } else {
                            log_msg("%s", mod_mapped[i].name);
                        }

                        j = 1;
                    }
                }

                if(j) {
                    log_msg("+%s\n", get_key_name(ev.code));
                } else {
                    log_msg("%s\n", get_key_name(ev.code));
                }
            } else {
                hk = get_hot_key(ev.code, mods);

                if(hk) {
                    if(verbose_flag) {
                        log_msg("Match running command \"%s\"\n", hk->command);
                    }

                    run_command(hk->command);
                }
            }
        } else if(ev.value == 0){
            i = get_mod_value_from_map(ev.code);

            if(i > 0) mods &= ~i;
        }
        
    }

    on_close(0);
}
