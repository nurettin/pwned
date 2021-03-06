// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "product.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* Product_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Product_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_product_2eproto() {
  protobuf_AddDesc_product_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "product.proto");
  GOOGLE_CHECK(file != NULL);
  Product_descriptor_ = file->message_type(0);
  static const int Product_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Product, code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Product, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Product, price_),
  };
  Product_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Product_descriptor_,
      Product::default_instance_,
      Product_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Product, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Product, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Product));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_product_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Product_descriptor_, &Product::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_product_2eproto() {
  delete Product::default_instance_;
  delete Product_reflection_;
}

void protobuf_AddDesc_product_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rproduct.proto\"4\n\007Product\022\014\n\004code\030\001 \002(\t"
    "\022\014\n\004name\030\002 \001(\t\022\r\n\005price\030\003 \001(\002", 69);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "product.proto", &protobuf_RegisterTypes);
  Product::default_instance_ = new Product();
  Product::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_product_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_product_2eproto {
  StaticDescriptorInitializer_product_2eproto() {
    protobuf_AddDesc_product_2eproto();
  }
} static_descriptor_initializer_product_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int Product::kCodeFieldNumber;
const int Product::kNameFieldNumber;
const int Product::kPriceFieldNumber;
#endif  // !_MSC_VER

Product::Product()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Product::InitAsDefaultInstance() {
}

Product::Product(const Product& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Product::SharedCtor() {
  _cached_size_ = 0;
  code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  price_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Product::~Product() {
  SharedDtor();
}

void Product::SharedDtor() {
  if (code_ != &::google::protobuf::internal::kEmptyString) {
    delete code_;
  }
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (this != default_instance_) {
  }
}

void Product::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Product::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Product_descriptor_;
}

const Product& Product::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_product_2eproto();  return *default_instance_;
}

Product* Product::default_instance_ = NULL;

Product* Product::New() const {
  return new Product;
}

void Product::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_code()) {
      if (code_ != &::google::protobuf::internal::kEmptyString) {
        code_->clear();
      }
    }
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::kEmptyString) {
        name_->clear();
      }
    }
    price_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Product::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string code = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_code()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->code().data(), this->code().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_name;
        break;
      }
      
      // optional string name = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(29)) goto parse_price;
        break;
      }
      
      // optional float price = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_price:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &price_)));
          set_has_price();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Product::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string code = 1;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->code(), output);
  }
  
  // optional string name = 2;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->name(), output);
  }
  
  // optional float price = 3;
  if (has_price()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(3, this->price(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Product::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string code = 1;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->code(), target);
  }
  
  // optional string name = 2;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->name(), target);
  }
  
  // optional float price = 3;
  if (has_price()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(3, this->price(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Product::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string code = 1;
    if (has_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->code());
    }
    
    // optional string name = 2;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }
    
    // optional float price = 3;
    if (has_price()) {
      total_size += 1 + 4;
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Product::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Product* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Product*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Product::MergeFrom(const Product& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_code()) {
      set_code(from.code());
    }
    if (from.has_name()) {
      set_name(from.name());
    }
    if (from.has_price()) {
      set_price(from.price());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Product::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Product::CopyFrom(const Product& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Product::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void Product::Swap(Product* other) {
  if (other != this) {
    std::swap(code_, other->code_);
    std::swap(name_, other->name_);
    std::swap(price_, other->price_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Product::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Product_descriptor_;
  metadata.reflection = Product_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
