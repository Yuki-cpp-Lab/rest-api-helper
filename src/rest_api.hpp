#pragma once

#include <functional>
#include <map>
#include <string>

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace yuki::web
{

/**
 * @brief Enum representing supported HTTP methods.
 */
enum class HTTPMethod
{
    HTTP_GET,
    HTTP_POST,
    HTTP_DELETE,
    HTTP_PUT,
};

/**
 * @brief Converts HTTPMethod enum to its string representation.
 *
 * @param method The HTTPMethod to convert.
 * @return std::string The string representation of the method (e.g., "GET").
 */
std::string http_method_to_string(HTTPMethod method);

/**
 * @brief A helper class to manage REST API routes and documentation.
 *
 * This class wraps an httplib::Server and provides a structured way to add routes
 * and endpoints, automatically generating documentation for them.
 */
class RestAPI
{
  public:
    /**
     * @brief Represents a single route in the API (e.g., "/users").
     *
     * A Route can have multiple endpoints (e.g., GET /users, POST /users).
     */
    class Route
    {
      public:
        Route(const Route&) = delete;
        Route& operator=(const Route&) = delete;
        Route(Route&&) = delete;
        Route& operator=(Route&&) = delete;
        ~Route() = default;

        /**
         * @brief Adds an endpoint to the route.
         *
         * @param method The HTTP method for the endpoint.
         * @param handler The function to handle requests.
         * @param description A description of what the endpoint does.
         * @param parameters_descriptions A map of parameter names to their descriptions (optional).
         */
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

    /**
     * @brief Construct a new RestAPI object.
     *
     * @param base_server The httplib::Server instance to attach routes to.
     * @param base_api_route The base path for all routes (e.g., "/api/v1").
     */
    RestAPI(httplib::Server& base_server, const std::string& base_api_route);
    RestAPI(const RestAPI&) = delete;
    RestAPI& operator=(const RestAPI&) = delete;
    RestAPI(RestAPI&&) = delete;
    RestAPI& operator=(RestAPI&&) = delete;
    ~RestAPI() = default;

    /**
     * @brief Adds a new route to the API.
     *
     * @param path The path for the route, relative to the base API route.
     * @param description A description of the route resource.
     * @param path_parameters_descriptions A map of path parameter names to their descriptions (optional).
     * @return yuki::web::RestAPI::Route& A reference to the created Route object.
     */
    yuki::web::RestAPI::Route&
    add_route(const std::string& path,
              const std::string& description,
              const std::map<std::string, std::string>& path_parameters_descriptions = {});

    /**
     * @brief Adds a documentation endpoint that serves the generated API docs in JSON format.
     *
     * @param docs_path The path for the documentation endpoint, relative to the base API route.
     */
    void add_docs_endpoint(const std::string& docs_path);

  private:
    httplib::Server& server_;
    std::string base_api_route_;
    nlohmann::json routes_documentation_;

    std::map<std::string, std::unique_ptr<yuki::web::RestAPI::Route>> routes_;
};

}  // namespace yuki::web
