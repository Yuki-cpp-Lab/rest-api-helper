#pragma once

#include <functional>
#include <map>
#include <string>

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace yuki::web
{

enum class HTTPMethod
{
    HTTP_GET,
    HTTP_POST,
    HTTP_DELETE,
    HTTP_PUT,
};

std::string http_method_to_string(HTTPMethod method);

class RestAPI
{
  public:
    class Route
    {
      public:
        Route(const Route&) = delete;
        Route& operator=(const Route&) = delete;
        Route(Route&&) = delete;
        Route& operator=(Route&&) = delete;
        ~Route() = default;

        void add_endpoint(HTTPMethod method,
                          std::function<void(const httplib::Request&, httplib::Response&)> handler,
                          const std::string& description,
                          const std::map<std::string, std::string>& parameters_descriptions = {});

      private:
        Route(httplib::Server& server,
              nlohmann::json& endpoints_documentation,
              const std::string& route_path);

        httplib::Server& server_;
        nlohmann::json& endpoints_documentation_;
        std::string route_path_;

        friend class yuki::web::RestAPI;
    };

    RestAPI(httplib::Server& base_server, const std::string& base_api_route);
    RestAPI(const RestAPI&) = delete;
    RestAPI& operator=(const RestAPI&) = delete;
    RestAPI(RestAPI&&) = delete;
    RestAPI& operator=(RestAPI&&) = delete;
    ~RestAPI() = default;

    yuki::web::RestAPI::Route&
    add_route(const std::string& path,
              const std::string& description,
              const std::map<std::string, std::string>& path_parameters_descriptions = {});

    void add_docs_endpoint(const std::string& docs_path);

  private:
    httplib::Server& server_;
    std::string base_api_route_;
    nlohmann::json routes_documentation_;

    std::map<std::string, std::unique_ptr<yuki::web::RestAPI::Route>> routes_;
};

}  // namespace yuki::web
