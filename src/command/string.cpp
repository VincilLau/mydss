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

#include <command/string.hpp>

using std::make_shared;
using std::shared_ptr;

namespace mydss::string {
Status Set(const Instance& inst, const Req& req, shared_ptr<Piece>& piece) {
  SPDLOG_DEBUG("SET");
  piece = make_shared<SimpleStringPiece>("OK");
  return Status::Ok();
}

}  // namespace mydss::string
