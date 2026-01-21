#ifndef SRC_HELLO_HPP_
#define SRC_HELLO_HPP_

#include <string>

namespace rest_api_helper {

class Greeter {
public:
  Greeter(std::string name);
  std::string say_hello() const;

private:
  std::string name_;
};

} // namespace rest_api_helper

#endif // SRC_HELLO_HPP_
