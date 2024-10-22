#include "main.h"
#include "solution.h"
#include "unit_test.h"
#include "my_sol.h"
#include "local_battle.h"

// Constants for URLs
// Environment variables
char *char_TOKEN = std::getenv("token");
char *char_WHITE = std::getenv("white");
char *char_GAME_ID = std::getenv("game_id");
char *char_AI_NAME = std::getenv("ai_name");
char *char_LOCAL_BATTLE = std::getenv("local_battle");
char *char_IS_BATTLE_DEV = std::getenv("is_battle_dev");
char *char_IS_TEST= std::getenv("is_test");

std::string TOKEN(char_TOKEN ? char_TOKEN: "");
std::string WHITE(char_WHITE ? char_WHITE: "");
std::string GAME_ID(char_GAME_ID ? char_GAME_ID: "");
std::string AI_NAME(char_AI_NAME ? char_AI_NAME: "");
std::string LOCAL_BATTLE(char_LOCAL_BATTLE ? char_LOCAL_BATTLE: "");
std::string IS_TEST(char_IS_TEST ? char_IS_TEST: "");

const std::string QUERY_URL(char_IS_BATTLE_DEV ? "https://battle1024.ejoy.com/play/query": "http://plat1024-battle-service:4000/play/query");
const std::string MOVE_URL(char_IS_BATTLE_DEV ? "https://battle1024.ejoy.com/play/move": "http://plat1024-battle-service:4000/play/move");

// Function prototypes
json send_init_request();
json send_moves_request(const std::vector<std::vector<std::string>> &next_move);
json parse_json(const std::string body);
void start_battle();

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void log(std::string msg, std::string log_level) {
    json j = {{"msg", msg}};
    std::cout << "[" << log_level << "]" << "[" << "game_id: " << GAME_ID
        << ", white: " << WHITE << ", ai_name: " << AI_NAME << "] " << j.dump() << std::endl;
}



json send_request(std::string url, json request_data) {
    CURL *curl;
    CURLcode res;
    std::string read_buffer;
    char errbuf[CURL_ERROR_SIZE];

    curl = curl_easy_init();
    if (!curl) {
        log(string_format("curl_easy_init fail, url: %s",  url.c_str()));
        return NULL;
    }

    std::string json_data = request_data.dump();

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    errbuf[0] = 0;

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        log(string_format("request error, url: %s, error: %s", url.c_str(),  errbuf));
        curl_easy_cleanup(curl);
        return NULL;
    }

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if(response_code != 200) {
        log(string_format("http error, url: %s, http_response_code: %ld, body: %s", url.c_str(), response_code, read_buffer.c_str()));
        curl_easy_cleanup(curl);
        return NULL;
    }
    curl_easy_cleanup(curl);
    return parse_json(read_buffer);
}

json send_init_request() {
    json request_data = {
        {"token", TOKEN}
    };
    return send_request(QUERY_URL, request_data);
}

json send_moves_request(const std::vector<std::vector<std::string>> &next_move) {
    json request_data = {
        {"token", TOKEN},
        {"move", next_move}
    };
    return send_request(MOVE_URL, request_data);
}

json parse_json(const std::string init_response) {
    return json::parse(init_response);
}


void start_battle() {
    MySolution sol;

    if (IS_TEST == "1") {
        sol.set_data(10, 20, 2);
    } else {
        sol.set_data(57000, 20, 6);
    }
    while (true) {
        try {
            log("Sending Query request...");
            json init_response = send_init_request();
            if(init_response == NULL) {
                log("query fail, retry after 2 second");
                sleep(2);
                continue;
            }

            if(init_response["code"] == 10005) {
                log("room server is close");
                break;
            }

            if(init_response["code"] == 10001) {
                log((std::string)"Game is over, server will soon close, resp: " + init_response.dump());
                break;
            }

            if(init_response["code"] == 10004) {
                log("等待对方落子, 继续query轮询");
                continue;
            }

            if (init_response["code"] == 10003) {
                log("query success");
                std::vector<std::vector<std::string>> next_move = Solution::get_next_move(init_response["board"], WHITE == "true", sol);
                json move_response = send_moves_request(next_move);
                if (move_response["code"] == 20000) {
                    json move = {
                        next_move
                    };
                    log((std::string)"Invalid move, move: "+ move.dump() + ", resp: " + move_response.dump());
                    break;
                }
                if (move_response["code"] == 10001) {
                    log((std::string)"move, game is over, server will soon close, resp: " + move_response.dump());
                    break;
                }
                if (move_response["code"] == 10000) {
                    log("move success");
                } else {
                    log((std::string)"move Unexpected code received: " + move_response.dump());
                }
            } else {
                log((std::string)"query Unexpected code received, resp: " + init_response.dump());
            }
        } catch (const std::exception &e) {
            log((std::string)"An error occurred: " + e.what());
            break;
        }
    }
}

int main() {
    init_2();
    if (LOCAL_BATTLE == "true") {
        // run_test();
        local_battle();
        // start_battle();
    } else {
        start_battle();
    }

    return 0;
}
