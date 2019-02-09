
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
using nfsFuse::NFSFuse;
using nfsFuse::GetAttrRequestParams;
using nfsFuse::GetAttrResponseParams;
using nfsFuse::ReadDirRequestParams;
using nfsFuse::ReadDirResponseParams;

using namespace std;

class nfsFuseImpl final : public NFSFuse::Service {
/*    public:
        explicit nfsFuseImpl(const std::string& mountpoint) {
            
        }
  */  
    Status nfs_getattr(ServerContext* context, const GetAttrRequestParams* request, GetAttrResponseParams* response)  {
        // print out the client path 
	cout<<"Get Attribute:"<<endl;
	std::cout<<"\tinit path: "<<request->path().c_str()<<std::endl;

        // translate the server path
        char server_path[512] = {0};
        //strcat(server_path, "./server");
        strcat(server_path, "/home/ubuntu/LikeNFS_FUSE/server");
	strcat(server_path + 9, request->path().c_str());
        server_path[strlen(server_path)] = '\0';
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
	
	char server_path[512]={0};
        strcat(server_path, "./server");
        strcat(server_path + 8, request->path().c_str());
        server_path[strlen(server_path)] = '\0';

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
    
    private:
};

void RunServer() {

  // Specify the server address
  std::string server_address("0.0.0.0:50051");
  //nfsFuseImpl service(mountpoint);
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

    /*
     * Specify a mount point through arguments, wait to implement
     * 
     */
    std::string mountpoint = "path here";
    //RunServer(mountpoint);
    RunServer();
    return 0;
}
