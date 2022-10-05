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

#include <spdlog/spdlog.h>
#include <sys/epoll.h>

#include <err/errno.hpp>
#include <net/loop.hpp>

using mydss::err::ErrnoStr;

namespace mydss::net {

Loop::Loop() : epfd_(epoll_create(1)) {
  if (epfd_ == -1) {
    SPDLOG_CRITICAL("epoll_create error: {}", ErrnoStr());
    exit(EXIT_FAILURE);
  }
}

void Loop::Add(int fd, Handler handler) {
  assert(handler);                      // 读事件的 handler 不能为空
  assert(fds_.find(fd) == fds_.end());  // 不能重复添加 fd

  fds_[fd].first = std::move(handler);

  epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLET;
  int ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
  if (ret == -1) {
    SPDLOG_CRITICAL("epoll_ctl error: {}", ErrnoStr());
    exit(EXIT_FAILURE);
  }
}

void Loop::SetWriteHandler(int fd, Handler handler) {
  assert(fds_.find(fd) != fds_.end());
  const auto& old_handler = fds_.at(fd).second;
  assert(static_cast<bool>(handler) ^ static_cast<bool>(old_handler));

  fds_.at(fd).second = std::move(handler);

  epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLET;
  ev.events |= handler ? EPOLLOUT : 0;
  int ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
  if (ret == -1) {
    SPDLOG_CRITICAL("epoll_ctl error: {}", ErrnoStr());
    exit(EXIT_FAILURE);
  }
}

void Loop::Remove(int fd) {
  assert(fds_.find(fd) != fds_.end());

  fds_.erase(fd);

  int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
  if (ret == -1) {
    SPDLOG_CRITICAL("epoll_ctl error: {}", ErrnoStr());
    exit(EXIT_FAILURE);
  }
}

void Loop::Run() {
  for (;;) {
    assert(fds_.size() > 0);

    epoll_event ev;
    int ret = epoll_wait(epfd_, &ev, 1, -1);
    assert(ret != 0);
    if (ret == -1) {
      SPDLOG_CRITICAL("epoll_wait error: {}", ErrnoStr());
      exit(EXIT_FAILURE);
    }

    if (ev.events & EPOLLIN) {
      const auto& handler = fds_.at(ev.data.fd).first;
      assert(handler);
      handler();
    }

    if (ev.events & EPOLLOUT) {
      const auto& handler = fds_.at(ev.data.fd).second;
      assert(handler);
      handler();
    }
  }
}

}  // namespace mydss::net
