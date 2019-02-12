
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <memory>

#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#include <grpc++/grpc++.h>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "nfsFuse.grpc.pb.h"

#define READ_MAX    10000000

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using namespace nfsFuse;
using namespace std;

vector<WriteRequestParams> StagedWrites;
vector<WriteRequestParams> reStagedWrites;
bool _read = false;

void translatePath(const char* client_path, char* server_path){
    strcat(server_path, "./nfsServer");
    strcat(server_path+11, client_path);
    server_path[strlen(server_path)] = '\0';
}

class nfsFuseImpl final : public NFSFuse::Service {
    public:
        explicit nfsFuseImpl() {}  
        Status nfs_getattr(ServerContext* context, const GetAttrRequestParams* request, GetAttrResponseParams* response) override  {
            // print out the client path 
	    // cout<<"Get Attribute:"<<endl;

            // translate the server path
            char server_path[512] = {0};
            translatePath(request->path().c_str(), server_path);
            // std::cout<<"\tclient path: "<<request->path().c_str()<<"\tserver path:"<<server_path<<endl;

	    struct stat status;
            int lstat_result = lstat(server_path, &status);

            if(lstat_result == -1){
		// std::cout<<"errno: "<<errno<<std::endl;
                // perror(strerror(errno));
                response->set_err(errno);
            } else {
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
                // cout<<"dev:"<<response->dev()<<" inode:"<<response->inode()<<" mode"<<response->mode()<<" nlink"<<response->nlink()<<" uid"<<response->uid()<<" guid"<<response->guid()<<endl;
            }
	
 	    return Status::OK;
        }

    
        Status nfs_readdir(ServerContext* context, const ReadDirRequestParams* request, ServerWriter<ReadDirResponseParams>* response) override {
            // cout<<"Read Attr"<<endl;
        
	    DIR *dp;
	    struct dirent *de;
	    ReadDirResponseParams dir_response;
	    char server_path[512] = {0};
	    translatePath(request->path().c_str(), server_path);
            // cout<< " print server path: " << server_path << endl;
	    dp = opendir(server_path);
  	    if (dp == NULL){
		// cout<<"dp == NULL"<<endl;
	        perror(strerror(errno));
	        dir_response.set_err(errno);
                return Status::OK;
	    }

	    while((de = readdir(dp)) != NULL) {
		// cout<<"\[while] read:"<<de->d_name<<endl;
	        dir_response.set_dinode(de->d_ino);
	        dir_response.set_dname(de->d_name);
	        dir_response.set_dtype(de->d_type);
	        response->Write(dir_response);
	    }
	    
	    dir_response.set_err(0);
	    closedir(dp);
	    return Status::OK;
        }

        Status nfs_mkdir(ServerContext* context, const MkDirRequestParams* in_dir, VoidMessage* vmsg) override {
            // cout<<"Mkdir"<<endl;

	    char server_path[512] = {0};
	    translatePath(in_dir->path().c_str(), server_path);
	    //cout<<"\ttranslate done. "<<"mode:"<<in_dir->mode();
	    int result = mkdir(server_path, in_dir->mode());
	    //cout<<"\tmkdir done"<<"\terrno is:"<<errno<<endl;
	    if (result == -1) {
	        perror(strerror(errno)); 
                vmsg->set_err(errno);
                return Status::OK;
	    }
	
	    vmsg->set_err(0);
	    return Status::OK;
        };

        Status nfs_rmdir(ServerContext* context, const RmDirRequestParams* in_dir, VoidMessage* vmsg) override {
            // cout<<"Rmdir" <<endl;

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
        
	Status nfs_create(ServerContext* context, const CreateRequestParams* request, CreateResponseParams* response)override {
	    // cout << "[DEBUG] CREATE Client's Path: " << request->path() << endl;
	    // cout << "[DEBUG] CREATE Client's Flag: " << request->flags() << endl;
	    // cout << "[DEBUG] CREATE Client's Mode: " << request->mode() << endl;
	
	    // const char* serverpath = request->path().c_str();
	    char serverpath[512] = {0};
	    translatePath(request->path().c_str(), serverpath);
            // std::cout<<"\tclient path: "<<request->path().c_str()<<"\tserver path:"<<serverpath<<endl;
	    
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

	Status nfs_open(ServerContext* context, const OpenRequestParams* request, OpenResponseParams* response) override {
	    // cout << "[DEBUG] OPEN Client's Path: " << request->path() << endl;
	    // cout << "[DEBUG] OPEN Client's Flag: " << request->flags() << endl;

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
	
	Status nfs_read(ServerContext* context, const ReadRequestParams* request, ReadResponseParams* response) override {
	    // cout << "[DEBUG] READ Client's Path: " << request->path() << endl;
	    // cout << "[DEBUG] READ Client's size: " << request->size() << endl;
	    // cout << "[DEBUG] READ Client's offset " << request->offset() << endl;

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

	    _read = true;
	    response->set_buffer(buf);
	    response->set_bufferlength(res);
	    response->set_err(0);

	    if (fd > 0) {
	        close(fd);
	    }
	    free(buf);
	    return Status::OK;
	}

	Status nfs_write(ServerContext* context, const WriteRequestParams* request, WriteResponseParams* response) override {
	    /*
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
	    */

	    // cout << "[DEBUG] nfs_write" << endl;
	    WriteRequestParams _request(*request);

            StagedWrites.push_back(_request);
            response->set_bufferlength(request->bufferlength());
            response->set_err(0);

            return Status::OK;

	}

	Status nfs_commit(ServerContext* context, const CommitRequestParams* request, CommitResponseParams* response) override {
	
            // cout << "[DEBUG] nfs_commit" << endl;
	    int res;
	    string buf;
	    int buflength = 0;
	    int offset;
	    char serverpath[512] = {0};

	    int stagedWritesSize = StagedWrites.size();
	    int reStagedWritesSize = reStagedWrites.size();
	    // cout << stagedWritesSize << " " << reStagedWritesSize << endl;

	    // read, then append nothing
	    if (StagedWrites.size() == 0 && _read) {
	        //  cout << "[DEBUG] nfs_commit read" << endl;
		close(request->fh());
		response->set_err(0);
                _read = false;
		// cout << "[DEBUG] nfs_commit read finished" << endl;
                return Status::OK;		
	    }

	    // totally lost, when reach the commit stage
	    if (stagedWritesSize == 0 && reStagedWritesSize == 0) {
		// cout << "[DEBUG] commit lost" << endl;
	        response->set_err(-2);
		response->set_serverstatus(request->endoffset());
		return Status::OK;
	    } else if (reStagedWritesSize != 0) {
	        // reach here after recommit all 
		// cout << "[DEBUG] recommit" << endl;
		translatePath(reStagedWrites.begin()->path().c_str(), serverpath);
	        offset = reStagedWrites.begin()->offset();
		for (int i = 0; i < reStagedWritesSize; i ++) {
		    buf.append(reStagedWrites.at(i).buffer());
		    buflength += reStagedWrites.at(i).bufferlength();
		}
	    } else if (request->offset() != StagedWrites.begin()->offset()) {
	        // crash during writing the data
		// cout << "[DEBUG] commit resend" << endl;
		response->set_err(-1);
		response->set_serverstatus(StagedWrites.begin()->offset());
		return Status::OK;
	    } else {
	        // not crash
		// cout << "[DEBUG] not crash" << endl;
		translatePath(StagedWrites.begin()->path().c_str(), serverpath);
		offset = StagedWrites.begin()->offset();
	    }

	    for (int i = 0; i < stagedWritesSize; i++) {
	        buf.append(StagedWrites.at(i).buffer());
		buflength += StagedWrites.at(i).bufferlength();
	    }

	    int fd = open(serverpath, O_WRONLY);

	    if (fd == -1) {
	        response->set_err(errno);
		return Status::OK;
	    }

	    res = pwrite(fd, buf.c_str(), buflength, offset);
	    if (res == -1) {
                response->set_err(errno);
                return Status::OK;
            }

            // synchronize file contents
            fsync(fd);
            // cout << "[DEBUG] Commit sync fd: " << fd << endl;
            if (fd > 0) {
                close(fd);
            }
	    // cout << "[DEBUG] Commit finish sync" << endl;
	    // cout << "[DEBUG] staged writes size: " << stagedWritesSize << endl;
	    // cout << "[DEBUG] restaged writes size: " << reStagedWritesSize << endl;
	    
	    for (int i = 0; i < stagedWritesSize; i++ ) {
	        StagedWrites.pop_back();
	    }

	    // cout << "pop staged " << stagedWritesSize << endl;

	    for (int i = 0; i < reStagedWritesSize; i++) {
	        reStagedWrites.pop_back();
	    }

	    // cout << " pop reStaged" << reStagedWritesSize << endl;

	    response->set_err(0);
	    return Status::OK;
	}
	
	Status nfs_recommit(ServerContext* context, const WriteRequestParams* request, WriteResponseParams* response) override {
            // cout << "[DEBUG] nfs_recommit" << endl;
	    WriteRequestParams _request(*request);

	    reStagedWrites.push_back(_request);
	    response->set_bufferlength(request->bufferlength());
	    response->set_err(0);

	    return Status::OK;
	} 
    
	Status nfs_unlink(ServerContext* context, const UnlinkRequestParams* request, VoidMessage* vmsg) override {
	    // cout << "[DEBUG] Unlink path:" << request->path().c_str() << endl;
	    char server_path[512] = {0};
	    translatePath(request->path().c_str(), server_path);
	    
	    int result = unlink(server_path);
	    if (result == -1) {
		// cout<< "Unlink fails." << endl;
	        perror(strerror(errno));
		vmsg->set_err(errno);
		return Status::OK;
	    }

	    vmsg->set_err(0);
	    return Status::OK;
	}

	Status nfs_mknod(ServerContext* context, const MknodRequestParams* request, VoidMessage* vmsg) override {
	    // cout << "[DEBUG] mknode path:" << request->path().c_str() << endl;
            char server_path[512] = {0};
	    translatePath(request->path().c_str(), server_path);
	    
	    int mode = request->mode();
	    int rdev = request->rdev();

	    int result;
	    if (S_ISFIFO(mode)) {
	        result = mkfifo(server_path, mode);
	    } else {
	        result = mknod(server_path, mode, rdev);
	    }

	    if (result == -1) {
                // cout<< "Unlink fails." << endl;
                perror(strerror(errno));
                vmsg->set_err(errno);
                return Status::OK;
            }

            vmsg->set_err(0);
            return Status::OK;
        }


	
    Status nfs_rename(ServerContext* context, const RenameRequestParams* request, VoidMessage* vmsg) override {
        // cout << "[DEBUG] rename from path:" << request->fp().c_str() << " to: " << request->tp().c_str() << endl;
	char from_path[512] = {0};
	char to_path[512] = {0};
	translatePath(request->fp().c_str(), from_path);
	translatePath(request->tp().c_str(), to_path);
      
	if (request->flag()) {
            perror(strerror(errno));
            vmsg->set_err(EINVAL);
	    vmsg->set_ret("rename fails");
	    return Status::OK;
	}
        
        int result = rename(from_path, to_path);
        if (result == -1) {
            perror(strerror(errno));
            vmsg->set_err(errno);
	    return Status::OK;
	}

        vmsg->set_err(0);
        return Status::OK;	
    }

        Status nfs_crash(ServerContext* context, const CrashRequestParams* request, CrashResponseParams* response) override {
	    exit(0);
	    return Status::OK;
	}    
/*
        Status nfs_utimens(ServerContext* context, const UtimensRequestParams* request, VoidMessage* vmsg) override {
	    cout << "[DEBUG] utimens" <<endl;
	    char server_path[512] = {0};
	    translatePath(request->path().c_str(), server_path);
            
	    struct timespec ts[2];
            ts[0].tv_sec = request->sec();
	    ts[0].tv_nsec = request->nsec();
	    ts[1].tv_sec = request->sec2();
	    ts[1].tv_nsec = request->nsec2();
            
	    int result = utimensat(AT_FDCWD, server_path, ts, AT_SYMLINK_NOFOLLOW);
	    if (result == -1) {
	        perror(strerror(errno));
		vmsg->set_err(errno);
		return Status::OK;
	    }

	    vmsg->set_err(0);
	    return Status::OK;
	}
*/
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
