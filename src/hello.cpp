#include "src/hello.hpp"

#include <utility>

namespace yuki::web
{

std::string http_method_to_string(HTTPMethod method)
{
    switch (method)
    {
        case HTTPMethod::HTTP_GET:
            return "GET";
        case HTTPMethod::HTTP_POST:
            return "POST";
        case HTTPMethod::HTTP_DELETE:
            return "DELETE";
        case HTTPMethod::HTTP_PUT:
            return "PUT";
        default:
            return "UNKNOWN";
    }
}

RestAPI::Route::Route(httplib::Server& server,
                      nlohmann::json& endpoints_documentation,
                      const std::string& route_path) :
  server_(server), endpoints_documentation_(endpoints_documentation), route_path_(route_path)
{}

void RestAPI::Route::add_endpoint(
    HTTPMethod method,
    std::function<void(const httplib::Request&, httplib::Response&)> handler,
    const std::string& description,
    const std::map<std::string, std::string>& parameters_descriptions)
{
    if (endpoints_documentation_.contains(http_method_to_string(method)))
    {
        throw std::runtime_error("Endpoint for method " + http_method_to_string(method) +
                                 " already exists in route " + route_path_);
    }

    nlohmann::json endpoint;
    endpoint["description"] = description;
    endpoint["parameters"] = parameters_descriptions;
    endpoints_documentation_[http_method_to_string(method)] = endpoint;

    switch (method)
    {
        case HTTPMethod::HTTP_GET:
            server_.Get(route_path_.c_str(), handler);
            break;
        case HTTPMethod::HTTP_POST:
            server_.Post(route_path_.c_str(), handler);
            break;
        case HTTPMethod::HTTP_DELETE:
            server_.Delete(route_path_.c_str(), handler);
            break;
        case HTTPMethod::HTTP_PUT:
            server_.Put(route_path_.c_str(), handler);
            break;
        default:
            throw std::runtime_error("Unsupported HTTP method");
    }
}

RestAPI::RestAPI(httplib::Server& base_server, const std::string& base_api_route) :
  server_(base_server), base_api_route_(base_api_route)
{
    // Add a trailing slash if not present
    if (!base_api_route_.empty() && base_api_route_.back() != '/')
    {
        base_api_route_ += '/';
    }
}

yuki::web::RestAPI::Route&
RestAPI::add_route(const std::string& path,
                   const std::string& description,
                   const std::map<std::string, std::string>& path_parameters_descriptions)
{
    if (path.empty())
    {
        throw std::runtime_error("Path must not be empty");
    }

    auto full_path = path[0] == '/' ? path : base_api_route_ + path;

    if (routes_documentation_.contains(full_path))
    {
        throw std::runtime_error("Route already exists: " + full_path);
    }

    routes_documentation_[full_path]["description"] = description;
    routes_documentation_[full_path]["path_parameters"] = path_parameters_descriptions;

    routes_.emplace(
        full_path,
        std::unique_ptr<Route>(new Route(server_, routes_documentation_[full_path], full_path)));
    return *routes_.at(full_path);
}

void RestAPI::add_docs_endpoint(const std::string& docs_path)
{
    auto full_path = docs_path[0] == '/' ? docs_path : base_api_route_ + docs_path;

    server_.Get(full_path,
                [this]([[maybe_unused]] const httplib::Request& req, httplib::Response& res) {
                    res.set_content(routes_documentation_.dump(4), "application/json");
                });
}

}  // namespace yuki::web
