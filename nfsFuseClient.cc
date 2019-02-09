/*
 * Fuse Client, implemented based on FUSE: Filesystem in Userspace
 */
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>

#include "nfsFuseGrpcClient.h"


/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
static struct options {
    nfsFuseGrpcClient* nfsFuseClient;
    int show_help;
} options;

#define OPTION(t, p)                           \
    { t, offsetof(struct options, p), 1 }
static const struct fuse_opt option_spec[] = {
    OPTION("-h", show_help),
    OPTION("--help", show_help),
    FUSE_OPT_END
};

static void show_help(const char *progname)
{
    std::cout << ("usage: %s [options] <mountpoint>\n\n", progname);
}

static void *client_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    (void) conn;
    return NULL;
}

static int client_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    cout<<"client getattr in cc file:"<<path<<endl;
    // memset(stbuf, 0, sizeof(struct stat));      
    cout << "[DEBUG] Path: " << path << endl; 
    return options.nfsFuseClient->rpc_getattr(path, stbuf);
}

static int client_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    std::cout << "[DEBUG] create: " << endl;
    int ret;
    ret = options.nfsFuseClient->nfs_create(path, mode, fi);
    std::cout << "[DEBUG] create end: " << endl;
    return ret;
}

static int client_open(const char *path, struct fuse_file_info *fi) {
    std::cout << "[DEBUG] open: " << endl;
    int ret;
    ret = options.nfsFuseClient->nfs_open(path, fi);
    return ret;
}

static int client_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    std::cout << "[DEBUG] read: " << endl;
    (void) fi;
    int ret;
    ret = options.nfsFuseClient->nfs_read(path, buf, size, offset);
    return ret;
}

static int client_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    std::cout << "[DEBUG] write: " << endl;
    (void) fi;
    int ret;
    ret = options.nfsFuseClient->nfs_write(path, buf, size, offset, fi);
    return ret;
}

static int client_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, 
    struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    cout<<"client readdir in cc file"<<path<<endl;
    return options.nfsFuseClient->rpc_readdir(path, buf, filler);
}

static int client_mkdir(const char *path, mode_t mode) {
    cout<<"client mkdir in cc file"<<path<<endl;
    return options.nfsFuseClient->rpc_mkdir(path, mode);
}

static int client_rmdir(const char *path) {
    cout<<"client rmdir in cc file"<<path<<endl;
    return options.nfsFuseClient->rpc_rmdir(path);
}
/*
 * Define FUSE operations 
 */
static struct client_operations : fuse_operations {
    client_operations() { 
        init = client_init;
        create = client_create;
        open = client_open;
	read = client_read;
	write = client_write;
        getattr = client_getattr;
        readdir = client_readdir;
        mkdir = client_mkdir;
        rmdir = client_rmdir;
    }
} client_oper;

int main(int argc, char* argv[]) {
    int ret;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));

    /* Parse options */
    if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1) {
        return 1;
    }

    /* When --help is specified, first print our own file-system 
     * specific help text, then signal fuse_main to show 
     * additional help (by adding `--help` to the options again) 
     * without usage: line (by setting argv[0] to the empty 
     * string)
     */
    if (options.show_help) {
        show_help(argv[0]);
        assert(fuse_opt_add_arg(&args, "--help") == 0);
        args.argv[0][0] = '\0';
    }

    ret = fuse_main(args.argc, args.argv, &client_oper, NULL);
    fuse_opt_free_args(&args);
    cout<<"Terminate client"<<endl;
    return ret;
}
