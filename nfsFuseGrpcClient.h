#include <grpc++/grpc++.h>
#include "nfsFuse.grpc.pb.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReader;

using namespace nfsFuse;
using namespace std;

class nfsFuseGrpcClient;

static struct _options {
    nfsFuseGrpcClient* nfsFuseClient;
} _options;

vector<WriteRequestParams> StagedWrites;

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
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_create(&_ctx, request, &response);
	    }
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
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_open(&_ctx, request, &response);
	    }
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
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_read(&_ctx, request, &response);
	    }
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

	    StagedWrites.push_back(request);

	    Status status = stub_->nfs_write(&ctx, request, &response);
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_write(&_ctx, request, &response);
	    }

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
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_getattr(&_ctx, path, &new_response);
	    }
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
	
        // cout<<"!!!reader:"<<reader->Read(&response)<<" respoonse dname:"<<response.dname()<<"\tres dinode():"<<response.dinode()<<" response dtype:"<<response.dtype()<<endl;
        
	
	    while (reader->Read(&response)) {
                cout<<" -> in while ";
	        struct stat sta;
	        memset(&sta, 0, sizeof(sta));
	        dir.d_ino = response.dinode();
	        strcpy(dir.d_name, response.dname().c_str());
	        dir.d_type = response.dtype();
	        sta.st_ino = dir.d_ino;
	        //sta.st_mode = dir.d_type << 12;
                sta.st_mode = dir.d_type;
	        if (filler(buf, dir.d_name, &sta, 0, static_cast <fuse_fill_dir_flags>(0))) {
	            cout<<"[break] break by filler"<<endl;
	            break;
	        }
	    }
        
	    status = reader->Finish();
            cout<<"<before return>"<<endl;       
	    return -response.err();
        }

        int rpc_mkdir(string in_path, mode_t mode) {
            ClientContext ccontext;
	    nfsFuse::MkDirRequestParams dir_request;
	    VoidMessage vmsg;

	    dir_request.set_path(in_path);
	    dir_request.set_mode(mode);

	    Status status = stub_->nfs_mkdir(&ccontext, dir_request, &vmsg);
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_mkdir(&_ctx, dir_request, &vmsg);
	    }
	
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
	    // crash etc.
	    while (!status.ok()) {
	        ClientContext _ctx;
		// new attemps
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_rmdir(&_ctx, dir_request, &vmsg);
	    }

	    if (vmsg.err() != 0) {
	        std::cout << "Error: rmdir fails" << endl;
	    }

	    return vmsg.err();
        }

	int reCommitPartial(int serverstatus) {
	    // the serverstatus marks the offset where the transmission stops
	    if (StagedWrites.size() == 0) {
	        // staged writes is empty, no need commit
		return -1;
	    }

	    vector<WriteRequestParams>::const_iterator it = StagedWrites.begin();

	    while (it != StagedWrites.end()) {
	       ClientContext ctx;
	       WriteResponseParams response;
	       if (it->offset() == serverstatus) break;
	       Status status = stub_->nfs_recommit(&ctx, *it, &response);
	       it++;
	    }

	    return 0;
	}

	int reCommitAll() {
	    // the serverstatus marks the offset where the transmission stops
	    if (StagedWrites.size() == 0) {
	        // staged writes is empty, no need commit
		return -1;
	    }

	    vector<WriteRequestParams>::const_iterator it = StagedWrites.begin();

	    while (it != StagedWrites.end()) {
	       ClientContext ctx;
	       WriteResponseParams response;
	       Status status = stub_->nfs_recommit(&ctx, *it, &response);
	       it++;
	    }

	    return 0;
	}

	int nfs_commit(int fh, int firstWrite_offset, int lastWrite_offset) {
	    ClientContext ctx;
	    CommitRequestParams request;
	    CommitResponseParams response;
	    request.set_fh(fh);
	    request.set_offset(StagedWrites.front().offset());
	    request.set_endoffset(StagedWrites.back().offset());
	    
	    Status status = stub_->nfs_commit(&ctx, request, &response);
	    while (!status.ok()) {
	        ClientContext _ctx;
		_options.nfsFuseClient = new nfsFuseGrpcClient(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));
		status = stub_->nfs_commit(&_ctx, request, &response);
	    }

	    if (response.err() == 0) {
	        // Commits finished without errer
		int StagedWritesSize = StagedWrites.size();
		for (int i = 0; i < StagedWritesSize; i++) {
	            StagedWrites.pop_back();	
		}
	    } else {
	        // some errors happen at the server side
		int serverstatus = response.serverstatus();
		int res;

		if (response.err() == -1) {
		    // crash during transmission, resend writes to Server Side
		    res = this->reCommitPartial(serverstatus);
		} else if (response.err() == -2) {
		    // crash before transmission, resend all writes to Server Side
		    res = this->reCommitAll();
		}

		if (res != 0) {
		    // reCommit fails
		    return -1;
		}

		// Commit Again to match the original wirtes and resent writes
		res = this->nfs_commit(fh, firstWrite_offset, lastWrite_offset);
		if (res != 0) {
		    // not match
		    return -1;
		}
		
		return res;
	    }
	}

    int rpc_unlink(string in_path) {
        ClientContext ccontext;
        nfsFuse::UnlinkRequestParams dir_request;
        VoidMessage vmsg;
        
        dir_request.set_path(in_path);

        Status status = stub_->nfs_unlink(&ccontext, dir_request, &vmsg);

        if (vmsg.err() != 0) {
	    cout << "Error: unlink fails" << endl;
	}	 

	return vmsg.err();
    }

    int rpc_mknod(string in_path, mode_t mode, dev_t dev) {
        ClientContext ccontext;
	nfsFuse::MknodRequestParams dir_request;
	VoidMessage vmsg;

	dir_request.set_path(in_path);
        dir_request.set_mode(mode);
	dir_request.set_rdev(dev);

	Status status = stub_->nfs_mknod(&ccontext, dir_request, &vmsg);

	if (vmsg.err() != 0) {
	    cout << "Error: mknod fails" << endl;
	} 

	return vmsg.err();
    }

    
    int rpc_rename(string from, string to, unsigned int flag) {
        ClientContext ccontext;
        nfsFuse::RenameRequestParams request;
        VoidMessage vmsg;

        request.set_fp(from);
        request.set_tp(to);
        request.set_flag(flag);	

        Status status = stub_->nfs_rename(&ccontext, request, &vmsg);
	if (vmsg.err() != 0) {
	    cout << "Error: rename fails" << endl;
	}

	return vmsg.err();
    } 
   
   /* 
    int rpc_utimens(string in_path, struct timespec *ts, struct fuse_file_info *fi) {
        ClientContext ccontext;
	nfsFuse::UtimensRequestParams request;
	VoidMessage vmsg;

	request.set_path(in_path);
	request.set_sec(ts[0].tv_sec);
	request.set_nsec(ts[0].tv_nsec);
        request.set_sec2(ts[1].tv_sec);
	request.set_nsec2(ts[1].tv_nsec);

        Status status = stub_->nfs_utimens(&ccontext, request, &vmsg);
	if (vmsg.err() != 0) {
	    cout << "Error: utimens fails" << endl;
	}

	return vmsg.err();
    }
 */
    private:
	std::unique_ptr<NFSFuse::Stub> stub_;
};
