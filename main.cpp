#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include "solution.h"


// Constants for URLs
const std::string QUERY_URL = "https://battle1024.ejoy.com/play/query";
const std::string MOVE_URL = "https://battle1024.ejoy.com/play/move";
// Environment variables
std::string TOKEN = std::getenv("token");
std::string WHITE = std::getenv("white");

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
            std::cout << "Sending Query request...\n";
            json init_response = send_init_request();
            std::cout << init_response.dump(4) << std::endl;

            if(init_response["code"] == 10005) {
                std::cout << "room server is close\n";
                break;
            }

            if(init_response["code"] == 10001) {
                std::cout << "Game is over, server will soon close\n";
                break;
            }

            if(init_response["code"] == 10004) {
                std::cout << "等待对方落子, 继续query轮询\n";
                continue;
            }

            if (init_response["code"] == 10000 || init_response["code"] == 10002 || init_response["code"] == 10003) {
                std::vector<std::vector<std::string>> next_move = Solution::get_next_move(init_response, WHITE == "true");
                json move_response = send_moves_request(next_move);
                std::cout << "Move response:\n" << move_response.dump(4) << std::endl;

                while (move_response["code"] == 20000) {
                    std::cout << "Invalid move, please try again.\n";
                    std::vector<std::vector<std::string>> next_move = Solution::get_next_move(move_response, WHITE == "true");
                    move_response = send_moves_request(next_move);
                }
                if (move_response["code"] == 10001) {
                    std::cout << move_response["winner"] << std::endl;
                    return;
                }
            } else if (init_response["code"] == 30000) {
                std::cout << "落子超时，继续下子\n";
                std::vector<std::vector<std::string>> next_move = Solution::get_next_move(init_response, WHITE == "true");
                json move_response = send_moves_request(next_move);
                // Handle invalid moves
            } else if (init_response["code"] == 30001) {
                std::cout << "重复移动棋子超过3次，失败\n";
                return;
            } else if (init_response["code"] == 10001) {
                std::cout << init_response["board"] << std::endl;
                std::cout << init_response.value("winner", "") << std::endl;
                return;
            } else {
                std::cout << "Unexpected code received: " << init_response["code"] << std::endl;
                return;
            }
        } catch (const std::exception &e) {
            std::cout << "An error occurred: " << e.what() << std::endl;
            break;
        }
    }
}

int main() {
    start_battle();
    return 0;
}
