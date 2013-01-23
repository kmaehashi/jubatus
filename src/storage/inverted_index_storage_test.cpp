#include <gtest/gtest.h>
#include <cmath>
#include "inverted_index_storage.hpp"

namespace jubatus {
namespace storage {

using namespace std;

TEST(inverted_index_storage, trivial) {
  inverted_index_storage s;
  // r1: (1, 1, 1, 0, 0)
  s.set("c1", "r1", 1);
  s.set("c2", "r1", 1);
  s.set("c3", "r1", 1);
  // r2: (1, 0, 1, 1, 0)
  s.set("c1", "r2", 1);
  s.set("c3", "r2", 1);
  s.set("c4", "r2", 1);
  // r3: (0, 1, 0, 0, 1)
  s.set("c2", "r3", 1);
  s.set("c5", "r3", 1);

  // v:  (1, 1, 0, 0, 0)
  sfv_t v;
  v.push_back(make_pair("c1", 1.0));
  v.push_back(make_pair("c2", 1.0));

  vector<pair<string, float> > scores;
  s.calc_scores(v, scores, 100);

  ASSERT_EQ(3u, scores.size());
  EXPECT_FLOAT_EQ(2.0 / sqrt(3) / sqrt(2), scores[0].second);
  EXPECT_EQ("r1", scores[0].first);
  EXPECT_FLOAT_EQ(1.0 / sqrt(2) / sqrt(2), scores[1].second);
  EXPECT_EQ("r3", scores[1].first);
  EXPECT_FLOAT_EQ(1.0 / sqrt(2) / sqrt(3), scores[2].second);
  EXPECT_EQ("r2", scores[2].first);

  stringstream ss;
  s.save(ss);
  inverted_index_storage s2;
  s2.load(ss);
  vector<pair<string, float> > scores2;
  s.calc_scores(v, scores2, 100);
  // expect to get same result
  ASSERT_EQ(3u, scores2.size());
  EXPECT_FLOAT_EQ(2.0 / sqrt(3) / sqrt(2), scores2[0].second);
  EXPECT_EQ("r1", scores2[0].first);
  EXPECT_FLOAT_EQ(1.0 / sqrt(2) / sqrt(2), scores2[1].second);
  EXPECT_EQ("r3", scores2[1].first);
  EXPECT_FLOAT_EQ(1.0 / sqrt(2) / sqrt(3), scores2[2].second);
  EXPECT_EQ("r2", scores2[2].first);
}

TEST(inverted_index_storage, diff) {
  inverted_index_storage s;
  // r1: (1, 1, 0, 0, 0)
  s.set("c1", "r1", 1);
  s.set("c2", "r1", 1);

  string diff;
  s.get_diff(diff);

  inverted_index_storage t;
  t.set_mixed_and_clear_diff(diff);
  EXPECT_EQ(1.0, t.get("c1", "r1"));
  EXPECT_EQ(1.0, t.get("c2", "r1"));
  EXPECT_EQ(0.0, t.get("c3", "r1"));
  EXPECT_EQ(0.0, t.get("c1", "r2"));
}

TEST(inverted_index_storage, mix) {
  inverted_index_storage s1;
  // c1: (1, 1, 0)
  s1.set("c1", "r1", 1);
  s1.set("c1", "r2", 1);
  // c2: (0, 0, 1)
  s1.set("c2", "r3", 1);

  inverted_index_storage s2;
  // c1: (0, 2, 0)
  s2.set("c1", "r2", 2);

  string d1, d2;
  s1.get_diff(d1);
  s2.get_diff(d2);

  // d2 --> d1
  s2.mix(d2, d1);

  s2.set_mixed_and_clear_diff(d1);

  // expected:
  //  c1: (1, 2, 0)   = (1,1,0) << (0,2,0)
  //  c2: (0, 0, 1)   = () << (0,0,1)
  EXPECT_EQ(1.0, s2.get("c1", "r1"));
  EXPECT_EQ(2.0, s2.get("c1", "r2"));
  EXPECT_EQ(0.0, s2.get("c1", "r3"));

  EXPECT_EQ(0.0, s2.get("c2", "r1"));
  EXPECT_EQ(0.0, s2.get("c2", "r2"));
  EXPECT_EQ(1.0, s2.get("c2", "r3"));
}

}
}
