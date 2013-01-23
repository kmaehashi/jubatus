// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

#include <pficommon/data/serialization.h>
#include <pficommon/data/serialization/unordered_map.h>
#include <pficommon/data/unordered_map.h>
#include "storage_type.hpp"
#include "sparse_matrix_storage.hpp"
#include "../common/key_manager.hpp"

namespace jubatus {
namespace storage {

class index_storage {
 public:
  index_storage();
  ~index_storage();

  virtual void get_diff(sparse_matrix_storage& sms) const = 0;
  virtual void set_mixed_and_clear_diff(sparse_matrix_storage& mixed) const = 0;

  virtual std::string name() const = 0;
 private:
};

}
}
