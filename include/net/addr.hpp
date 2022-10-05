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

#ifndef MYDSS_INCLUDE_NET_ADDR_HPP_
#define MYDSS_INCLUDE_NET_ADDR_HPP_

#include <arpa/inet.h>
#include <fmt/ostream.h>

#include <cstdint>
#include <string>

namespace mydss::net {

// IPv4 地址
class Addr {
 public:
  // 存储点分 IPv4 的字符串，包含结尾的 '\0' 最多 16 个字节
  static constexpr size_t kInetNtopBufSize = 16;

  Addr() : ip_{}, port_(0) {}
  Addr(std::string ip, uint16_t port) : ip_(std::move(ip)), port_(port) {}

  [[nodiscard]] const auto& ip() const { return ip_; }
  [[nodiscard]] auto port() const { return port_; }

  // 将 Addr 转换为 struct sockaddr 的表示形式
  // 若成功返回 true，否则返回 false
  [[nodiscard]] bool ToSockAddr(sockaddr& sock_addr) const;

  // 将 struct sockaddr 转换为 Addr
  // 若成功返回 true，否则返回 false
  [[nodiscard]] bool FromSockAddr(const sockaddr& sock_addr);

 private:
  std::string ip_;  // 点分 IPv4 地址
  uint16_t port_;   // 端口号
};

// 向 std::ostream 输出 Addr 的字符串表示形式，用于 fmt 库
// 格式为 'ip:port'
inline static std::ostream& operator<<(std::ostream& os, const Addr& addr) {
  os << addr.ip() << ':' << addr.port();
  return os;
}

}  // namespace mydss::net

#endif  // MYDSS_INCLUDE_NET_ADDR_HPP_
