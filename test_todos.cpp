#include <gtest/gtest.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// damit die Tests ausgeführt werden können muss die todoApplikation gestartet werden
// ansonsten funktionieren die Tests nicht
TEST(TodoApiTest, PostTodo) {
auto res = cpr::Post(
        cpr::Url{"http://localhost:18080/todos"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{R"({"task": "für Softwareentwicklung lernen"})"}
);

ASSERT_EQ(res.status_code, 201);
json body = json::parse(res.text);
ASSERT_TRUE(body.contains("id"));
}

TEST(TodoApiTest, GetTodos) {
auto res = cpr::Get(cpr::Url{"http://localhost:18080/todos"});

ASSERT_EQ(res.status_code, 200);
json todos = json::parse(res.text);
ASSERT_TRUE(todos.is_array());
}
