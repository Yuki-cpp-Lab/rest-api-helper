#pragma once

#include <string>

namespace rest_api_helper
{

class Greeter
{
  public:
    Greeter(std::string name);
    std::string say_hello() const;

  private:
    std::string name_;
};

}  // namespace rest_api_helper
