// this program is automatically generated by jenerator. do not edit.
#include "../framework.hpp"
#include "stat_server.hpp"
#include "stat_serv.hpp"
using namespace jubatus;
using namespace jubatus::framework;
namespace jubatus {
namespace server {
class stat_impl_ : public stat<stat_impl_> {
 public:
  stat_impl_(const server_argv& a)
      : stat<stat_impl_>(a.timeout),
        p_(new server_helper<stat_serv>(a, true)) {
  }

  std::string get_config(std::string name)  //analysis random
      {
    JRLOCK__(p_);
    return get_p()->get_config();
  }

  bool push(std::string name, std::string key, double val)  //update cht(1)
            {
    JWLOCK__(p_);
    return get_p()->push(key, val);
  }

  double sum(std::string name, std::string key)  //analysis cht(1)
             {
    JRLOCK__(p_);
    return get_p()->sum(key);
  }

  double stddev(std::string name, std::string key)  //analysis cht(1)
                {
    JRLOCK__(p_);
    return get_p()->stddev(key);
  }

  double max(std::string name, std::string key)  //analysis cht(1)
             {
    JRLOCK__(p_);
    return get_p()->max(key);
  }

  double min(std::string name, std::string key)  //analysis cht(1)
             {
    JRLOCK__(p_);
    return get_p()->min(key);
  }

  double entropy(std::string name, std::string key)  //analysis cht(1)
                 {
    JRLOCK__(p_);
    return get_p()->entropy(key);
  }

  double moment(std::string name, std::string key, int n, double c)  //analysis cht(1)
                {
    JRLOCK__(p_);
    return get_p()->moment(key, n, c);
  }

  bool save(std::string name, std::string id)  //update broadcast
            {
    JWLOCK__(p_);
    return get_p()->save(id);
  }

  bool load(std::string name, std::string id)  //update broadcast
            {
    JWLOCK__(p_);
    return get_p()->load(id);
  }

  std::map<std::string, std::map<std::string, std::string> > get_status(
      std::string name)  //analysis broadcast
      {
    JRLOCK__(p_);
    return p_->get_status();
  }
  int run() {
    return p_->start(*this);
  }
  ;
  common::cshared_ptr<stat_serv> get_p() {
    return p_->server();
  }
  ;
 private:
  common::cshared_ptr<server_helper<stat_serv> > p_;
};
}
}  // namespace jubatus::server
int main(int args, char** argv) {
  return jubatus::framework::run_server<jubatus::server::stat_impl_>(args, argv,
                                                                     "stat");
}
