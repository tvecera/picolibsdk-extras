#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

// Dummy _close implementation
int _close(int file) {
    errno = EBADF;
    return -1;
}

// Dummy _fstat implementation
int _fstat(int file, struct stat *st) {
    errno = ENOSYS;
    return -1;
}

// Dummy _getpid implementation
pid_t _getpid(void) {
    return 1;
}

// Dummy _isatty implementation
int _isatty(int file) {
    return 0;
}

// Dummy _kill implementation
int _kill(pid_t pid, int sig) {
    errno = ENOSYS;
    return -1;
}

// Dummy _lseek implementation
off_t _lseek(int file, off_t offset, int whence) {
    return (off_t)-1;
}

// Dummy _read implementation
ssize_t _read(int file, void *ptr, size_t len) {
    return 0;
}

// Dummy _write implementation
ssize_t _write(int file, const void *ptr, size_t len) {
    return len;
}
