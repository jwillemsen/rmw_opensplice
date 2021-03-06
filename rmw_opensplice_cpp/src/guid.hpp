// Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GUID_HPP_
#define GUID_HPP_

#include <dds_dcps.h>

#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// TODO(ross-desmond): This should all be in opensplice
typedef char octet;

/**
 * Structure to hold GUID information for DDS instances.
 * http://www.eprosima.com/docs/fast-rtps/1.6.0/html/_guid_8h_source.html
 *
 */
struct GuidPrefix_t
{
  static constexpr unsigned int kSize = 12;
  octet value[kSize];

  GuidPrefix_t()
  {
    memset(value, 0, kSize);
  }

  explicit GuidPrefix_t(octet guid[kSize])
  {
    memcpy(value, guid, kSize);
  }

  GuidPrefix_t(const GuidPrefix_t & g)
  {
    memcpy(value, g.value, kSize);
  }

  GuidPrefix_t(GuidPrefix_t && g)
  {
    memmove(value, g.value, kSize);
  }

  GuidPrefix_t & operator=(const GuidPrefix_t & guidpre)
  {
    memcpy(value, guidpre.value, kSize);
    return *this;
  }

  GuidPrefix_t & operator=(GuidPrefix_t && guidpre)
  {
    memmove(value, guidpre.value, kSize);
    return *this;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC

  bool operator==(const GuidPrefix_t & prefix) const
  {
    return memcmp(value, prefix.value, kSize) == 0;
  }

  bool operator!=(const GuidPrefix_t & prefix) const
  {
    return memcmp(value, prefix.value, kSize) != 0;
  }

#endif
};

inline bool operator<(const GuidPrefix_t & g1, const GuidPrefix_t & g2)
{
  for (uint8_t i = 0; i < GuidPrefix_t::kSize; ++i) {
    if (g1.value[i] < g2.value[i]) {
      return true;
    } else if (g1.value[i] > g2.value[i]) {
      return false;
    }
  }
  return false;
}

inline std::ostream & operator<<(std::ostream & output, const GuidPrefix_t & guiP)
{
  output << std::hex;
  for (uint8_t i = 0; i < GuidPrefix_t::kSize - 1; ++i) {
    output << static_cast<int>(guiP.value[i]) << ".";
  }
  output << static_cast<int>(guiP.value[GuidPrefix_t::kSize - 1]);
  return output << std::dec;
}

// TODO(ross-desmond): check this with opensplice source code to ensure compatibility
/**
 * Convert an instance handle to a guid.
 *
 * @param guid [out] the resulting guid
 * @param domain_id to prepend to the guid
 * @param instance_handle to append to the guid
 */
inline void DDS_InstanceHandle_to_GUID(
  struct GuidPrefix_t * guid,
  DDS::DomainId_t domain_id,
  DDS::InstanceHandle_t instance_handle)
{
  memcpy(guid->value, reinterpret_cast<char *>(&domain_id), sizeof(DDS::DomainId_t));
  memcpy(guid->value + sizeof(DDS::DomainId_t),
    reinterpret_cast<char *>(&instance_handle), sizeof(DDS::InstanceHandle_t));
}

inline void DDS_BuiltinTopicKey_to_GUID(
  struct GuidPrefix_t * guid,
  DDS::BuiltinTopicKey_t buitinTopicKey)
{
#if BIG_ENDIAN
  /* Big Endian */
  memcpy(guid->value, reinterpret_cast<octet *>(buitinTopicKey), GuidPrefix_t::kSize);
#else
  /* Little Endian */
  int i;
  octet * topicKeyBuffer = reinterpret_cast<octet *>(buitinTopicKey);
  for (i = 0; i < 3; ++i) {
    octet * guidElement = &guid->value[i * 3];
    octet * keyBufferElement = reinterpret_cast<octet *>(&buitinTopicKey[i * 3]);
    guidElement[0] = keyBufferElement[2];
    guidElement[1] = keyBufferElement[1];
    guidElement[2] = keyBufferElement[0];
  }

#endif
}

#endif  // GUID_HPP_
