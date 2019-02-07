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
    private:
	std::unique_ptr<NFSFuse::Stub> stub_;
};
