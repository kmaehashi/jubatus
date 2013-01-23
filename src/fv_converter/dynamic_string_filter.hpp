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

#include <map>
#include <pficommon/lang/scoped_ptr.h>
#include "string_filter.hpp"
#include "dynamic_loader.hpp"

namespace jubatus {
namespace fv_converter {

class dynamic_string_filter : public string_filter {
 public:
  dynamic_string_filter(const std::string& path, const std::string& function,
                        const std::map<std::string, std::string>& params);

  void filter(const std::string& input, std::string& output) const;

 private:
  dynamic_loader loader_;
  pfi::lang::scoped_ptr<string_filter> impl_;
};

}
}
