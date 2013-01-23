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

#include <gtest/gtest.h>

#include <pficommon/lang/scoped_ptr.h>
#include "num_filter_factory.hpp"
#include "exception.hpp"
#include "num_filter_impl.hpp"

using namespace std;
using namespace jubatus::fv_converter;
using namespace pfi::lang;

namespace jubatus {

TEST(num_filter_factory, unknown_name) {
  num_filter_factory f;
  EXPECT_THROW(f.create("unknonw", map<string, string>()),
      converter_exception);
}

TEST(num_filter_factory, create_add_filter_with_illegal_config) {
  num_filter_factory f;
  map<string, string> params;
  EXPECT_THROW(f.create("add", params),
      converter_exception);
  params["value"] = "hoge";
  EXPECT_THROW(f.create("add", params),
      bad_cast);
}

TEST(num_filter_factory, create_add_filter) {
  num_filter_factory f;

  map<string, string> params;
  params["value"] = "10";
  scoped_ptr<num_filter> filter(f.create("add", params));

  EXPECT_EQ(typeid(add_filter).name(), typeid(*filter).name());
  EXPECT_EQ(20.0, filter->filter(10.0));
}

TEST(num_filter_factory, create_dynamic) {
  num_filter_factory f;

  map<string, string> params;
  EXPECT_THROW(f.create("dynamic", params),
      converter_exception);

  params["path"] = "unknown_file_name";
  EXPECT_THROW(f.create("dynamic", params),
      converter_exception);

  params["path"] = LIBNUM_FILTER_SAMPLE;
  EXPECT_THROW(f.create("dynamic", params),
      converter_exception);

  params["function"] = "unknown_func";
  EXPECT_THROW(f.create("dynamic", params),
      converter_exception);

  params["function"] = "create";
  scoped_ptr<num_filter> s(f.create("dynamic", params));

  EXPECT_EQ(6.0, s->filter(3.0));
}

}
