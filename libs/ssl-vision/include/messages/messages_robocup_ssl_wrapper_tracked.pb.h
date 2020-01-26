// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: messages_robocup_ssl_wrapper_tracked.proto

#ifndef PROTOBUF_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto__INCLUDED
#define PROTOBUF_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "messages_robocup_ssl_detection_tracked.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto();
void protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto();
void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto();

class TrackerWrapperPacket;

// ===================================================================

class TrackerWrapperPacket : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TrackerWrapperPacket) */ {
 public:
  TrackerWrapperPacket();
  virtual ~TrackerWrapperPacket();

  TrackerWrapperPacket(const TrackerWrapperPacket& from);

  inline TrackerWrapperPacket& operator=(const TrackerWrapperPacket& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TrackerWrapperPacket& default_instance();

  void Swap(TrackerWrapperPacket* other);

  // implements Message ----------------------------------------------

  inline TrackerWrapperPacket* New() const { return New(NULL); }

  TrackerWrapperPacket* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TrackerWrapperPacket& from);
  void MergeFrom(const TrackerWrapperPacket& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(TrackerWrapperPacket* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string uuid = 1;
  bool has_uuid() const;
  void clear_uuid();
  static const int kUuidFieldNumber = 1;
  const ::std::string& uuid() const;
  void set_uuid(const ::std::string& value);
  void set_uuid(const char* value);
  void set_uuid(const char* value, size_t size);
  ::std::string* mutable_uuid();
  ::std::string* release_uuid();
  void set_allocated_uuid(::std::string* uuid);

  // optional string source_name = 2;
  bool has_source_name() const;
  void clear_source_name();
  static const int kSourceNameFieldNumber = 2;
  const ::std::string& source_name() const;
  void set_source_name(const ::std::string& value);
  void set_source_name(const char* value);
  void set_source_name(const char* value, size_t size);
  ::std::string* mutable_source_name();
  ::std::string* release_source_name();
  void set_allocated_source_name(::std::string* source_name);

  // optional .TrackedFrame tracked_frame = 3;
  bool has_tracked_frame() const;
  void clear_tracked_frame();
  static const int kTrackedFrameFieldNumber = 3;
  const ::TrackedFrame& tracked_frame() const;
  ::TrackedFrame* mutable_tracked_frame();
  ::TrackedFrame* release_tracked_frame();
  void set_allocated_tracked_frame(::TrackedFrame* tracked_frame);

  // @@protoc_insertion_point(class_scope:TrackerWrapperPacket)
 private:
  inline void set_has_uuid();
  inline void clear_has_uuid();
  inline void set_has_source_name();
  inline void clear_has_source_name();
  inline void set_has_tracked_frame();
  inline void clear_has_tracked_frame();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr uuid_;
  ::google::protobuf::internal::ArenaStringPtr source_name_;
  ::TrackedFrame* tracked_frame_;
  friend void  protobuf_AddDesc_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto();
  friend void protobuf_AssignDesc_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto();
  friend void protobuf_ShutdownFile_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto();

  void InitAsDefaultInstance();
  static TrackerWrapperPacket* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TrackerWrapperPacket

// required string uuid = 1;
inline bool TrackerWrapperPacket::has_uuid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TrackerWrapperPacket::set_has_uuid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TrackerWrapperPacket::clear_has_uuid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TrackerWrapperPacket::clear_uuid() {
  uuid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_uuid();
}
inline const ::std::string& TrackerWrapperPacket::uuid() const {
  // @@protoc_insertion_point(field_get:TrackerWrapperPacket.uuid)
  return uuid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TrackerWrapperPacket::set_uuid(const ::std::string& value) {
  set_has_uuid();
  uuid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TrackerWrapperPacket.uuid)
}
inline void TrackerWrapperPacket::set_uuid(const char* value) {
  set_has_uuid();
  uuid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TrackerWrapperPacket.uuid)
}
inline void TrackerWrapperPacket::set_uuid(const char* value, size_t size) {
  set_has_uuid();
  uuid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TrackerWrapperPacket.uuid)
}
inline ::std::string* TrackerWrapperPacket::mutable_uuid() {
  set_has_uuid();
  // @@protoc_insertion_point(field_mutable:TrackerWrapperPacket.uuid)
  return uuid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TrackerWrapperPacket::release_uuid() {
  // @@protoc_insertion_point(field_release:TrackerWrapperPacket.uuid)
  clear_has_uuid();
  return uuid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TrackerWrapperPacket::set_allocated_uuid(::std::string* uuid) {
  if (uuid != NULL) {
    set_has_uuid();
  } else {
    clear_has_uuid();
  }
  uuid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), uuid);
  // @@protoc_insertion_point(field_set_allocated:TrackerWrapperPacket.uuid)
}

// optional string source_name = 2;
inline bool TrackerWrapperPacket::has_source_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TrackerWrapperPacket::set_has_source_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TrackerWrapperPacket::clear_has_source_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TrackerWrapperPacket::clear_source_name() {
  source_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_source_name();
}
inline const ::std::string& TrackerWrapperPacket::source_name() const {
  // @@protoc_insertion_point(field_get:TrackerWrapperPacket.source_name)
  return source_name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TrackerWrapperPacket::set_source_name(const ::std::string& value) {
  set_has_source_name();
  source_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TrackerWrapperPacket.source_name)
}
inline void TrackerWrapperPacket::set_source_name(const char* value) {
  set_has_source_name();
  source_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TrackerWrapperPacket.source_name)
}
inline void TrackerWrapperPacket::set_source_name(const char* value, size_t size) {
  set_has_source_name();
  source_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TrackerWrapperPacket.source_name)
}
inline ::std::string* TrackerWrapperPacket::mutable_source_name() {
  set_has_source_name();
  // @@protoc_insertion_point(field_mutable:TrackerWrapperPacket.source_name)
  return source_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TrackerWrapperPacket::release_source_name() {
  // @@protoc_insertion_point(field_release:TrackerWrapperPacket.source_name)
  clear_has_source_name();
  return source_name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TrackerWrapperPacket::set_allocated_source_name(::std::string* source_name) {
  if (source_name != NULL) {
    set_has_source_name();
  } else {
    clear_has_source_name();
  }
  source_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), source_name);
  // @@protoc_insertion_point(field_set_allocated:TrackerWrapperPacket.source_name)
}

// optional .TrackedFrame tracked_frame = 3;
inline bool TrackerWrapperPacket::has_tracked_frame() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void TrackerWrapperPacket::set_has_tracked_frame() {
  _has_bits_[0] |= 0x00000004u;
}
inline void TrackerWrapperPacket::clear_has_tracked_frame() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void TrackerWrapperPacket::clear_tracked_frame() {
  if (tracked_frame_ != NULL) tracked_frame_->::TrackedFrame::Clear();
  clear_has_tracked_frame();
}
inline const ::TrackedFrame& TrackerWrapperPacket::tracked_frame() const {
  // @@protoc_insertion_point(field_get:TrackerWrapperPacket.tracked_frame)
  return tracked_frame_ != NULL ? *tracked_frame_ : *default_instance_->tracked_frame_;
}
inline ::TrackedFrame* TrackerWrapperPacket::mutable_tracked_frame() {
  set_has_tracked_frame();
  if (tracked_frame_ == NULL) {
    tracked_frame_ = new ::TrackedFrame;
  }
  // @@protoc_insertion_point(field_mutable:TrackerWrapperPacket.tracked_frame)
  return tracked_frame_;
}
inline ::TrackedFrame* TrackerWrapperPacket::release_tracked_frame() {
  // @@protoc_insertion_point(field_release:TrackerWrapperPacket.tracked_frame)
  clear_has_tracked_frame();
  ::TrackedFrame* temp = tracked_frame_;
  tracked_frame_ = NULL;
  return temp;
}
inline void TrackerWrapperPacket::set_allocated_tracked_frame(::TrackedFrame* tracked_frame) {
  delete tracked_frame_;
  tracked_frame_ = tracked_frame;
  if (tracked_frame) {
    set_has_tracked_frame();
  } else {
    clear_has_tracked_frame();
  }
  // @@protoc_insertion_point(field_set_allocated:TrackerWrapperPacket.tracked_frame)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_messages_5frobocup_5fssl_5fwrapper_5ftracked_2eproto__INCLUDED