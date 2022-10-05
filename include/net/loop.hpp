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

#ifndef MYDSS_INCLUDE_NET_LOOP_HPP_
#define MYDSS_INCLUDE_NET_LOOP_HPP_

#include <functional>
#include <memory>
#include <unordered_map>

namespace mydss::net {

class Loop {
 public:
  using Handler = std::function<void()>;

  [[nodiscard]] static auto New() { return std::shared_ptr<Loop>(new Loop()); }

  void AddFd(int fd, Handler read_handler);
  void SetWriteHandler(int fd, Handler handler);
  void RemoveFd(int fd);
  void Run();

 private:
  Loop();

 private:
  int epfd_;
  std::unordered_map<int, std::pair<Handler, Handler>> fds_;
};

}  // namespace mydss::net

#endif  // MYDSS_INCLUDE_NET_LOOP_HPP_