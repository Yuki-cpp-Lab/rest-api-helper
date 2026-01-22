#include <gtest/gtest.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

#include "src/rest_api.hpp"

using namespace yuki::web;

class RestAPITest : public ::testing::Test {
protected:
    void SetUp() override {
        server_ = std::make_unique<httplib::Server>();
        api_ = std::make_unique<RestAPI>(*server_, "/api");
    }

    void TearDown() override {
        if (server_thread_.joinable()) {
            server_->stop();
            server_thread_.join();
        }
    }

    void StartServer() {
        // Find a free port or just use a fixed one.
        // Using 0 usually lets the OS pick one, but httplib::bind_to_port + listen_after_bind is needed.
        // For simplicity, let's try a fixed port and hope it's free, or retry.
        port_ = 12345;
        server_thread_ = std::thread([this]() {
            server_->listen("127.0.0.1", port_);
        });

        // Wait for server to start
        int retries = 0;
        while (!server_->is_running() && retries < 50) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            retries++;
        }
    }

    std::unique_ptr<httplib::Server> server_;
    std::unique_ptr<RestAPI> api_;
    std::thread server_thread_;
    int port_;
};

TEST_F(RestAPITest, TestAddRouteAndEndpoint) {
    auto& route = api_->add_route("test", "Test route");
    route.add_endpoint(HTTPMethod::HTTP_GET, [](const httplib::Request&, httplib::Response& res) {
        res.set_content("hello", "text/plain");
    }, "Get endpoint");

    StartServer();

    httplib::Client cli("127.0.0.1", port_);
    auto res = cli.Get("/api/test");

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->body, "hello");
}

TEST_F(RestAPITest, TestDocsGeneration) {
    auto& route = api_->add_route("test", "Test route");
    route.add_endpoint(HTTPMethod::HTTP_GET, [](const httplib::Request&, httplib::Response&) {}, "Get endpoint");

    api_->add_docs_endpoint("docs");

    StartServer();

    httplib::Client cli("127.0.0.1", port_);
    auto res = cli.Get("/api/docs");

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);

    auto json = nlohmann::json::parse(res->body);
    EXPECT_TRUE(json.contains("/api/test"));
    EXPECT_EQ(json["/api/test"]["description"], "Test route");
    EXPECT_TRUE(json["/api/test"].contains("GET"));
    EXPECT_EQ(json["/api/test"]["GET"]["description"], "Get endpoint");
}

TEST_F(RestAPITest, TestDuplicateRouteThrows) {
    api_->add_route("test", "Test route");
    EXPECT_THROW(api_->add_route("test", "Duplicate"), std::runtime_error);
}

TEST_F(RestAPITest, TestDuplicateEndpointThrows) {
    auto& route = api_->add_route("test", "Test route");
    route.add_endpoint(HTTPMethod::HTTP_GET, [](const httplib::Request&, httplib::Response&) {}, "Get endpoint");
    EXPECT_THROW(route.add_endpoint(HTTPMethod::HTTP_GET, [](const httplib::Request&, httplib::Response&) {}, "Duplicate"), std::runtime_error);
}

TEST_F(RestAPITest, TestPathParametersInDocs) {
    auto& route = api_->add_route("users/:id", "User route", {{"id", "User ID"}});
    route.add_endpoint(HTTPMethod::HTTP_GET, [](const httplib::Request&, httplib::Response&) {}, "Get user");

    api_->add_docs_endpoint("docs");
    StartServer();

    httplib::Client cli("127.0.0.1", port_);
    auto res = cli.Get("/api/docs");

    ASSERT_TRUE(res);
    auto json = nlohmann::json::parse(res->body);
    EXPECT_TRUE(json["/api/users/:id"]["path_parameters"].contains("id"));
    EXPECT_EQ(json["/api/users/:id"]["path_parameters"]["id"], "User ID");
}
