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

#include <ux/ux.hpp>
#include "../../fv_converter/word_splitter.hpp"

namespace jubatus {

class ux_splitter : public fv_converter::word_splitter {
 public:
  ux_splitter(const std::vector<std::string>& keywords);
  ~ux_splitter();

  void split(const std::string& string,
             std::vector<std::pair<size_t, size_t> >& ret_boundaries) const;

 private:
  ux::Trie trie_;
};

}

extern "C" {
jubatus::ux_splitter*
create(const std::map<std::string, std::string>& params);
}
