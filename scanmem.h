/*
    Provide interfaces for front-ends.

    This file is part of libscanmem.

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCANMEM_H
#define SCANMEM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#include "scanroutines.h"
#include "list.h"
#include "value.h"
#include "target_memory_info_array.h"


#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "(unknown)"
#endif

/* from string.h in glibc for Android/BSD */
#ifndef strdupa
#define strdupa(s)                                                            \
    ({                                                                        \
      const char *__old = (s);                                                \
      size_t __len = strlen(__old) + 1;                                       \
      char *__new = (char *) alloca(__len);                                   \
      (char *) memcpy(__new, __old, __len);                                   \
    })
#endif

#ifndef strndupa
#define strndupa(s, n)                                                        \
    ({                                                                        \
      const char *__old = (s);                                                \
      size_t __len = strnlen(__old, (n));                                     \
      char *__new = (char *) alloca(__len + 1);                               \
      __new[__len] = '\0';                                                    \
      (char *) memcpy(__new, __old, __len);                                   \
    })
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

/* global settings */
typedef struct {
    unsigned exit:1;
    pid_t target;
    matches_and_old_values_array *matches;
    long num_matches;
    double scan_progress;
    list_t *regions;
    list_t *commands;              /* command handlers */
    const char *current_cmdline;   /* the command being executed */
    void (*printversion)(FILE *outfd);
    struct {
        unsigned short alignment;
        unsigned short debug;
        unsigned short backend;    /* if 1, scanmem will work as a backend and
                                      output would be more machine-readable */

        /* options that can be changed during runtime */
        scan_data_type_t scan_data_type;
        region_scan_level_t region_scan_level;
        unsigned short detect_reverse_change;
        unsigned short dump_with_ascii;
        unsigned short reverse_endianness;
    } options;
} globals_t;

/* global settings */
extern globals_t sm_globals;

bool sm_init();
void sm_printversion(FILE *outfd);
void sm_set_backend();
void sm_backend_exec_cmd(const char *commandline);
long sm_get_num_matches();
const char *sm_get_version();
double sm_get_scan_progress();
void sm_reset_scan_progress();

/* ptrace.c */
bool sm_detach(pid_t target);
bool sm_setaddr(pid_t target, void *addr, const value_t *to);
bool sm_checkmatches(globals_t *vars, scan_match_type_t match_type,
                     const uservalue_t *uservalue);
bool sm_searchregions(globals_t *vars, scan_match_type_t match_type,
                      const uservalue_t *uservalue);
bool sm_peekdata(pid_t pid, void *addr, value_t *result);
bool sm_attach(pid_t target);
bool sm_read_array(pid_t target, void *addr, char *buf, int len);
bool sm_write_array(pid_t target, void *addr, const void *data, int len);

/* menu.c */
bool sm_getcommand(globals_t *vars, char **line);

#endif /* SCANMEM_H */
