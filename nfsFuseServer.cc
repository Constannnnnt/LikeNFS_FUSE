
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "nfsFuse.grpc.pb.h"

#include <string>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using namespace nfsFuse;
using namespace std;

void translatePath(const char* client_path, char* server_path){
    strcat(server_path, "~/nfsServer");
    strcat(server_path+11, client_path);
    server_path[strlen(server_path)] = '\0';
}

class nfsFuseImpl final : public NFSFuse::Service {
    public:
        explicit nfsFuseImpl() {}  
        Status nfs_getattr(ServerContext* context, const GetAttrRequestParams* request, GetAttrResponseParams* response)  {
            // print out the client path 
	    cout<<"Get Attribute:"<<endl;
	    std::cout<<"\tinit path: "<<request->path().c_str()<<std::endl;

            // translate the server path
            char server_path[512] = {0};
            translatePath(request->path().c_str(), server_path);
            std::cout<<"\tclient path: "<<request->path().c_str()<<"\tserver path:"<<server_path<<endl;

	    struct stat status;
            int lstat_result = lstat(server_path, &status);

            if(lstat_result == -1){
		std::cout<<"errno: "<<errno<<std::endl;
                // perror(strerror(errno));
                response->set_err(errno);
            } else {
	        response->set_dev(status.st_dev);
                response->set_inode(status.st_ino);
                response->set_mode(status.st_mode);
                response->set_nlink(status.st_nlink);
                response->set_uid(status.st_uid);
                response->set_guid(status.st_gid);
                response->set_size(status.st_size);
                response->set_blocksize(status.st_blksize);
                response->set_nblock(status.st_blocks);
                response->set_latime(status.st_atime);
                response->set_mtime(status.st_mtime);
                response->set_ctime(status.st_ctime);
                response->set_err(0);
                cout<<"dev:"<<response->dev()<<" inode:"<<response->inode()<<" mode"<<response->mode()<<" nlink"<<response->nlink()<<" uid"<<response->uid()<<" guid"<<response->guid()<<endl;
            }
	
 	    return Status::OK;
        }

    
        Status nfs_readdir(ServerContext* context, ReadDirRequestParams* request, ServerWriter<ReadDirResponseParams>* response) {
            cout<<"Read Attr"<<endl;
        
	    DIR *dp;
	    struct dirent *de;
	    ReadDirResponseParams dir_response;
	    char server_path[512] = {0};
	    translatePath(request->path().c_str(), server_path);

	    dp = opendir(server_path);
  	    if (dp == NULL){
	        perror(strerror(errno));
	        dir_response.set_err(errno);
                return Status::OK;
	    }

	    while((de = readdir(dp)) != NULL) {
	        dir_response.set_dinode(de->d_ino);
	        dir_response.set_dname(de->d_name);
	        dir_response.set_dtype(de->d_type);
	        response->Write(dir_response);
	    }

	    dir_response.set_err(0);
	    closedir(dp);
	    return Status::OK;
        }

        Status nfs_mkdir(ServerContext* context, MkDirRequestParams* in_dir, VoidMessage* vmsg) {
            cout<<"Mkdir"<<endl;

	    char server_path[512] = {0};
	    translatePath(in_dir->path().c_str(), server_path);
	    int result = mkdir(server_path, in_dir->mode());

	    if (result == -1) {
	        perror(strerror(errno)); 
                vmsg->set_err(errno);
                return Status::OK;
	    }
	
	    vmsg->set_err(errno);
	    return Status::OK;
        };

        Status nfs_rmdir(ServerContext* context, RmDirRequestParams* in_dir, VoidMessage* vmsg) {
            cout<<"Rmdir" <<endl;

	    char server_path[512] = {0};
            translatePath(in_dir->path().c_str(), server_path);
	    int result = rmdir(server_path);

	    if (result == -1) {
	        perror(strerror(errno));
	        vmsg->set_err(errno);
	        return Status::OK;
	    }

	    vmsg->set_err(0);
	    return Status::OK;
	}
        
	Status nfs_create(ServerContext* context, CreateRequestParams* request, CreateResponseParams* response){
	    cout << "[DEBUG] Client's Path: " << request->path() << endl;
	    cout << "[DEBUG] Client's Flag: " << request->flags() << endl;
	    cout << "[DEBUG] Client's Mode: " << request->mode() << endl;
	
	    // const char* serverpath = request->path().c_str();
	    char serverpath[512] = {0};
	    translatePath(request->path().c_str(), serverpath);
	    
	    int res = open(serverpath, request->flags(), request->mode());
	    if (res == -1) {
		response->set_err(errno);
	        return Status::OK;
	    } else {
	        response->set_fh(res);
		response->set_err(0);
		close(res);
		return Status::OK;
	    }
	}

	Status nfs_open(ServerContext* context, OpenRequestParams* request, OpenResponseParams* response) {
	    cout << "[DEBUG] Client's Path: " << request->path() << endl;
	    cout << "[DEBUG] Client's Flag: " << request->flags() << endl;

	    // const char* serverpath = request->path().c_str();
	    char serverpath[512] = {0};
	    translatePath(request->path().c_str(), serverpath);
	    int res = open(serverpath, request->flags());
	    if (res == -1) {
	        response->set_err(errno);
		return Status::OK;
	    } else {
	        response->set_fh(res);
		response->set_err(0);
		close(res);
		return Status::OK;
	    }
	}
	
	Status nfs_read(ServerContext* context, ReadRequestParams* request, ReadResponseParams* response) {
	    cout << "[DEBUG] Client's Path: " << request->path() << endl;
	    cout << "[DEBUG] Client's size: " << request->size() << endl;
	    cout << "[DEBUG] Client's offset " << request->offset() << endl;

	    // const char* serverpath = request->path().c_str();
	    char serverpath[512] = {0};
	    translatePath(request->path().c_str(), serverpath);
	    char* buf = new char[request->size()];
	    int res;
	    int fd = open(serverpath, O_RDONLY);

	    if (fd == -1) {
	        response->set_err(errno);
		return Status::OK;
	    }

	    res = pread(fd, buf, request->size(), request->offset());
	    if (res == -1) {
	        response->set_err(errno);
		return Status::OK;
	    }

	    response->set_buffer(buf);
	    response->set_bufferlength(res);
	    response->set_err(0);

	    if (fd > 0) {
	        close(fd);
		free(buf);
	    }
	    return Status::OK;
	}

	Status nfs_write(ServerContext* context, WriteRequestParams* request, WriteResponseParams* response) {
	
	    cout << "[DEBUG] Client's Path: " << request->path() << endl;
	    cout << "[DEBUG] Client's size: " << request->bufferlength() << endl;
	    cout << "[DEBUG] Client's offset " << request->offset() << endl;

	    // const char* serverpath = request->path().c_str();
	    char serverpath[512] = {0};
	    translatePath(request->path().c_str(), serverpath);
	    int res;
	    int fd;

	    fd = open(serverpath, O_WRONLY);
	    if (fd == -1) {
	        response->set_err(errno);
		return Status::OK;
	    }

	    res = pwrite(fd, request->buffer().c_str(), request->bufferlength(), request->offset());
	    if (res == -1) {
	        response->set_err(errno);
		return Status::OK;
	    }

	    // synchronize file contents
	    fsync(fd);

	    response->set_bufferlength(res);
	    response->set_err(0);

	    if (fd > 0) {
	        close(fd);
	    } 
	    
	    return Status::OK;
	}
    
    private:
};

void RunServer() {

    // Specify the server address
    std::string server_address("0.0.0.0:50051");
    nfsFuseImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {

    RunServer();
    return 0;
}
