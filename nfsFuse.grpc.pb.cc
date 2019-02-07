// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: nfsFuse.proto

#include "nfsFuse.pb.h"
#include "nfsFuse.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace nfsFuse {

static const char* NFSFuse_method_names[] = {
  "/nfsFuse.NFSFuse/getattr",
  "/nfsFuse.NFSFuse/readdir",
  "/nfsFuse.NFSFuse/mkdir",
  "/nfsFuse.NFSFuse/rmdir",
  "/nfsFuse.NFSFuse/create",
  "/nfsFuse.NFSFuse/open",
  "/nfsFuse.NFSFuse/read",
  "/nfsFuse.NFSFuse/write",
};

std::unique_ptr< NFSFuse::Stub> NFSFuse::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< NFSFuse::Stub> stub(new NFSFuse::Stub(channel));
  return stub;
}

NFSFuse::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_getattr_(NFSFuse_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_readdir_(NFSFuse_method_names[1], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_mkdir_(NFSFuse_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_rmdir_(NFSFuse_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_create_(NFSFuse_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_open_(NFSFuse_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_read_(NFSFuse_method_names[6], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_write_(NFSFuse_method_names[7], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status NFSFuse::Stub::getattr(::grpc::ClientContext* context, const ::nfsFuse::GetAttrRequestParams& request, ::nfsFuse::GetAttrResponseParams* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getattr_, context, request, response);
}

void NFSFuse::Stub::experimental_async::getattr(::grpc::ClientContext* context, const ::nfsFuse::GetAttrRequestParams* request, ::nfsFuse::GetAttrResponseParams* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_getattr_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::GetAttrResponseParams>* NFSFuse::Stub::AsyncgetattrRaw(::grpc::ClientContext* context, const ::nfsFuse::GetAttrRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::GetAttrResponseParams>::Create(channel_.get(), cq, rpcmethod_getattr_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::GetAttrResponseParams>* NFSFuse::Stub::PrepareAsyncgetattrRaw(::grpc::ClientContext* context, const ::nfsFuse::GetAttrRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::GetAttrResponseParams>::Create(channel_.get(), cq, rpcmethod_getattr_, context, request, false);
}

::grpc::ClientReader< ::nfsFuse::ReadDirResponseParams>* NFSFuse::Stub::readdirRaw(::grpc::ClientContext* context, const ::nfsFuse::ReadDirRequestParams& request) {
  return ::grpc::internal::ClientReaderFactory< ::nfsFuse::ReadDirResponseParams>::Create(channel_.get(), rpcmethod_readdir_, context, request);
}

void NFSFuse::Stub::experimental_async::readdir(::grpc::ClientContext* context, ::nfsFuse::ReadDirRequestParams* request, ::grpc::experimental::ClientReadReactor< ::nfsFuse::ReadDirResponseParams>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::nfsFuse::ReadDirResponseParams>::Create(stub_->channel_.get(), stub_->rpcmethod_readdir_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::nfsFuse::ReadDirResponseParams>* NFSFuse::Stub::AsyncreaddirRaw(::grpc::ClientContext* context, const ::nfsFuse::ReadDirRequestParams& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::nfsFuse::ReadDirResponseParams>::Create(channel_.get(), cq, rpcmethod_readdir_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::nfsFuse::ReadDirResponseParams>* NFSFuse::Stub::PrepareAsyncreaddirRaw(::grpc::ClientContext* context, const ::nfsFuse::ReadDirRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::nfsFuse::ReadDirResponseParams>::Create(channel_.get(), cq, rpcmethod_readdir_, context, request, false, nullptr);
}

::grpc::Status NFSFuse::Stub::mkdir(::grpc::ClientContext* context, const ::nfsFuse::MkDirRequestParams& request, ::nfsFuse::VoidMessage* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_mkdir_, context, request, response);
}

void NFSFuse::Stub::experimental_async::mkdir(::grpc::ClientContext* context, const ::nfsFuse::MkDirRequestParams* request, ::nfsFuse::VoidMessage* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_mkdir_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::VoidMessage>* NFSFuse::Stub::AsyncmkdirRaw(::grpc::ClientContext* context, const ::nfsFuse::MkDirRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::VoidMessage>::Create(channel_.get(), cq, rpcmethod_mkdir_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::VoidMessage>* NFSFuse::Stub::PrepareAsyncmkdirRaw(::grpc::ClientContext* context, const ::nfsFuse::MkDirRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::VoidMessage>::Create(channel_.get(), cq, rpcmethod_mkdir_, context, request, false);
}

::grpc::Status NFSFuse::Stub::rmdir(::grpc::ClientContext* context, const ::nfsFuse::RmDirRequestParams& request, ::nfsFuse::VoidMessage* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_rmdir_, context, request, response);
}

void NFSFuse::Stub::experimental_async::rmdir(::grpc::ClientContext* context, const ::nfsFuse::RmDirRequestParams* request, ::nfsFuse::VoidMessage* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_rmdir_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::VoidMessage>* NFSFuse::Stub::AsyncrmdirRaw(::grpc::ClientContext* context, const ::nfsFuse::RmDirRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::VoidMessage>::Create(channel_.get(), cq, rpcmethod_rmdir_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::VoidMessage>* NFSFuse::Stub::PrepareAsyncrmdirRaw(::grpc::ClientContext* context, const ::nfsFuse::RmDirRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::VoidMessage>::Create(channel_.get(), cq, rpcmethod_rmdir_, context, request, false);
}

::grpc::Status NFSFuse::Stub::create(::grpc::ClientContext* context, const ::nfsFuse::CreateRequestParams& request, ::nfsFuse::CreateResponseParams* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_create_, context, request, response);
}

void NFSFuse::Stub::experimental_async::create(::grpc::ClientContext* context, const ::nfsFuse::CreateRequestParams* request, ::nfsFuse::CreateResponseParams* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_create_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::CreateResponseParams>* NFSFuse::Stub::AsynccreateRaw(::grpc::ClientContext* context, const ::nfsFuse::CreateRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::CreateResponseParams>::Create(channel_.get(), cq, rpcmethod_create_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::CreateResponseParams>* NFSFuse::Stub::PrepareAsynccreateRaw(::grpc::ClientContext* context, const ::nfsFuse::CreateRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::CreateResponseParams>::Create(channel_.get(), cq, rpcmethod_create_, context, request, false);
}

::grpc::Status NFSFuse::Stub::open(::grpc::ClientContext* context, const ::nfsFuse::OpenRequestParams& request, ::nfsFuse::OpenResponseParams* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_open_, context, request, response);
}

void NFSFuse::Stub::experimental_async::open(::grpc::ClientContext* context, const ::nfsFuse::OpenRequestParams* request, ::nfsFuse::OpenResponseParams* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_open_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::OpenResponseParams>* NFSFuse::Stub::AsyncopenRaw(::grpc::ClientContext* context, const ::nfsFuse::OpenRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::OpenResponseParams>::Create(channel_.get(), cq, rpcmethod_open_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::OpenResponseParams>* NFSFuse::Stub::PrepareAsyncopenRaw(::grpc::ClientContext* context, const ::nfsFuse::OpenRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::OpenResponseParams>::Create(channel_.get(), cq, rpcmethod_open_, context, request, false);
}

::grpc::Status NFSFuse::Stub::read(::grpc::ClientContext* context, const ::nfsFuse::ReadRequestParams& request, ::nfsFuse::ReadResponseParams* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_read_, context, request, response);
}

void NFSFuse::Stub::experimental_async::read(::grpc::ClientContext* context, const ::nfsFuse::ReadRequestParams* request, ::nfsFuse::ReadResponseParams* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_read_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::ReadResponseParams>* NFSFuse::Stub::AsyncreadRaw(::grpc::ClientContext* context, const ::nfsFuse::ReadRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::ReadResponseParams>::Create(channel_.get(), cq, rpcmethod_read_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::ReadResponseParams>* NFSFuse::Stub::PrepareAsyncreadRaw(::grpc::ClientContext* context, const ::nfsFuse::ReadRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::ReadResponseParams>::Create(channel_.get(), cq, rpcmethod_read_, context, request, false);
}

::grpc::Status NFSFuse::Stub::write(::grpc::ClientContext* context, const ::nfsFuse::WriteRequestParams& request, ::nfsFuse::WriteReponseParams* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_write_, context, request, response);
}

void NFSFuse::Stub::experimental_async::write(::grpc::ClientContext* context, const ::nfsFuse::WriteRequestParams* request, ::nfsFuse::WriteReponseParams* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_write_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::WriteReponseParams>* NFSFuse::Stub::AsyncwriteRaw(::grpc::ClientContext* context, const ::nfsFuse::WriteRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::WriteReponseParams>::Create(channel_.get(), cq, rpcmethod_write_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::nfsFuse::WriteReponseParams>* NFSFuse::Stub::PrepareAsyncwriteRaw(::grpc::ClientContext* context, const ::nfsFuse::WriteRequestParams& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::nfsFuse::WriteReponseParams>::Create(channel_.get(), cq, rpcmethod_write_, context, request, false);
}

NFSFuse::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::GetAttrRequestParams, ::nfsFuse::GetAttrResponseParams>(
          std::mem_fn(&NFSFuse::Service::getattr), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[1],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< NFSFuse::Service, ::nfsFuse::ReadDirRequestParams, ::nfsFuse::ReadDirResponseParams>(
          std::mem_fn(&NFSFuse::Service::readdir), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::MkDirRequestParams, ::nfsFuse::VoidMessage>(
          std::mem_fn(&NFSFuse::Service::mkdir), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::RmDirRequestParams, ::nfsFuse::VoidMessage>(
          std::mem_fn(&NFSFuse::Service::rmdir), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::CreateRequestParams, ::nfsFuse::CreateResponseParams>(
          std::mem_fn(&NFSFuse::Service::create), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::OpenRequestParams, ::nfsFuse::OpenResponseParams>(
          std::mem_fn(&NFSFuse::Service::open), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::ReadRequestParams, ::nfsFuse::ReadResponseParams>(
          std::mem_fn(&NFSFuse::Service::read), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      NFSFuse_method_names[7],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< NFSFuse::Service, ::nfsFuse::WriteRequestParams, ::nfsFuse::WriteReponseParams>(
          std::mem_fn(&NFSFuse::Service::write), this)));
}

NFSFuse::Service::~Service() {
}

::grpc::Status NFSFuse::Service::getattr(::grpc::ServerContext* context, const ::nfsFuse::GetAttrRequestParams* request, ::nfsFuse::GetAttrResponseParams* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::readdir(::grpc::ServerContext* context, const ::nfsFuse::ReadDirRequestParams* request, ::grpc::ServerWriter< ::nfsFuse::ReadDirResponseParams>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::mkdir(::grpc::ServerContext* context, const ::nfsFuse::MkDirRequestParams* request, ::nfsFuse::VoidMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::rmdir(::grpc::ServerContext* context, const ::nfsFuse::RmDirRequestParams* request, ::nfsFuse::VoidMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::create(::grpc::ServerContext* context, const ::nfsFuse::CreateRequestParams* request, ::nfsFuse::CreateResponseParams* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::open(::grpc::ServerContext* context, const ::nfsFuse::OpenRequestParams* request, ::nfsFuse::OpenResponseParams* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::read(::grpc::ServerContext* context, const ::nfsFuse::ReadRequestParams* request, ::nfsFuse::ReadResponseParams* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NFSFuse::Service::write(::grpc::ServerContext* context, const ::nfsFuse::WriteRequestParams* request, ::nfsFuse::WriteReponseParams* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace nfsFuse

