/************************************************************************** 
 * Portions of the following code were lifted from xf86drm.c, so we include
 * this copyright notice...
 **************************************************************************/
/*
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 *
 * Copyright 2000 Hewlett-Packard Company, Ft. Collins, Colorado.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h> 
#include <unistd.h> 
#include <string.h> 
#include <ctype.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <signal.h> 
#include <assert.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/sysmacros.h>  /* for makedev() */
#include "hpgfx_utils.h"
#include "linuxkernel/drm/drm.h"
#include <sys/framebuf.h>


/********************************************************* 
 * Code lifted directly from xf86drm.c [ 
 *********************************************************/
#define _DRM_MALLOC malloc
#define _DRM_FREE   free


static void *drmHashTable = NULL; /* Context switch callbacks */

typedef struct drmHashEntry {
    int      fd;
    void     (*f)(int, void *, void *);
    void     *tagTable;
} drmHashEntry;

void *drmMalloc(int size)
{
    void *pt;
    if ((pt = _DRM_MALLOC(size))) memset(pt, 0, size);
    return pt;
}

void drmFree(void *pt)
{
    if (pt) _DRM_FREE(pt);
}

static char *drmStrdup(const char *s)
{
    return s ? strdup(s) : NULL;
}


static unsigned long drmGetKeyFromFd(int fd)
{
    struct stat     st;

    st.st_rdev = 0;
    fstat(fd, &st);
    return st.st_rdev;
}

static drmHashEntry *drmGetEntry(int fd)
{
    unsigned long key = drmGetKeyFromFd(fd);
    void          *value;
    drmHashEntry  *entry;

    if (!drmHashTable) drmHashTable = drmHashCreate();

    if (drmHashLookup(drmHashTable, key, &value)) {
    entry           = drmMalloc(sizeof(*entry));
    entry->fd       = fd;
    entry->f        = NULL;
    entry->tagTable = drmHashCreate();
    drmHashInsert(drmHashTable, key, entry);
    } else {
    entry = value;
    }
    return entry;
}

/* drmAvailable looks for /proc/drm, and returns 1 if it is present. */

int drmAvailable(void)
{
    if (!access("/proc/dri/0", R_OK)) return 1;
    return 0;
}


void drmFreeVersion(drmVersionPtr v)
{
    if (!v) return;
    if (v->name) drmFree(v->name);
    if (v->date) drmFree(v->date);
    if (v->desc) drmFree(v->desc);
    drmFree(v);
}

static void drmFreeKernelVersion(drm_version_t *v)
{
    if (!v) return;
    if (v->name) drmFree(v->name);
    if (v->date) drmFree(v->date);
    if (v->desc) drmFree(v->desc);
    drmFree(v);
}

static void drmCopyVersion(drmVersionPtr d, drm_version_t *s)
{
    d->version_major      = s->version_major;
    d->version_minor      = s->version_minor;
    d->version_patchlevel = s->version_patchlevel;
    d->name_len           = s->name_len;
    d->name               = drmStrdup(s->name);
    d->date_len           = s->date_len;
    d->date               = drmStrdup(s->date);
    d->desc_len           = s->desc_len;
    d->desc               = drmStrdup(s->desc);
}

/* drmVersion obtains the version information via an ioctl.  Similar
 * information is available via /proc/drm. */

drmVersionPtr drmGetVersion(int fd)
{
    drmVersionPtr retval;
    drm_version_t *version = drmMalloc(sizeof(*version));

                /* First, get the lengths */
    version->name_len    = 0;
    version->name        = NULL;
    version->date_len    = 0;
    version->date        = NULL;
    version->desc_len    = 0;
    version->desc        = NULL;
    
    if (ioctl(fd, DRM_IOCTL_VERSION, version)) {
    drmFreeKernelVersion(version);
    return NULL;
    }

                /* Now, allocate space and get the data */
    if (version->name_len)
    version->name    = drmMalloc(version->name_len + 1);
    if (version->date_len)
    version->date    = drmMalloc(version->date_len + 1);
    if (version->desc_len)
    version->desc    = drmMalloc(version->desc_len + 1);
    
    if (ioctl(fd, DRM_IOCTL_VERSION, version)) {
    drmFreeKernelVersion(version);
    return NULL;
    }

                /* The results might not be null-terminated
                                   strings, so terminate them. */

    if (version->name_len) version->name[version->name_len] = '\0';
    if (version->date_len) version->date[version->date_len] = '\0';
    if (version->desc_len) version->desc[version->desc_len] = '\0';

                /* Now, copy it all back into the
                                   client-visible data structure... */
    retval = drmMalloc(sizeof(*retval));
    drmCopyVersion(retval, version);
    drmFreeKernelVersion(version);
    return retval;
}

void drmFreeBusid(const char *busid)
{
    drmFree((void *)busid);
}

char *drmGetBusid(int fd)
{
    drm_unique_t u;

    u.unique_len = 0;
    u.unique     = NULL;

    if (ioctl(fd, DRM_IOCTL_GET_UNIQUE, &u)) return NULL;
    u.unique = drmMalloc(u.unique_len + 1);
    if (ioctl(fd, DRM_IOCTL_GET_UNIQUE, &u)) return NULL;
    u.unique[u.unique_len] = '\0';
    return u.unique;
}

int drmSetBusid(int fd, const char *busid)
{
    drm_unique_t u;

    u.unique     = (char *)busid;
    u.unique_len = strlen(busid);

    if (ioctl(fd, DRM_IOCTL_SET_UNIQUE, &u)) return -errno;
    return 0;
}

int drmGetMagic(int fd, drmMagicPtr magic)
{
    drm_auth_t auth;

    *magic = 0;
    if (ioctl(fd, DRM_IOCTL_GET_MAGIC, &auth)) return -errno;
    *magic = auth.magic;
    return 0;
}

int drmAuthMagic(int fd, drmMagic magic)
{
    drm_auth_t auth;

    auth.magic = magic;
    if (ioctl(fd, DRM_IOCTL_AUTH_MAGIC, &auth)) return -errno;
    return 0;
}

int drmAddMap(int fd,
          drmHandle offset,
          drmSize size,
          drmMapType type,
          drmMapFlags flags,
          drmHandlePtr handle)
{
    drm_map_t map;

    map.offset  = offset;
    map.size    = size;
    map.handle  = 0;
    map.type    = type;
    map.flags   = flags;
    if (ioctl(fd, DRM_IOCTL_ADD_MAP, &map)) return -errno;
    if (handle) *handle = (drmHandle)map.handle;
    return 0;
}

int drmAddBufs(int fd, int count, int size, int flags)
{
    drm_buf_desc_t request;
    
    request.count     = count;
    request.size      = size;
    request.low_mark  = 0;
    request.high_mark = 0;
    request.flags     = flags;
    if (ioctl(fd, DRM_IOCTL_ADD_BUFS, &request)) return -errno;
    return request.count;
}

int drmMarkBufs(int fd, double low, double high)
{
    drm_buf_info_t info;
    int            i;

    info.count = 0;
    info.list  = NULL;

    if (ioctl(fd, DRM_IOCTL_INFO_BUFS, &info)) return -EINVAL;

    if (!info.count) return -EINVAL;
    
    if (!(info.list = drmMalloc(info.count * sizeof(*info.list))))
    return -ENOMEM;
    
    if (ioctl(fd, DRM_IOCTL_INFO_BUFS, &info)) {
    int retval = -errno;
    drmFree(info.list);
    return retval;
    }
    
    for (i = 0; i < info.count; i++) {
    info.list[i].low_mark  = low  * info.list[i].count;
    info.list[i].high_mark = high * info.list[i].count;
    if (ioctl(fd, DRM_IOCTL_MARK_BUFS, &info.list[i])) {
        int retval = -errno;
        drmFree(info.list);
        return retval;
    }
    }
    drmFree(info.list);
    
    return 0;
}

int drmFreeBufs(int fd, int count, int *list)
{
    drm_buf_free_t request;

    request.count = count;
    request.list  = list;
    if (ioctl(fd, DRM_IOCTL_FREE_BUFS, &request)) return -errno;
    return 0;
}

int drmClose(int fd)
{
    unsigned long key    = drmGetKeyFromFd(fd);
    drmHashEntry  *entry = drmGetEntry(fd);

    drmHashDestroy(entry->tagTable);
    entry->fd       = 0;
    entry->f        = NULL;
    entry->tagTable = NULL;

    drmHashDelete(drmHashTable, key);
    drmFree(entry);

    return close(fd);
}

int drmMap(int fd,
       drmHandle handle,
       drmSize size,
       drmAddressPtr address)
{
    if (fd < 0) return -EINVAL;
    *address = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, handle);
    if (*address == MAP_FAILED) return -errno;
    return 0;
}

int drmUnmap(drmAddress address, drmSize size)
{
    return munmap(address, size);
}

drmBufInfoPtr drmGetBufInfo(int fd)
{
    drm_buf_info_t info;
    drmBufInfoPtr  retval;
    int            i;

    info.count = 0;
    info.list  = NULL;

    if (ioctl(fd, DRM_IOCTL_INFO_BUFS, &info)) return NULL;

    if (info.count) {
    if (!(info.list = drmMalloc(info.count * sizeof(*info.list))))
        return NULL;
    
    if (ioctl(fd, DRM_IOCTL_INFO_BUFS, &info)) {
        drmFree(info.list);
        return NULL;
    }
                /* Now, copy it all back into the
                                   client-visible data structure... */
    retval = drmMalloc(sizeof(*retval));
    retval->count = info.count;
    retval->list  = drmMalloc(info.count * sizeof(*retval->list));
    for (i = 0; i < info.count; i++) {
        retval->list[i].count     = info.list[i].count;
        retval->list[i].size      = info.list[i].size;
        retval->list[i].low_mark  = info.list[i].low_mark;
        retval->list[i].high_mark = info.list[i].high_mark;
    }
    drmFree(info.list);
    return retval;
    }
    return NULL;
}

drmBufMapPtr drmMapBufs(int fd)
{
    drm_buf_map_t bufs;
    drmBufMapPtr  retval;
    int           i;
    
    bufs.count = 0;
    bufs.list  = NULL;
    if (ioctl(fd, DRM_IOCTL_MAP_BUFS, &bufs)) return NULL;

    if (bufs.count) {
    if (!(bufs.list = drmMalloc(bufs.count * sizeof(*bufs.list))))
        return NULL;

    if (ioctl(fd, DRM_IOCTL_MAP_BUFS, &bufs)) {
        drmFree(bufs.list);
        return NULL;
    }
                /* Now, copy it all back into the
                                   client-visible data structure... */
    retval = drmMalloc(sizeof(*retval));
    retval->count = bufs.count;
    retval->list  = drmMalloc(bufs.count * sizeof(*retval->list));
    for (i = 0; i < bufs.count; i++) {
        retval->list[i].idx     = bufs.list[i].idx;
        retval->list[i].total   = bufs.list[i].total;
        retval->list[i].used    = 0;
        retval->list[i].address = bufs.list[i].address;
    }
    return retval;
    }
    return NULL;
}

int drmUnmapBufs(drmBufMapPtr bufs)
{
    int i;
    
    for (i = 0; i < bufs->count; i++) {
    munmap(bufs->list[i].address, bufs->list[i].total);
    }
    return 0;
}

int drmDMA(int fd, drmDMAReqPtr request)
{
    drm_dma_t dma;

                /* Copy to hidden structure */
    dma.context         = request->context;
    dma.send_count      = request->send_count;
    dma.send_indices    = request->send_list;
    dma.send_sizes      = request->send_sizes;
    dma.flags           = request->flags;
    dma.request_count   = request->request_count;
    dma.request_size    = request->request_size;
    dma.request_indices = request->request_list;
    dma.request_sizes   = request->request_sizes;
    if (ioctl(fd, DRM_IOCTL_DMA, &dma))
        return -errno;
    request->granted_count = dma.granted_count;
    
    return 0;
}

int drmGetLock(int fd, drmContext context, drmLockFlags flags)
{
    drm_lock_t lock;

    lock.context = context;
    lock.flags   = 0;
    if (flags & DRM_LOCK_READY)      lock.flags |= _DRM_LOCK_READY;
    if (flags & DRM_LOCK_QUIESCENT)  lock.flags |= _DRM_LOCK_QUIESCENT;
    if (flags & DRM_LOCK_FLUSH)      lock.flags |= _DRM_LOCK_FLUSH;
    if (flags & DRM_LOCK_FLUSH_ALL)  lock.flags |= _DRM_LOCK_FLUSH_ALL;
    if (flags & DRM_HALT_ALL_QUEUES) lock.flags |= _DRM_HALT_ALL_QUEUES;
    if (flags & DRM_HALT_CUR_QUEUES) lock.flags |= _DRM_HALT_CUR_QUEUES;
    
    while (ioctl(fd, DRM_IOCTL_LOCK, &lock))
    ;
    return 0;
}

int drmUnlock(int fd, drmContext context)
{
    drm_lock_t lock;

    lock.context = context;
    lock.flags   = 0;
    return ioctl(fd, DRM_IOCTL_UNLOCK, &lock);
}

drmContextPtr drmGetReservedContextList(int fd, int *count)
{
    drm_ctx_res_t res;
    drm_ctx_t     *list;
    drmContextPtr retval;
    int           i;

    res.count    = 0;
    res.contexts = NULL;
    if (ioctl(fd, DRM_IOCTL_RES_CTX, &res)) return NULL;

    if (!res.count) return NULL;

    if (!(list   = drmMalloc(res.count * sizeof(*list)))) return NULL;
    if (!(retval = drmMalloc(res.count * sizeof(*retval)))) {
    drmFree(list);
    return NULL;
    }

    res.contexts = list;
    if (ioctl(fd, DRM_IOCTL_RES_CTX, &res)) return NULL;

    for (i = 0; i < res.count; i++) retval[i] = list[i].handle;
    drmFree(list);

    *count = res.count;
    return retval;
}

void drmFreeReservedContextList(drmContextPtr pt)
{
    drmFree(pt);
}

int drmCreateContext(int fd, drmContextPtr handle)
{
    drm_ctx_t ctx;

    ctx.flags = 0;  /* Modified with functions below */
    if (ioctl(fd, DRM_IOCTL_ADD_CTX, &ctx)) return -errno;
    *handle = ctx.handle;
    return 0;
}

int drmSwitchToContext(int fd, drmContext context)
{
    drm_ctx_t ctx;

    ctx.handle = context;
    if (ioctl(fd, DRM_IOCTL_SWITCH_CTX, &ctx)) return -errno;
    return 0;
}

int drmSetContextFlags(int fd, drmContext context, drmContextFlags flags)
{
    drm_ctx_t ctx;

                /* Context preserving means that no context
                                   switched are done between DMA buffers
                                   from one context and the next.  This is
                                   suitable for use in the X server (which
                                   promises to maintain hardware context,
                                   or in the client-side library when
                                   buffers are swapped on behalf of two
                                   threads. */
    ctx.handle = context;
    ctx.flags  = 0;
    if (flags & DRM_CONTEXT_PRESERVED) ctx.flags |= _DRM_CONTEXT_PRESERVED;
    if (flags & DRM_CONTEXT_2DONLY)    ctx.flags |= _DRM_CONTEXT_2DONLY;
    if (ioctl(fd, DRM_IOCTL_MOD_CTX, &ctx)) return -errno;
    return 0;
}

int drmGetContextFlags(int fd, drmContext context, drmContextFlagsPtr flags)
{
    drm_ctx_t ctx;

    ctx.handle = context;
    if (ioctl(fd, DRM_IOCTL_GET_CTX, &ctx)) return -errno;
    *flags = 0;
    if (ctx.flags & _DRM_CONTEXT_PRESERVED) *flags |= DRM_CONTEXT_PRESERVED;
    if (ctx.flags & _DRM_CONTEXT_2DONLY)    *flags |= DRM_CONTEXT_2DONLY;
    return 0;
}
    
int drmDestroyContext(int fd, drmContext handle)
{
    drm_ctx_t ctx;
    ctx.handle = handle;
    if (ioctl(fd, DRM_IOCTL_RM_CTX, &ctx)) return -errno;
    return 0;
}

int drmCreateDrawable(int fd, drmDrawablePtr handle)
{
    drm_draw_t draw;
    if (ioctl(fd, DRM_IOCTL_ADD_DRAW, &draw)) return -errno;
    *handle = draw.handle;
    return 0;
}

int drmDestroyDrawable(int fd, drmDrawable handle)
{
    drm_draw_t draw;
    draw.handle = handle;
    if (ioctl(fd, DRM_IOCTL_RM_DRAW, &draw)) return -errno;
    return 0;
}

int drmError(int err, const char *label)
{
    switch (err) {
    case DRM_ERR_NO_DEVICE: fprintf(stderr, "%s: no device\n", label);   break;
    case DRM_ERR_NO_ACCESS: fprintf(stderr, "%s: no access\n", label);   break;
    case DRM_ERR_NOT_ROOT:  fprintf(stderr, "%s: not root\n", label);    break;
    case DRM_ERR_INVALID:   fprintf(stderr, "%s: invalid args\n", label);break;
    default:
    if (err < 0) err = -err;
    fprintf( stderr, "%s: error %d (%s)\n", label, err, strerror(err) );
    break;
    }

    return 1;
}

int drmCtlInstHandler(int fd, int irq)
{
    drm_control_t ctl;

    ctl.func  = DRM_INST_HANDLER;
    ctl.irq   = irq;
    if (ioctl(fd, DRM_IOCTL_CONTROL, &ctl)) return -errno;
    return 0;
}

int drmCtlUninstHandler(int fd)
{
    drm_control_t ctl;

    ctl.func  = DRM_UNINST_HANDLER;
    ctl.irq   = 0;
    if (ioctl(fd, DRM_IOCTL_CONTROL, &ctl)) return -errno;
    return 0;
}

int drmFinish(int fd, int context, drmLockFlags flags)
{
    drm_lock_t lock;

    lock.context = context;
    lock.flags   = 0;
    if (flags & DRM_LOCK_READY)      lock.flags |= _DRM_LOCK_READY;
    if (flags & DRM_LOCK_QUIESCENT)  lock.flags |= _DRM_LOCK_QUIESCENT;
    if (flags & DRM_LOCK_FLUSH)      lock.flags |= _DRM_LOCK_FLUSH;
    if (flags & DRM_LOCK_FLUSH_ALL)  lock.flags |= _DRM_LOCK_FLUSH_ALL;
    if (flags & DRM_HALT_ALL_QUEUES) lock.flags |= _DRM_HALT_ALL_QUEUES;
    if (flags & DRM_HALT_CUR_QUEUES) lock.flags |= _DRM_HALT_CUR_QUEUES;
    if (ioctl(fd, DRM_IOCTL_FINISH, &lock)) return -errno;
    return 0;
}

int drmGetInterruptFromBusID(int fd, int busnum, int devnum, int funcnum)
{
    drm_irq_busid_t p;

    p.busnum  = busnum;
    p.devnum  = devnum;
    p.funcnum = funcnum;
    if (ioctl(fd, DRM_IOCTL_IRQ_BUSID, &p)) return -errno;
    return p.irq;
}

int drmAddContextTag(int fd, drmContext context, void *tag)
{
    drmHashEntry  *entry = drmGetEntry(fd);

    if (drmHashInsert(entry->tagTable, context, tag)) {
    drmHashDelete(entry->tagTable, context);
    drmHashInsert(entry->tagTable, context, tag);
    }
    return 0;
}

int drmDelContextTag(int fd, drmContext context)
{
    drmHashEntry  *entry = drmGetEntry(fd);

    return drmHashDelete(entry->tagTable, context);
}

void *drmGetContextTag(int fd, drmContext context)
{
    drmHashEntry  *entry = drmGetEntry(fd);
    void          *value;
    
    if (drmHashLookup(entry->tagTable, context, &value)) return NULL;

    return value;
}

/********************************************************* 
 * ] End of code lifted directly from xf86drm.c
 *********************************************************/

/********************************************************* 
 * Code lifted directly from xf86drmHash.c [ 
 *********************************************************/

#define N(x)  drm##x

#define HASH_MAGIC 0xdeadbeef
#define HASH_DEBUG 0
#define HASH_SIZE  512      /* Good for about 100 entries */
                /* If you change this value, you probably
                                   have to change the HashHash hashing
                                   function! */
#define HASH_ALLOC malloc
#define HASH_FREE  free
#define HASH_RANDOM_DECL
#define HASH_RANDOM_INIT(seed)  srandom(seed)
#define HASH_RANDOM             random()

typedef struct HashBucket {
    unsigned long     key;
    void              *value;
    struct HashBucket *next;
} HashBucket, *HashBucketPtr;

typedef struct HashTable {
    unsigned long    magic;
    unsigned long    entries;
    unsigned long    hits;  /* At top of linked list */
    unsigned long    partials;  /* Not at top of linked list */
    unsigned long    misses;    /* Not in table */
    HashBucketPtr    buckets[HASH_SIZE];
    int              p0;
    HashBucketPtr    p1;
} HashTable, *HashTablePtr;


static unsigned long HashHash(unsigned long key)
{
    unsigned long        hash = 0;
    unsigned long        tmp  = key;
    static int           init = 0;
    static unsigned long scatter[256];
    int                  i;

    if (!init) {
    HASH_RANDOM_DECL;
    HASH_RANDOM_INIT(37);
    for (i = 0; i < 256; i++) scatter[i] = HASH_RANDOM;
    ++init;
    }

    while (tmp) {
    hash = (hash << 1) + scatter[tmp & 0xff];
    tmp >>= 8;
    }

    hash %= HASH_SIZE;
#if HASH_DEBUG
    printf( "Hash(%d) = %d\n", key, hash);
#endif
    return hash;
}

void *N(HashCreate)(void)
{
    HashTablePtr table;
    int          i;

    table           = HASH_ALLOC(sizeof(*table));
    if (!table) return NULL;
    table->magic    = HASH_MAGIC;
    table->entries  = 0;
    table->hits     = 0;
    table->partials = 0;
    table->misses   = 0;

    for (i = 0; i < HASH_SIZE; i++) table->buckets[i] = NULL;
    return table;
}

int N(HashDestroy)(void *t)
{
    HashTablePtr  table = (HashTablePtr)t;
    HashBucketPtr bucket;
    HashBucketPtr next;
    int           i;

    if (table->magic != HASH_MAGIC) return -1; /* Bad magic */
    
    for (i = 0; i < HASH_SIZE; i++) {
    for (bucket = table->buckets[i]; bucket;) {
        next = bucket->next;
        HASH_FREE(bucket);
        bucket = next;
    }
    }
    HASH_FREE(table);
    return 0;
}

/* Find the bucket and organize the list so that this bucket is at the
   top. */

static HashBucketPtr HashFind(HashTablePtr table,
                  unsigned long key, unsigned long *h)
{
    unsigned long hash = HashHash(key);
    HashBucketPtr prev = NULL;
    HashBucketPtr bucket;

    if (h) *h = hash;

    for (bucket = table->buckets[hash]; bucket; bucket = bucket->next) {
    if (bucket->key == key) {
        if (prev) {
                /* Organize */
        prev->next           = bucket->next;
        bucket->next         = table->buckets[hash];
        table->buckets[hash] = bucket;
        ++table->partials;
        } else {
        ++table->hits;
        }
        return bucket;
    }
    prev = bucket;
    }
    ++table->misses;
    return NULL;
}

int N(HashLookup)(void *t, unsigned long key, void **value)
{
    HashTablePtr  table = (HashTablePtr)t;
    HashBucketPtr bucket;

    if (table->magic != HASH_MAGIC) return -1; /* Bad magic */
    
    bucket = HashFind(table, key, NULL);
    if (!bucket) return 1;  /* Not found */
    *value = bucket->value;
    return 0;           /* Found */
}

int N(HashInsert)(void *t, unsigned long key, void *value)
{
    HashTablePtr  table = (HashTablePtr)t;
    HashBucketPtr bucket;
    unsigned long hash;

    if (table->magic != HASH_MAGIC) return -1; /* Bad magic */
    
    if (HashFind(table, key, &hash)) return 1; /* Already in table */

    bucket               = HASH_ALLOC(sizeof(*bucket));
    if (!bucket) return -1; /* Error */
    bucket->key          = key;
    bucket->value        = value;
    bucket->next         = table->buckets[hash];
    table->buckets[hash] = bucket;
#if HASH_DEBUG
    printf("Inserted %d at %d/%p\n", key, hash, bucket);
#endif
    return 0;           /* Added to table */
}

int N(HashDelete)(void *t, unsigned long key)
{
    HashTablePtr  table = (HashTablePtr)t;
    unsigned long hash;
    HashBucketPtr bucket;

    if (table->magic != HASH_MAGIC) return -1; /* Bad magic */
    
    bucket = HashFind(table, key, &hash);

    if (!bucket) return 1;  /* Not found */

    table->buckets[hash] = bucket->next;
    HASH_FREE(bucket);
    return 0;
}

int N(HashNext)(void *t, unsigned long *key, void **value)
{
    HashTablePtr  table = (HashTablePtr)t;
    
    for (; table->p0 < HASH_SIZE;
     ++table->p0, table->p1 = table->buckets[table->p0]) {
    if (table->p1) {
        *key       = table->p1->key;
        *value     = table->p1->value;
        table->p1  = table->p1->next;
        return 1;
    }
    }
    return 0;
}

int N(HashFirst)(void *t, unsigned long *key, void **value)
{
    HashTablePtr  table = (HashTablePtr)t;
    
    if (table->magic != HASH_MAGIC) return -1; /* Bad magic */

    table->p0 = 0;
    table->p1 = table->buckets[0];
    return N(HashNext)(table, key, value);
}

/********************************************************* 
 * ] End of code lifted directly from xf86drmHash.c
 *********************************************************/

static int OpenFile(const char *file)
{
    int fd = open(file, O_RDWR, 0);

    if (fd >= 0) {
        return fd;
    }
    return -errno;
} 

static int OpenDevice(const char *path, long dev,
                      mode_t mode, uid_t user, gid_t group)
{
    struct stat st;
    int fd;

    if (!stat(path, &st) && st.st_rdev == dev) {
        return OpenFile(path);
    }

    if (geteuid()) {
        return DRM_ERR_NOT_ROOT;
    }
    remove(path);
    if (mknod(path, S_IFCHR, dev)) {
    remove(path);
    return DRM_ERR_NOT_ROOT;
    }
    chown(path, user, group);
    chmod(path, mode);

    return OpenFile(path);
}

#define S_IRUGO     (S_IRUSR|S_IRGRP|S_IROTH)
#define S_IWUGO     (S_IWUSR|S_IWGRP|S_IWOTH)

int hpgfx_open_graphics_device(int pci_bus, int pci_dev, int pci_func)
{
    int i, fd, result;
    unsigned int majmin;
    int major, minor;
    char buf[512];
    char proc_name[128];
    char dev_name[128];
    char drv_name[64];
#if 0 /* HACK!!  Should provide some security... */
    mode_t mode   = DRM_DEV_MODE;
#else
    mode_t mode   = S_IRUGO | S_IWUGO;
#endif
    mode_t dirmode;
    gid_t  group  = DRM_DEV_GID;
    uid_t  user   = DRM_DEV_UID;

    sprintf(proc_name, "/proc/dri/%d:%d:%d/name", pci_bus, pci_dev, pci_func);
    sprintf(dev_name, "/dev/dri/hpgfx%d:%d:%d", pci_bus, pci_dev, pci_func);
    if ((fd = open(proc_name, 0, 0)) >= 0) {
        result = read(fd, buf, sizeof(buf)-1);
        close(fd);
        if (result) {
            result = sscanf(buf, "%s %x", drv_name, &majmin);
            if (result > 1) {
                if (!geteuid()) {
                    dirmode = mode;
                    if (dirmode & S_IRUSR)
                        dirmode |= S_IXUSR;

                    if (dirmode & S_IRGRP)
                        dirmode |= S_IXGRP;
                    
                    if (dirmode & S_IROTH)
                        dirmode |= S_IXOTH;

                    dirmode &= ~(S_IWGRP | S_IWOTH);
                    mkdir("/dev/dri", 0);
                    chown("/dev/dri", user, group);
                    chmod("/dev/dri", dirmode);
                }
                return OpenDevice(dev_name, majmin, mode, user, group);
            }
        }
    }

    return -1;
}

  /* HACK!!!  For now, just set the sarea size to 4k (1 page) */
#define SAREA_MAX           0x1000

int hpgfx_map_sarea(int fd, drmSize size, drmMapFlags flags, drmAddressPtr address)
{
    int i, pfd, result, dummy, err;
    char procName[128];
    char buf[256];
    char *busidPtr;
    drmHandle hSArea;

    busidPtr = drmGetBusid(fd);
    if (!busidPtr) {
        return -ENXIO; /* failure */
    }
    sprintf(procName, "/proc/dri/%s/name", busidPtr);
    if ((pfd = open(procName, 0, 0)) >= 0) {
        result = read(pfd, buf, sizeof(buf)-1);
        close(pfd);
        if (result) {
            result = sscanf(buf, "%s %x %x", procName, &dummy, (int *) &hSArea);
            if ((result == 3) && (hSArea == 0)) {
                if (err = drmAddMap(fd, 0, SAREA_MAX, DRM_SHM, flags, &hSArea) < 0) {
                    drmFreeBusid(busidPtr);
                    return err;
                }
            }
            drmFreeBusid(busidPtr);
            return drmMap(fd, hSArea, size, address);
        }
    }
    drmFreeBusid(busidPtr);
    return -ENXIO; /* failure */
}

int hpgfx_unmap_sarea(drmAddress ptr, drmSize size)
{
    return drmUnmap(ptr, size);
}

