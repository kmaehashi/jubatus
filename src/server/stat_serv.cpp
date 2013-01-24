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

#include "stat_serv.hpp"

#include <algorithm>
#include <string>

#include <pficommon/text/json.h>

#include "../common/shared_ptr.hpp"
#include "../common/jsonconfig.hpp"
#include "../framework/mixer/mixer_factory.hpp"

using std::string;
using std::make_pair;
using pfi::text::json::json;
using pfi::text::json::json_cast;
using pfi::lang::lexical_cast;

using jubatus::common::cshared_ptr;
using jubatus::common::lock_service;
using jubatus::framework::server_argv;
using jubatus::framework::mixer::create_mixer;
using jubatus::framework::mixable_holder;

namespace jubatus {
namespace server {

struct stat_serv_config {
  int32_t window_size;

  template<typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(window_size);
  }
};

stat_serv::stat_serv(const server_argv& a, const cshared_ptr<lock_service>& zk)
    : server_base(a) {
  mixer_.reset(create_mixer(a, zk));
  mixable_holder_.reset(new mixable_holder());

  mixer_->set_mixable_holder(mixable_holder_);
  mixable_holder_->register_mixable(&stat_);
}

stat_serv::~stat_serv() {
}

framework::mixer::mixer* stat_serv::get_mixer() const {
  return mixer_.get();
}

pfi::lang::shared_ptr<mixable_holder> stat_serv::get_mixable_holder() const {
  return mixable_holder_;
}

void stat_serv::get_status(status_t& status) const {
  status.insert(make_pair("storage", stat_.get_model()->type()));
}

bool stat_serv::set_config(const string& config) {
  jsonconfig::config conf_root(lexical_cast<json>(config));
  stat_serv_config conf = jsonconfig::config_cast_check<stat_serv_config>(
      conf_root);

  common::cshared_ptr<stat::mixable_stat> model(
      new stat::mixable_stat(conf.window_size));
  config_ = config;
  stat_.set_model(model);

  LOG(INFO) << "config loaded: " << config;
  return true;
}

string stat_serv::get_config() const {
  return config_;
}

bool stat_serv::push(const std::string& key, double value) {
  stat_.get_model()->push(key, value);
  DLOG(INFO) << "pushed: " << key;
  return true;
}

double stat_serv::sum(const std::string& key) const {
  return stat_.get_model()->sum(key);
}

double stat_serv::stddev(const std::string& key) const {
  return stat_.get_model()->stddev(key);
}

double stat_serv::max(const std::string& key) const {
  return stat_.get_model()->max(key);
}

double stat_serv::min(const std::string& key) const {
  return stat_.get_model()->min(key);
}

double stat_serv::entropy(const std::string& key) const {
#ifdef HAVE_ZOOKEEPER_H
  // TODO(kuenishi): currently gets old value of entropy when mix completed
  return stat_.get_model()->mixed_entropy();
#else
  return stat_.get_model()->entropy();
#endif
}

double stat_serv::moment(const std::string& key, int n, double c) const {
  return stat_.get_model()->moment(key, n, c);
}

}  // namespace server
}  // namespace jubatus
