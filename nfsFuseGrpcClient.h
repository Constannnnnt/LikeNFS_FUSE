#include <grpc++/grpc++.h>
#include "nfsFuse.grpc.pb.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

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
    	cout<<"** rpc client get attr **"<<endl;
	GetAttrResponseParams new_response;
    	ClientContext context;
	GetAttrRequestParams path;
        cout<<"\tin_path:"<<in_path<<endl;
    	path.set_path(in_path);
    	memset(response, 0, sizeof(GetAttrResponseParams));

    	Status status = stub_->nfs_getattr(&context, path, &new_response);

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
    }; 

    int rpc_readdir(string in_path, void *buf, fuse_fill_dir_t filler){
        ClientContext ccontext;
	Status status;
	
	ReadDirRequestParams request;
	request.set_path(in_path);
	ReadDirResponseParams response;
	dirent dir;
	
	std::unique_ptr<ClientReader<ReadDirResponseParams> >reader(
			stub_->nfs_readdir(&ccontext, request));
        cout<<"** rpc client read dir **"<<endl;       
	while (reader->Read(&response)) {
	    struct stat sta;
	    memset(&status, 0, sizeof(sta));
	    dir.d_ino = response.dinode();
	    strcpy(dir.d_name, response.dname().c_str());
	    dir.d_type = response.dtype();
	    sta.st_ino = dir.d_ino;
	    sta.st_mode = dir.d_type << 12;
            
            if (filler(buf, dir.d_name, &sta, 0, static_cast<fuse_fill_dir_flags>(0))) {
	        break;
	    }
	}

	status = reader->Finish();
	return -response.err();
    };
 
 
    private:
	std::unique_ptr<NFSFuse::Stub> stub_;
};
