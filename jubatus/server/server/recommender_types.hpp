// This file is auto-generated from recommender.idl(0.5.2-68-g68e898d) with jenerator version 0.5.4-224-g49229fa/develop
// *** DO NOT EDIT ***

#ifndef JUBATUS_SERVER_SERVER_RECOMMENDER_TYPES_HPP_
#define JUBATUS_SERVER_SERVER_RECOMMENDER_TYPES_HPP_

#include <stdint.h>

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "jubatus/core/fv_converter/datum.hpp"
#include <msgpack.hpp>

namespace jubatus {

struct id_with_score {
 public:
  MSGPACK_DEFINE(id, score);
  std::string id;
  float score;
  id_with_score() {
  }
  id_with_score(const std::string& id, float score)
    : id(id), score(score) {
  }
};

}  // namespace jubatus

#endif  // JUBATUS_SERVER_SERVER_RECOMMENDER_TYPES_HPP_
