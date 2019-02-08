#include <grpc++/grpc++.h>
#include "nfsFuse.grpc.pb.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReader;

using namespace nfsFuse;
using namespace std;

class nfsFuseGrpcClient {
    public:
        nfsFuseGrpcClient(std::shared_ptr<Channel> channel):stub_(NFSFuse::NewStub(channel)) {}
 
    int rpc_getattr(std::string in_path, struct stat* response) {
    	GetAttrResponseParams new_response;
    	ClientContext context;
	GetAttrRequestParams path;

    	path.set_path(in_path);
    	memset(response, 0, sizeof(GetAttrResponseParams));

    	Status status = stub_->getattr(&context, path, &new_response);

    	if(new_response.err() != 0){
            return -new_response.err();
        } else {
	    response->st_ino = new_response.inode();
	    response->st_mode = new_response.mode();
    	    response->st_nlink = new_response.nlink();
	    response->st_uid = new_response.uid();
	    response->st_gid = new_response.guid();
    	    response->st_size = new_response.size();
	    response->st_blksize = new_response.blocksize();
	    response->st_blocks = new_response.nblock();
	    response->st_atime = new_response.latime();
	    response->st_mtime = new_response.mtime();
	    response->st_ctime = new_response.ctime();
	
	}
        return 0;
    }
 
 
    private:
	std::unique_ptr<NFSFuse::Stub> stub_;
};
