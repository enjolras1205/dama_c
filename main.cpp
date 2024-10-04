#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include "solution.h"


// Constants for URLs
const std::string QUERY_URL = "https://battle1024.ejoy.com/play/query";
const std::string MOVE_URL = "https://battle1024.ejoy.com/play/move";
// Environment variables
char *char_TOKEN = std::getenv("token");
char *char_WHITE = std::getenv("white");
char *char_GAME_ID = std::getenv("game_id");
char *char_AI_NAME = std::getenv("ai_name");

std::string TOKEN(char_TOKEN ? char_TOKEN: "");
std::string WHITE(char_WHITE ? char_WHITE: "");
std::string GAME_ID(char_GAME_ID ? char_GAME_ID: "");
std::string AI_NAME(char_AI_NAME ? char_AI_NAME: "");

// Function prototypes
json send_init_request();
json send_moves_request(const std::vector<std::vector<std::string>> &next_move);
json parse_response_init(const std::string &init_response);
json parse_response_move(const std::string &move_response);
void start_battle();

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void log(std::string msg, std::string log_level = "info") {
    json j = {{"msg", msg}};
    std::cout << "[" << log_level << "]" << "[" << "game_id: " << GAME_ID
        << ", white: " << WHITE << ", ai_name: " << AI_NAME << "] " << j.dump() << std::endl;
}

json send_init_request() {
    CURL *curl;
    CURLcode res;
    std::string read_buffer;

    curl = curl_easy_init();
    if (curl) {
        json request_data = {{"token", TOKEN}};
        std::string json_data = request_data.dump();
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, QUERY_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return parse_response_init(read_buffer);
}

json send_moves_request(const std::vector<std::vector<std::string>> &next_move) {
    CURL *curl;
    CURLcode res;
    std::string read_buffer;

    curl = curl_easy_init();
    if (curl) {
        json request_data = {
            {"token", TOKEN},
            {"move", next_move}
        };
        std::string json_data = request_data.dump();

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, MOVE_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return parse_response_move(read_buffer);
}

json parse_response_init(const std::string &init_response) {
    return json::parse(init_response);
}

json parse_response_move(const std::string &move_response) {
    return json::parse(move_response);
}

void start_battle() {
    while (true) {
        try {
            log("Sending Query request...");
            json init_response = send_init_request();
            log(init_response.dump());

            if(init_response["code"] == 10005) {
                log("room server is close");
                break;
            }

            if(init_response["code"] == 10001) {
                log("Game is over, server will soon close");
                break;
            }

            if(init_response["code"] == 10004) {
                log("等待对方落子, 继续query轮询");
                continue;
            }

            if (init_response["code"] == 10000 || init_response["code"] == 10002 || init_response["code"] == 10003) {
                std::vector<std::vector<std::string>> next_move = Solution::get_next_move(init_response, WHITE == "true");
                json move_response = send_moves_request(next_move);
                log((std::string)"Move response:" +  move_response.dump());

                while (move_response["code"] == 20000) {
                    log("Invalid move, please try again.");
                    std::vector<std::vector<std::string>> next_move = Solution::get_next_move(move_response, WHITE == "true");
                    move_response = send_moves_request(next_move);
                }
                if (move_response["code"] == 10001) {
                    log((std::string)"winner: " + move_response["winner"].dump());
                    return;
                }
            } else if (init_response["code"] == 30000) {
                log("落子超时，继续下子");
                std::vector<std::vector<std::string>> next_move = Solution::get_next_move(init_response, WHITE == "true");
                json move_response = send_moves_request(next_move);
                // Handle invalid moves
            } else if (init_response["code"] == 30001) {
                log("重复移动棋子超过3次，失败");
                return;
            } else if (init_response["code"] == 10001) {
                log((std::string)"winner: " + init_response["winner"].dump() + ", board: " +  init_response["board"].dump());
                return;
            } else {
                log((std::string)"Unexpected code received: " + init_response["code"].dump());
                return;
            }
        } catch (const std::exception &e) {
            log((std::string)"An error occurred: " + e.what());
            break;
        }
    }
}

int main() {
    start_battle();
    return 0;
}
