// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#include "datum.hpp"
#include "counter.hpp"
#include <pficommon/data/unordered_map.h>
#include "../common/type.hpp"
#include "keyword_weights.hpp"

namespace jubatus {
namespace fv_converter {

class weight_manager {
 public:
  weight_manager();
  
  void update_weight(const sfv_t& fv);
  void get_weight(sfv_t& fv)const;

  void add_weight(const std::string& key, float weight);

  const keyword_weights& get_diff() const {
    return diff_weights_;
  }

  void put_diff(const keyword_weights& diff) {
    master_weights_.merge(diff);
    diff_weights_.clear();
  }

  void save(std::ostream& os){
    pfi::data::serialization::binary_oarchive oa(os);
    oa << diff_weights_;
    oa << master_weights_;
  }
  void load(std::istream& is){
    pfi::data::serialization::binary_iarchive ia(is);
    ia >> diff_weights_;
    ia >> master_weights_;
  }
  void pretty_print(std::ostream& os){
    os << "# diff_weights" << std::endl;
    diff_weights_.pretty_print(os);
    os << "# master_weights" << std::endl;
    master_weights_.pretty_print(os);
  }

  template <class Archiver>
  void serialize(Archiver &ar) {
    ar
      & MEMBER(diff_weights_)
      & MEMBER(master_weights_);
  }
 private:
  size_t get_document_count() const {
    return diff_weights_.get_document_count()
        + master_weights_.get_document_count();
  }

  size_t get_document_frequency(const std::string& key) const {
    return diff_weights_.get_document_frequency(key)
        + master_weights_.get_document_frequency(key);
  }

  double get_user_weight(const std::string& key) const {
    return diff_weights_.get_user_weight(key)
        + master_weights_.get_user_weight(key);
  }


  double get_global_weight(const std::string& key) const;

  keyword_weights diff_weights_;
  keyword_weights master_weights_;

};

}
}
