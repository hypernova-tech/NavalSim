// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: grid_description.proto

#include "grid_description.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace proto {
namespace grid_description {
constexpr GridDescription::GridDescription(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : hor_angles_()
  , ver_angles_()
  , max_range_(0)
  , mode_(0)
{}
struct GridDescriptionDefaultTypeInternal {
  constexpr GridDescriptionDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~GridDescriptionDefaultTypeInternal() {}
  union {
    GridDescription _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT GridDescriptionDefaultTypeInternal _GridDescription_default_instance_;
}  // namespace grid_description
}  // namespace proto
namespace proto {
namespace grid_description {
bool GridDescription_GridMode_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

static ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<std::string> GridDescription_GridMode_strings[2] = {};

static const char GridDescription_GridMode_names[] =
  "kAlternating"
  "kFixed";

static const ::PROTOBUF_NAMESPACE_ID::internal::EnumEntry GridDescription_GridMode_entries[] = {
  { {GridDescription_GridMode_names + 0, 12}, 1 },
  { {GridDescription_GridMode_names + 12, 6}, 0 },
};

static const int GridDescription_GridMode_entries_by_number[] = {
  1, // 0 -> kFixed
  0, // 1 -> kAlternating
};

const std::string& GridDescription_GridMode_Name(
    GridDescription_GridMode value) {
  static const bool dummy =
      ::PROTOBUF_NAMESPACE_ID::internal::InitializeEnumStrings(
          GridDescription_GridMode_entries,
          GridDescription_GridMode_entries_by_number,
          2, GridDescription_GridMode_strings);
  (void) dummy;
  int idx = ::PROTOBUF_NAMESPACE_ID::internal::LookUpEnumName(
      GridDescription_GridMode_entries,
      GridDescription_GridMode_entries_by_number,
      2, value);
  return idx == -1 ? ::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString() :
                     GridDescription_GridMode_strings[idx].get();
}
bool GridDescription_GridMode_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, GridDescription_GridMode* value) {
  int int_value;
  bool success = ::PROTOBUF_NAMESPACE_ID::internal::LookUpEnumValue(
      GridDescription_GridMode_entries, 2, name, &int_value);
  if (success) {
    *value = static_cast<GridDescription_GridMode>(int_value);
  }
  return success;
}
#if (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))
constexpr GridDescription_GridMode GridDescription::kFixed;
constexpr GridDescription_GridMode GridDescription::kAlternating;
constexpr GridDescription_GridMode GridDescription::GridMode_MIN;
constexpr GridDescription_GridMode GridDescription::GridMode_MAX;
constexpr int GridDescription::GridMode_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))

// ===================================================================

class GridDescription::_Internal {
 public:
  using HasBits = decltype(std::declval<GridDescription>()._has_bits_);
  static void set_has_mode(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_max_range(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

GridDescription::GridDescription(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(arena, is_message_owned),
  hor_angles_(arena),
  ver_angles_(arena) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:proto.grid_description.GridDescription)
}
GridDescription::GridDescription(const GridDescription& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(),
      _has_bits_(from._has_bits_),
      hor_angles_(from.hor_angles_),
      ver_angles_(from.ver_angles_) {
  _internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
  ::memcpy(&max_range_, &from.max_range_,
    static_cast<size_t>(reinterpret_cast<char*>(&mode_) -
    reinterpret_cast<char*>(&max_range_)) + sizeof(mode_));
  // @@protoc_insertion_point(copy_constructor:proto.grid_description.GridDescription)
}

inline void GridDescription::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&max_range_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&mode_) -
    reinterpret_cast<char*>(&max_range_)) + sizeof(mode_));
}

GridDescription::~GridDescription() {
  // @@protoc_insertion_point(destructor:proto.grid_description.GridDescription)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<std::string>();
}

inline void GridDescription::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void GridDescription::ArenaDtor(void* object) {
  GridDescription* _this = reinterpret_cast< GridDescription* >(object);
  (void)_this;
}
void GridDescription::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void GridDescription::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void GridDescription::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.grid_description.GridDescription)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  hor_angles_.Clear();
  ver_angles_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&max_range_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&mode_) -
        reinterpret_cast<char*>(&max_range_)) + sizeof(mode_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<std::string>();
}

const char* GridDescription::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // optional .proto.grid_description.GridDescription.GridMode mode = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::proto::grid_description::GridDescription_GridMode_IsValid(val))) {
            _internal_set_mode(static_cast<::proto::grid_description::GridDescription_GridMode>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(1, val, mutable_unknown_fields());
          }
        } else
          goto handle_unusual;
        continue;
      // repeated double hor_angles = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 17)) {
          ptr -= 1;
          do {
            ptr += 1;
            _internal_add_hor_angles(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr));
            ptr += sizeof(double);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<17>(ptr));
        } else if (static_cast<uint8_t>(tag) == 18) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedDoubleParser(_internal_mutable_hor_angles(), ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // repeated double ver_angles = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 25)) {
          ptr -= 1;
          do {
            ptr += 1;
            _internal_add_ver_angles(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr));
            ptr += sizeof(double);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<25>(ptr));
        } else if (static_cast<uint8_t>(tag) == 26) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedDoubleParser(_internal_mutable_ver_angles(), ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // optional double max_range = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 33)) {
          _Internal::set_has_max_range(&has_bits);
          max_range_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<std::string>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* GridDescription::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.grid_description.GridDescription)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .proto.grid_description.GridDescription.GridMode mode = 1;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_mode(), target);
  }

  // repeated double hor_angles = 2;
  for (int i = 0, n = this->_internal_hor_angles_size(); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(2, this->_internal_hor_angles(i), target);
  }

  // repeated double ver_angles = 3;
  for (int i = 0, n = this->_internal_ver_angles_size(); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(3, this->_internal_ver_angles(i), target);
  }

  // optional double max_range = 4;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(4, this->_internal_max_range(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).data(),
        static_cast<int>(_internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.grid_description.GridDescription)
  return target;
}

size_t GridDescription::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.grid_description.GridDescription)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated double hor_angles = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_hor_angles_size());
    size_t data_size = 8UL * count;
    total_size += 1 *
                  ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(this->_internal_hor_angles_size());
    total_size += data_size;
  }

  // repeated double ver_angles = 3;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_ver_angles_size());
    size_t data_size = 8UL * count;
    total_size += 1 *
                  ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(this->_internal_ver_angles_size());
    total_size += data_size;
  }

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    // optional double max_range = 4;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 + 8;
    }

    // optional .proto.grid_description.GridDescription.GridMode mode = 1;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_mode());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString).size();
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void GridDescription::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::PROTOBUF_NAMESPACE_ID::internal::DownCast<const GridDescription*>(
      &from));
}

void GridDescription::MergeFrom(const GridDescription& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto.grid_description.GridDescription)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  hor_angles_.MergeFrom(from.hor_angles_);
  ver_angles_.MergeFrom(from.ver_angles_);
  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      max_range_ = from.max_range_;
    }
    if (cached_has_bits & 0x00000002u) {
      mode_ = from.mode_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
  _internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
}

void GridDescription::CopyFrom(const GridDescription& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.grid_description.GridDescription)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GridDescription::IsInitialized() const {
  return true;
}

void GridDescription::InternalSwap(GridDescription* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  hor_angles_.InternalSwap(&other->hor_angles_);
  ver_angles_.InternalSwap(&other->ver_angles_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(GridDescription, mode_)
      + sizeof(GridDescription::mode_)
      - PROTOBUF_FIELD_OFFSET(GridDescription, max_range_)>(
          reinterpret_cast<char*>(&max_range_),
          reinterpret_cast<char*>(&other->max_range_));
}

std::string GridDescription::GetTypeName() const {
  return "proto.grid_description.GridDescription";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace grid_description
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::grid_description::GridDescription* Arena::CreateMaybeMessage< ::proto::grid_description::GridDescription >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::grid_description::GridDescription >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
