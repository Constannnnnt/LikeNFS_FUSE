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
        int nfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
	    cout << "[DEBUG] nfs_create" << endl;
	    ClientContext ctx;
            CreateRequestParams request;
	    CreateResponseParams response;
	    request.set_path(path);
	    request.set_mode(mode);
	    request.set_flags(fi->flags);

	    cout << "[DEBUG] Before Send" << endl;
	    Status status = stub_->nfs_create(&ctx, request, &response);
	    cout << "[DEBUG] After Sent" << endl;
	    if (response.err() == 0) {
	        fi->fh = response.fh();
	        cout << "[DEBUG] nfs_create finished without err" << endl;
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
 
    int rpc_getattr(std::string in_path, struct stat* response) {
    	cout<<"** rpc client get attr **"<<endl;
	GetAttrResponseParams new_response;
    	ClientContext context;
	GetAttrRequestParams path;
        cout<<"\tin_path:"<<in_path<<endl;
    	path.set_path(in_path);
    	memset(response, 0, sizeof(GetAttrResponseParams));

    	Status status = stub_->nfs_getattr(&context, path, &new_response);
	cout << "[DEBUG] Error Code: " << new_response.err() << endl;

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
    }

    int rpc_mkdir(string in_path, mode_t mode) {
        ClientContext ccontext;
	nfsFuse::MkDirRequestParams dir_request;
	VoidMessage vmsg;

	dir_request.set_path(in_path);
	dir_request.set_mode(mode);

	Status status = stub_->nfs_mkdir(&ccontext, dir_request, &vmsg);
	
	if (vmsg.err() != 0) {
            std::cout << "Error: mkdir fails" << std::endl;
        }

        return vmsg.err();
    }

    int rpc_rmdir(string in_path) {
        ClientContext ccontext;
	nfsFuse::RmDirRequestParams dir_request;
	VoidMessage vmsg;

	dir_request.set_path(in_path);

	Status status = stub_->nfs_rmdir(&ccontext, dir_request, &vmsg);

	if (vmsg.err() != 0) {
	    std::cout << "Error: rmdir fails" << endl;
	}

	return vmsg.err();
    }
 
    private:
	std::unique_ptr<NFSFuse::Stub> stub_;
};
