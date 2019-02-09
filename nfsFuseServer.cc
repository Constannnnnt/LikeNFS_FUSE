
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

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using namespace nfsFuse;
using namespace std;

class nfsFuseImpl final : public NFSFuse::Service {
    public:
        explicit nfsFuseImpl() {}
    /*
    Status getattr(ServerContext* context, GetAttrRequestParams* request, GetAttrResponseParams* response) override {
        return Status::OK;
    }

    Status readattr(ServerContext* context, ReadDirRequestParams* request, ReadDirResponseParams* reponse) override {
        return Status::OK;
    }
    */
        Status nfs_create(ServerContext* context, CreateRequestParams* request, CreateResponseParams* response){
	    cout << "[DEBUG] Client's Path: " << request->path() << endl;
	    cout << "[DEBUG] Client's Flag: " << request->flags() << endl;
	    cout << "[DEBUG] Client's Mode: " << request->mode() << endl;
	
	    const char* serverpath = request->path().c_str();
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

	    const char* serverpath = request->path().c_str();
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

	    const char* serverpath = request->path().c_str();
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

	    const char* serverpath = request->path().c_str();
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
    // Initialize a mount point
    // nfsFuseImple service(db_path); 

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
