#include <sys/inotify.h>


//https://man7.org/linux/man-pages/man7/inotify.7.html

typedef enum FRIXIA_INODE_FLAG 
{
    FINODE_ACCESS          = (1<<0),
    FINODE_MODIFY          = (1<<1),
    FINODE_ATTRIB          = (1<<2),
    FINODE_CLOSE_WRITE     = (1<<3),
    FINODE_CLOSE_NOWRITE   = (1<<4),
    FINODE_CLOSE           = (1<<5),
    FINODE_OPEN            = (1<<6),
    FINODE_MOVED_FROM      = (1<<7),
    FINODE_MOVED_TO        = (1<<8),
    FINODE_MOVE            = (1<<9),
    FINODE_CREATE          = (1<<10),
    FINODE_DELETE          = (1<<11),
    FINODE_DELETE_SELF     = (1<<12),
    FINODE_MOVE_SELF       = (1<<13),
    FINODE_UNMOUNT         = (1<<14),
    FINODE_Q_OVERFLOW      = (1<<15),
    FINODE_IGNORED         = (1<<16),
    FINODE_ONLYDIR         = (1<<17),
    FINODE_DONT_FOLLOW     = (1<<18),
    FINODE_EXCL_UNLINK     = (1<<19),
    FINODE_MASK_CREATE     = (1<<20),
    FINODE_MASK_ADD        = (1<<21),
    FINODE_ISDIR           = (1<<22),
    FINODE_ONESHOT         = (1<<23),

    FINODE_ALL_EVENTS      = 0xFFFFFFFF,

} FRIXIA_INODE_FLAG_T;

static int FRIXIA_TO_INODE_FLAG(FRIXIA_INODE_FLAG_T flag)
{
    switch(flag)
    {
        case FINODE_ACCESS        : return IN_ACCESS;
        case FINODE_MODIFY        : return IN_MODIFY;
        case FINODE_ATTRIB        : return IN_ATTRIB;
        case FINODE_CLOSE_NOWRITE : return IN_CLOSE_NOWRITE;
        case FINODE_CLOSE         : return IN_CLOSE;
        case FINODE_OPEN          : return IN_OPEN;
        case FINODE_MOVED_FROM    : return IN_MOVED_FROM;
        case FINODE_MOVED_TO      : return IN_MOVED_TO;
        case FINODE_MOVE          : return IN_MOVE;
        case FINODE_CREATE        : return IN_CREATE;
        case FINODE_DELETE        : return IN_DELETE;
        case FINODE_DELETE_SELF   : return IN_DELETE_SELF;
        case FINODE_MOVE_SELF     : return IN_MOVE_SELF;
        case FINODE_UNMOUNT       : return IN_UNMOUNT;
        case FINODE_Q_OVERFLOW    : return IN_Q_OVERFLOW;
        case FINODE_IGNORED       : return IN_IGNORED;
        case FINODE_ONLYDIR       : return IN_ONLYDIR;
        case FINODE_DONT_FOLLOW   : return IN_DONT_FOLLOW;
        case FINODE_EXCL_UNLINK   : return IN_EXCL_UNLINK;
        case FINODE_MASK_CREATE   : return IN_MASK_CREATE;
        case FINODE_MASK_ADD      : return IN_MASK_ADD;
        case FINODE_ISDIR         : return IN_ISDIR;
        case FINODE_ONESHOT       : return IN_ONESHOT;
        case FINODE_ALL_EVENTS    : return IN_ALL_EVENTS;
    }
}

