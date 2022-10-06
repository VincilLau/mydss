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

#ifndef MYDSS_INCLUDE_CMD_STRING_HPP_
#define MYDSS_INCLUDE_CMD_STRING_HPP_

#include <db/object.hpp>
#include <db/type.hpp>
#include <proto/piece.hpp>
#include <proto/req.hpp>
#include <variant>

namespace mydss::cmd {

class String : public db::Object {
 public:
  String(std::string value) : Object(db::type::kString, db::encoding::kRaw) {
    SetValue(std::move(value));
  }

  [[nodiscard]] uint16_t Type() const override { return db::type::kString; }
  [[nodiscard]] std::string TypeStr() const override { return "string"; }

  [[nodiscard]] uint16_t Encoding() const override { return encoding_; }
  [[nodiscard]] std::string EncodingStr() const override;

  [[nodiscard]] int64_t I64() const {
    assert(encoding_ == db::encoding::kInt);
    return std::get<int64_t>(value_);
  }
  [[nodiscard]] const std::string& Str() const {
    assert(encoding_ == db::encoding::kRaw);
    return std::get<std::string>(value_);
  }
  void SetValue(std::string value);

 public:
  static void Set(const proto::Req& req, std::shared_ptr<proto::Piece>& resp);
  static void Get(const proto::Req& req, std::shared_ptr<proto::Piece>& resp);

 private:
  std::variant<std::string, int64_t> value_;
  uint16_t encoding_;
};

inline std::string String::EncodingStr() const {
  if (encoding_ == db::encoding::kInt) {
    return "int";
  }
  if (encoding_ == db::encoding::kRaw) {
    return "raw";
  }
  assert(false);
}

}  // namespace mydss::cmd

#endif // MYDSS_INCLUDE_CMD_STRING_HPP_