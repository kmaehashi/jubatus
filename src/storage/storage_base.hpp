// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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
#include <string>
#include <utility>
#include <stdexcept>
#include "storage_type.hpp"
#include "../common/exception.hpp"
#include "../common/type.hpp"

#include <iostream>

namespace jubatus {
namespace storage{

class storage_base {
public:
  virtual ~storage_base() {}

  virtual void get(const std::string &feature, feature_val1_t& ret) = 0;
  virtual void get2(const std::string &feature, feature_val2_t& ret) = 0;
  virtual void get3(const std::string &feature, feature_val3_t& ret) = 0;

  virtual void inp(const sfv_t& sfv, map_feature_val1_t& ret); /// inner product 

  virtual void set(const std::string &feature, const std::string &klass, const val1_t& w) = 0;
  virtual void set2(const std::string &feature, const std::string &klass, const val2_t& w) = 0;
  virtual void set3(const std::string &feature, const std::string &klass, const val3_t& w) = 0;

  virtual void get_status(std::map<std::string,std::string>&) = 0;

  virtual bool save(std::ostream&) = 0;
  virtual bool load(std::istream&) = 0;
  virtual bool pretty_print(std::ostream& os) {
    os << "pretty_print not implemented for this kind of storage\n";
    return false;
  }

  virtual void update(const std::string& feature, const std::string& inc_class, const std::string& dec_class, const val1_t& w);

  virtual void bulk_update(const sfv_t& sfv, float step_width, const std::string& inc_class, const std::string& dec_class);

  virtual void get_diff(features3_t&) const ;
  virtual void set_average_and_clear_diff(const features3_t&);

  virtual std::string type() const = 0;

};

class storage_exception : public jubatus::exception::jubaexception<storage_exception> {
public:
  storage_exception(const std::string &msg): msg(msg) {}
  ~storage_exception() throw() {}
  const char *what() const throw() { return msg.c_str(); }
private:
  std::string msg;
};

}
} // jubatus
