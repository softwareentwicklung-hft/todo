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

    // hier wird die index.html Datei aufgerufen, damit das UI angezeigt werden kann
    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("static/index.html");
        // Datei kann nicht gefunden werden
        if (!file.is_open()) {
            return crow::response(404, "File not found");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html");
        return res;
    });

    // Zeige alle Aufgaben an
    CROW_ROUTE(app, "/todos").methods("GET"_method)([]() {
        std::lock_guard<std::mutex> lock(todos_mutex);
        json j = json::array();
        // Schleife, um durch alle Aufgaben durchzugehen und diese im JSON zurückzugeben
        for (int i = 0; i < todos.size(); ++i) {
            j.push_back({{"id", todos.at(i).id}, {"task", todos.at(i).task}, {"done", todos.at(i).done}});
        }
        // Zurückgeben von allen Aufgaben
        return crow::response(j.dump());
    });

    // Füge eine Aufgabe hinzu
    CROW_ROUTE(app, "/todos").methods("POST"_method)([](const crow::request& req) {
        // Request wird in ein JSON umgewandelt
        nlohmann::basic_json<> body = json::parse(req.body);
        // Damit eine neue Aufgabe hinzugefügt werden kann, muss diese im Body sein.
        // Wenn dieser leer ist, wird eine Fehlermeldung ausgegeben
        if (!body.contains("task") || !body["task"].is_string()) {
            return crow::response(400, "Invalid JSON: missing 'task'");
        }
        std::lock_guard<std::mutex> lock(todos_mutex);
        // ID wird automatisch erstellt, indem wir die Variable next_id für jede ID um eins erhöhen
        Todo newTodo{next_id++, body["task"], false};
        // Mit push_back wird die Aufgabe der "Vektor"-Liste hinzugefügt
        todos.push_back(newTodo);
        // Wir geben zurück, dass die Aufgabe erfolgreich hinzugefügt wurde
        return crow::response(201, json{{"id", newTodo.id}}.dump());
    });

    // Lösche alle Aufgaben
    CROW_ROUTE(app, "/todos").methods("DELETE"_method)([]() {
        std::lock_guard<std::mutex> lock(todos_mutex);
        todos.clear(); // Alle Elemente löschen
        next_id = 1;   // ID-Zähler zurücksetzen
        // 204 No Content ist die Standardantwort für erfolgreiche Operation ohne Rückgabe
        return crow::response(204);
    });


    // Lösche eine spezifische Aufgabe
    CROW_ROUTE(app, "/todos/<int>").methods("DELETE"_method)([](int id) {
        std::lock_guard<std::mutex> lock(todos_mutex);

        bool foundAndRemoved = false;

        // Manuelles Suchen und Löschen
        for (auto it = todos.begin(); it != todos.end(); ++it) {
            if (it->id == id) {
                todos.erase(it);
                foundAndRemoved = true;
                break; // Da IDs eindeutig sind, können wir aufhören
            }
        }

        if (foundAndRemoved) {
            // Erfolgreich gelöscht
            return crow::response(204);
        }

        // Aufgabe nicht gefunden
        return crow::response(404, "Todo not found");
    });




    app.port(18080).multithreaded().run();
}
