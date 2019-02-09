#include <grpc++/grpc++.h>
#include "nfsFuse.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReader;

using namespace nfsFuse;
using namespace std;

class nfsFuseGrpcClient {
    public:
        nfsFuseGrpcClient(std::shared_ptr<Channel> channel):stub_(NFSFuse::NewStub(channel)) {}
        int nfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
	    cout << "[DEBUG] nfs_create" << endl;
	    ClientContext ctx;
            CreateRequestParams request;
	    CreateResponseParams response;
	    request.set_path(path);
	    request.set_mode(mode);
	    request.set_flags(fi->flags);

	    Status status = stub_->nfs_create(&ctx, request, &response);
	    if (response.err() == 0) {
	        fi->fh = response.fh(); 
	    }

	    return -response.err();
	}
	int nfs_open(const char *path, struct fuse_file_info *fi) {
	    cout << "[DEBUG] nfs_open" << endl;
	    ClientContext ctx;
	    OpenRequestParams request;
	    OpenResponseParams response;
	    request.set_path(path);
	    request.set_flags(fi->flags);

	    Status status = stub_->nfs_open(&ctx, request, &response);
	    if (response.err() == 0) {
	        fi->fh = response.fh();
	    }

	    return -response.err();
	}
	int nfs_read(const char *path, char* buf, size_t size, off_t offset) {
	    cout << "[DEBUG] nfs_read" << endl;
	    ClientContext ctx;
	    ReadRequestParams request;
	    ReadResponseParams response;
	    request.set_path(path);
	    request.set_size(size);
	    request.set_offset(offset);

	    Status status = stub_->nfs_read(&ctx, request, &response);
	    if (response.err() == 0) {
		strcpy(buf, response.buffer().c_str());
	        return response.bufferlength();
	    } else {
	        return -response.err();
	    }
	}
	int nfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	    cout << "[DEBUG] nfs_write" << endl;
	    ClientContext ctx;
	    WriteRequestParams request;
	    WriteResponseParams response;
	    request.set_path(path);
	    request.set_bufferlength(size);
	    request.set_offset(offset);
	    request.set_buffer(buf);

	    Status status = stub_->nfs_write(&ctx, request, &response);
	    if (response.err() == 0) {
	        return response.bufferlength();
	    } else {
	        return -response.err();
	    }
	}
    private:
	std::unique_ptr<NFSFuse::Stub> stub_;
};
