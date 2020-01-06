# clog-generic
An updated generic version of the original Clog written in C++.

## How to use

This includes two programs, clogd and clog.

clogd reads from its standard input and writes to a circular buffer
in POSIX shared memory given by the command line:

clogd /shared-path/logfile

The clog program is used to read this output:

clog /shared-path/logfile

Without any flags, clog reads and outputs the current buffer contents, then 
exits.

The following flags are supported by clog:

* -f = Read the buffer contents, then wait for more data to arrive, displaying the result indefinitely.  (Similar to tail -n 9999999999 -f.)

* -z = Ignore the existing buffer contents, then wait for new data to arrive, displaying the result indefinitely.  (Similar to tail -n 0 f.)

* -r = Remove a shared memory log and then exit.


