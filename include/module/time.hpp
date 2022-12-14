// Copyright 2022 Vincil Lau
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

#ifndef MYDSS_INCLUDE_MODULE_TIME_HPP_
#define MYDSS_INCLUDE_MODULE_TIME_HPP_

#include <sys/time.h>

#include <cstdint>

namespace mydss::module {

inline static int64_t TimeInMsec() {
  timeval now;
  gettimeofday(&now, nullptr);
  int64_t ts = now.tv_sec * 1000 + now.tv_usec / 1000;
  return ts;
}

}  // namespace mydss::module

#endif  // MYDSS_INCLUDE_MODULE_TIME_HPP_
