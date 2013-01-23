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

#include "space_splitter.hpp"

#include <string>
#include <vector>

namespace jubatus {
namespace fv_converter {

using namespace std;

static const char* SPACES = " \t\f\n\r\v";

void space_splitter::split(
    const string& string, vector<pair<size_t, size_t> >& ret_boundaries) const {
  vector < pair<size_t, size_t> > bounds;

  size_t last = 0;
  while (true) {
    size_t begin = string.find_first_not_of(SPACES, last);
    if (begin == string::npos) {
      break;
    }

    size_t end = string.find_first_of(SPACES, begin);
    if (end == string::npos) {
      size_t len = string.size() - begin;
      bounds.push_back(make_pair(begin, len));
      break;
    } else {
      size_t len = end - begin;
      bounds.push_back(make_pair(begin, len));
      last = end;
    }
  }

  bounds.swap(ret_boundaries);
}

}
}
