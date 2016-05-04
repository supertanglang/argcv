/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#include "argcv/sys/daemon.h"

#include <unistd.h>  // setsid
#include <syslog.h>    // syslog
#include <signal.h>    // signal
#include <sys/stat.h>  // open O_RDONL
#include <fcntl.h>     // open

int daemon_proc = 0;  // for err_XXX() functions
char proj_work_dir[1024] = "/";
int max_fd = 64;

int daemon_init(const char* ident) {
    openlog(ident, LOG_PID, LOG_USER);
    // openlog(ident,LOG_CONS|LOG_PID,LOG_USER);
    syslog(LOG_INFO, "daemon starting");
    int i;
    pid_t pid;

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);  // parent terminates
    }

    // child 1 continues ...
    if (setsid() < 0)  // become session leader
    {
        return -1;
    }

    signal(SIGHUP, SIG_IGN);

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);
    }
    // child 2 continues ...
    daemon_proc = 1;  // for err_XXX() functions

    syslog(LOG_INFO, "chdir: %s\n", proj_work_dir);

    chdir(proj_work_dir);

    // close off file descriptors
    for (i = 0; i < max_fd; i++) {
        close(i);
    }

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    syslog(LOG_INFO, "daemon started");
    return 0;
}

void daemon_destroy() {
    syslog(LOG_INFO, "daemon stopped");
    closelog();
}
