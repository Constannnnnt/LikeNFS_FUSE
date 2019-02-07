
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
using nfsFuse::NFSFuse;
using nfsFuse::GetAttrRequestParams;
using nfsFuse::GetAttrResponseParams;
using nfsFuse::ReadDirRequestParams;
using nfsFuse::ReadDirResponseParams;

class nfsFuseImpl final : public NFSFuse::Service {
    public:
        explicit nfsFuseImpl(const std::string& mountpoint) {
            
        }
    /*
    Status getattr(ServerContext* context, GetAttrRequestParams* request, GetAttrResponseParams* response) override {
        return Status::OK;
    }

    Status readattr(ServerContext* context, ReadDirRequestParams* request, ReadDirResponseParams* reponse) override {
        return Status::OK;
    }
    */
    private:
};

void RunServer(const std::string& mountpoint) {

  // Specify the server address
  std::string server_address("0.0.0.0:50051");
  nfsFuseImpl service(mountpoint);
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
    RunServer(mountpoint);
    
    return 0;
}
