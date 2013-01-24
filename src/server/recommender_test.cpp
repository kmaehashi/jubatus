// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#include <vector>
#include <utility>
#include <map>
#include <string>

#include <pficommon/lang/cast.h>

#include "recommender_client.hpp"
#include "../fv_converter/datum.hpp"
#include "../fv_converter/converter_config.hpp"
#include "../recommender/recommender.hpp"
#include "../recommender/recommender_type.hpp"
#include "../classifier/classifier_test_util.hpp"
#include "../common/exception.hpp"

#include "test_util.hpp"

using std::string;
using std::map;
using std::vector;
using std::pair;
using std::make_pair;
using std::stringstream;

using jubatus::sfv_diff_t;
using jubatus::client::recommender;
using pfi::lang::lexical_cast;

static const char* NAME = "test";
static const int PORT = 65433;

namespace {

class recommender_test : public ::testing::Test {
 protected:
  pid_t child_;

  recommender_test() {
    child_ = fork_process("recommender",
                          PORT,
                          "./test_input/config.recommender.json");
  }

  virtual ~recommender_test() {
    kill_process(child_);
  }

  virtual void restart_process() {
    kill_process(this->child_);
    this->child_ = fork_process("recommender",
                                PORT,
                                "./test_input/config.recommender.json");
  }
};

std::string make_simple_config(const string& method) {
  pfi::text::json::json js(new pfi::text::json::json_object());
  js["method"] = pfi::text::json::json(
      new pfi::text::json::json_string(method));
  jubatus::fv_converter::converter_config config;
  jubatus::fv_converter::num_rule rule = { "*", "num" };
  config.num_rules.push_back(rule);
  std::stringstream conv;
  conv << config_to_string(config);
  pfi::text::json::json jsc;
  conv >> jsc;
  js["converter"] = jsc;

  pfi::text::json::json param(new pfi::text::json::json_object());
  // recommender's parameter
  if (method == "lsh") {
    param["bit_num"] = pfi::text::json::json(
        new pfi::text::json::json_integer(64));
  } else if (method == "minhash") {
    param["hash_num"] = pfi::text::json::json(
        new pfi::text::json::json_integer(64));
  }
  js["parameter"] = param;

  std::stringstream ret;
  ret << pfi::text::json::pretty(js);

  return ret.str();
}

TEST_F(recommender_test, get_status) {
  jubatus::client::recommender cli("localhost", PORT, 10);
  map<string, map<string, string> > status = cli.get_status(NAME);
  EXPECT_EQ(status.size(), 1u);
  for (map<string, map<string, string> >::const_iterator it = status.begin();
      it != status.end(); ++it) {
    EXPECT_GE(it->second.size(), 8u);
  }
}

TEST_F(recommender_test, small) {
  jubatus::client::recommender c("localhost", PORT, 10);

  jubatus::datum d;
  d.num_values.push_back(make_pair("f1", 1.0));
  c.update_row(NAME, "key", d);
  c.clear_row(NAME, "key");
  c.update_row(NAME, "key", d);

  jubatus::datum d2 = c.complete_row_from_datum(NAME, d);
  jubatus::datum d3 = c.complete_row_from_id(NAME, "key");
  //  cout << res.size() << endl;

  c.save(NAME, "name");
  c.load(NAME, "name");
}

sfv_diff_t make_vec(float v1, float v2, float v3) {
  sfv_diff_t v;
  v.push_back(make_pair("c1", v1));
  v.push_back(make_pair("c2", v2));
  v.push_back(make_pair("c3", v3));
  return v;
}

sfv_diff_t make_vec(const string& c1, const string& c2, const string& c3) {
  sfv_diff_t v;
  v.push_back(make_pair(c1, 1.0));
  v.push_back(make_pair(c2, 1.0));
  v.push_back(make_pair(c3, 1.0));
  return v;
}

template<typename T>
class recommender_random_test : public testing::Test {
};

TYPED_TEST_CASE_P(recommender_random_test);

TYPED_TEST_P(recommender_random_test, trivial) {
  TypeParam r;

  r.update_row("r1", make_vec("c1", "c2", "c3"));
  r.update_row("r2", make_vec("c4", "c5", "c6"));

  vector<pair<string, float> > ids;
  r.similar_row(make_vec("c1", "c2", "c3"), ids, 1);
  ASSERT_EQ(1u, ids.size());
  EXPECT_EQ("r1", ids[0].first);
}

TYPED_TEST_P(recommender_random_test, random) {
  TypeParam r;

  // Generate random data from two norma distributions, N1 and N2.
  vector<float> mu1;
  mu1.push_back(1.0);
  mu1.push_back(1.0);
  mu1.push_back(1.0);
  for (size_t i = 0; i < 100; ++i) {
    vector<double> v;
    make_random(mu1, 1.0, 3, v);
    string row_name = "r1_" + lexical_cast<string>(i);
    r.update_row(row_name, make_vec(v[0], v[1], v[2]));
  }

  vector<float> mu2;
  mu2.push_back(-1.0);
  mu2.push_back(-1.0);
  mu2.push_back(-1.0);
  for (size_t i = 0; i < 100; ++i) {
    vector<double> v;
    make_random(mu2, 1.0, 3, v);
    string row_name = "r2_" + lexical_cast<string>(i);
    r.update_row(row_name, make_vec(v[0], v[1], v[2]));
  }

  // Then, recommend to mean of N1
  vector<pair<string, float> > ids;
  r.similar_row(make_vec(1.0, 1.0, 1.0), ids, 10);
  ASSERT_EQ(10u, ids.size());
  size_t correct = 0;
  for (size_t i = 0; i < ids.size(); ++i) {
    if (ids[i].first[1] == '1') {
      ++correct;
    }
  }
  EXPECT_GT(correct, 5u);

  // save the recommender
  stringstream oss;
  r.save(oss);
  TypeParam r2;
  r2.load(oss);

  // Run the same test
  ids.clear();
  r2.similar_row(make_vec(1.0, 1.0, 1.0), ids, 10);
  ASSERT_EQ(10u, ids.size());
  correct = 0;
  for (size_t i = 0; i < ids.size(); ++i) {
    if (ids[i].first[1] == '1') {
      ++correct;
    }
  }
  EXPECT_GT(correct, 5u);
}

}  // namespace
