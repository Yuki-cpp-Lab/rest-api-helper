#include <httplib.h>

#include "src/rest_api.hpp"

int main()
{
    httplib::Server server;
    yuki::web::RestAPI api(server, "/api");

    api.add_docs_endpoint("docs");

    auto& r1 = api.add_route("/non_api/stuff", "Random thingy", { { "a", "desc_a" } });
    r1.add_endpoint(
        yuki::web::HTTPMethod::HTTP_GET,
        [](const httplib::Request&, httplib::Response&) {},
        "Description of the GET endpoint");
    r1.add_endpoint(yuki::web::HTTPMethod::HTTP_POST,
                    [](const httplib::Request&, httplib::Response&) {},
                    "Description of the POST endpoint",
                    { { "b", "desc_b" } });

    auto& r2 = api.add_route("within/the/api", "More random thingy");
    r2.add_endpoint(
        yuki::web::HTTPMethod::HTTP_DELETE,
        [](const httplib::Request&, httplib::Response&) {},
        "Description of the DELETE endpoint");
    r2.add_endpoint(yuki::web::HTTPMethod::HTTP_PUT,
                    [](const httplib::Request&, httplib::Response&) {},
                    "Description of the PUT endpoint",
                    { { "c", "desc_c" } });

    server.listen("0.0.0.0", 8080);

    return 0;
}
