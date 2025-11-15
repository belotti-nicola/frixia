#include <sys/inotify.h>

/*
 * https://man7.org/linux/man-pages/man7/inotify.7.html
 * THIS IMPLEMENTATION IS OVERKILL BUT IN FUTURE MIGHT BE HANDY AND HELPED ME TO DO THINGS WITH ENUMS
*/



typedef enum FRIXIA_INODE_FLAG 
{
    FINODE_ACCESS          = (1<<0),
    FINODE_MODIFY          = (1<<1),
    FINODE_ATTRIB          = (1<<2),
    FINODE_CLOSE_WRITE     = (1<<3),
    FINODE_CLOSE_NOWRITE   = (1<<4),
    FINODE_CLOSE           =  FINODE_CLOSE_NOWRITE | FINODE_CLOSE_WRITE,
    FINODE_OPEN            = (1<<5),
    FINODE_MOVED_FROM      = (1<<6),
    FINODE_MOVED_TO        = (1<<7),
    FINODE_MOVE            = FINODE_MOVED_FROM | FINODE_MOVED_TO,
    FINODE_CREATE          = (1<<8),
    FINODE_DELETE          = (1<<9),
    FINODE_DELETE_SELF     = (1<<10),
    FINODE_MOVE_SELF       = (1<<11),
    FINODE_UNMOUNT         = (1<<12),
    FINODE_Q_OVERFLOW      = (1<<13),
    FINODE_IGNORED         = (1<<14),
    FINODE_ONLYDIR         = (1<<15),
    FINODE_DONT_FOLLOW     = (1<<25),
    FINODE_EXCL_UNLINK     = (1<<26),
    FINODE_MASK_CREATE     = (1<<28),
    FINODE_MASK_ADD        = (1<<29),
    FINODE_ISDIR           = (1<<30),
    FINODE_ONESHOT         = (1<<31),

} FRIXIA_INODE_FLAG_T;