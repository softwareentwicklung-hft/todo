#include "crow.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <mutex>

using json = nlohmann::json;

struct Todo {
    int id;
    std::string task;
    bool done;
};

std::vector<Todo> todos;
std::mutex todos_mutex;
int next_id = 1;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("static/index.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html");
        return res;
    });

    // Get all todos
    CROW_ROUTE(app, "/todos").methods("GET"_method)([]() {
        std::lock_guard<std::mutex> lock(todos_mutex);
        json j = json::array();
        for (const auto& todo : todos) {
            j.push_back({{"id", todo.id}, {"task", todo.task}, {"done", todo.done}});
        }
        return crow::response(j.dump());
    });

    // Add a todo
    CROW_ROUTE(app, "/todos").methods("POST"_method)([](const crow::request& req) {
        auto body = json::parse(req.body);
        if (!body.contains("task") || !body["task"].is_string()) {
            return crow::response(400, "Invalid JSON: missing 'task'");
        }
        std::lock_guard<std::mutex> lock(todos_mutex);
        Todo newTodo{next_id++, body["task"], false};
        todos.push_back(newTodo);
        return crow::response(201, json{{"id", newTodo.id}}.dump());
    });

    app.port(18080).multithreaded().run();
}
