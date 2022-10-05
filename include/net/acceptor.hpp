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

#ifndef MYDSS_INCLUDE_NET_ACCEPTOR_HPP_
#define MYDSS_INCLUDE_NET_ACCEPTOR_HPP_

#include <functional>

#include "addr.hpp"
#include "conn_.hpp"
#include "loop.hpp"

namespace mydss::net {

class Acceptor : public std::enable_shared_from_this<Acceptor> {
 public:
  // 接受连接事件的 handler
  using AcceptHandler = std::function<void()>;

  // 保证所有的 Acceptor 对象都由 std::shared_ptr 持有
  [[nodiscard]] static auto New(std::shared_ptr<Loop> loop) {
    return std::shared_ptr<Acceptor>(new Acceptor(loop));
  }

  // 将 Acceptor 绑定到 addr
  void Bind(const Addr& addr);

  // 开始接受连接
  void Listen(int backlog);

  // 异步接受连接
  void AsyncAccept(std::shared_ptr<Conn> conn, AcceptHandler handler);

 private:
  Acceptor(std::shared_ptr<Loop> loop);

  // 接受连接
  // 若成功，返回 true；若发生 EAGAIN 错误，返回 false
  bool Accept(std::shared_ptr<Conn> conn);

  // 处理可以建立连接的事件
  static void OnAccept(std::shared_ptr<Acceptor> acceptor);

 private:
  std::shared_ptr<Loop> loop_;  // 监听 listen_fd_ 的事件循环
  int listen_fd_;               // 监听套接字
  std::shared_ptr<Conn> conn_;  // 接受的 Conn 对象
  AcceptHandler handler_;       // 处理连接事件的 handler
};

}  // namespace mydss::net

#endif  // MYDSS_INCLUDE_NET_ACCEPTOR_HPP_
