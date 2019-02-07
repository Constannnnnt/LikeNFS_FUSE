
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "nfsFuse.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using nfsFuse::NFSFuse;
using nfsFuse::GetAttrRequestParams;
using nfsFuse::GetAttrResponseParams;
using nfsFuse::ReadDirRequestParams;
using nfsFuse::ReadDirResponseParams;

class nfsFuseClient {
    public:
        nfsFuseClient(std::shared_ptr<Channel> channel) : stub_(NFSFuse::NewStub(channel)) {
	}
    private:
        std::unique_ptr<NFSFuse::Stub> stub_;
};

int main(int argc, char** argv) {
    /*
     * Specify a mount point through arguments, wait to implement
     * The third argument of client should be the mount point
     */ 
    nfsFuseClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  return 0;
}
