#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "solution.h"


#define QUERY_URL "https://battle1024.ejoy.com/play/query"
#define MOVE_URL "https://battle1024.ejoy.com/play/move"
#define TOKEN getenv("token") // 获取环境变量 TOKEN
#define WHITE getenv("white") // 获取环境变量 WHITE

typedef struct {
    int x, y;
} Move;

typedef struct {
    Move* moves;
    size_t size;
} MoveList;


char* send_init_request();
char* parse_response_init(const char* init_response);
char* get_next_move(int board[8][8], bool white);
char* send_moves_request(const char* next_move);
char* parse_response_move(const char* move_response);
void start_battle();
void free_memory(char* str);

void free_memory(char* str) {
    if (str) free(str);
}

void start_battle() {
    while (true) {
        try {
            printf("Sending Query request...\n");
            char* init_response = send_init_request();
            printf("%s\n", init_response);

            json_object *jobj = json_tokener_parse(init_response);
            json_object *code_obj;
            json_object_object_get_ex(jobj, "code", &code_obj);
            int code = json_object_get_int(code_obj);

            if (strcmp(init_response, "game is over, do not query again") == 0) {
                printf("Game is over.\n");
                free_memory(init_response);
                break;
            }

            json_object *board_obj;
            if (code == 10000 || code == 10002 || code == 10003) {
                json_object_object_get_ex(jobj, "board", &board_obj);
                int board[8][8] = {0}; 

                char* next_move = get_next_move(board, strcmp(WHITE, "true") == 0);
                char* move_response = send_moves_request(next_move);
                char* detail = parse_response_move(move_response);

                printf("Move response is:\n");
                printf("%s\n", detail);
                printf("=============\n");

                json_object *detail_obj;
                json_object_object_get_ex(jobj, "code", &detail_obj);
                int detail_code = json_object_get_int(detail_obj);
                while (detail_code == 20000) {
                    printf("Invalid move, please try again.\n");
                    next_move = get_next_move(board);
                    move_response = send_moves_request(next_move);
                    detail = parse_response_move(move_response);
                    json_object_object_get_ex(jobj, "code", &detail_obj);
                    detail_code = json_object_get_int(detail_obj);
                }

                if (detail_code == 10001) {
                    json_object *winner_obj;
                    json_object_object_get_ex(jobj, "winner", &winner_obj);
                    printf("%s\n", json_object_get_string(winner_obj));
                    return;
                }
            } else if (code == 30000) {
                printf("Move timed out, continue moving.\n");
            } else if (code == 30001) {
                printf("Repeated moves exceeded 3 times, failed.\n");
                return;
            } else if (code == 10001) {
                printf("Final Board:\n%s\n", init_response);
                return;
            } else {
                printf("Unexpected code received: %d\n", code);
                return;
            }
        } catch (const char* e) {
            printf("An error occurred: %s\n", e);
            break;
        }
    }
}

MoveList* get_next_move(int board[8][8], bool white) {
    return 
}

char* send_init_request() {
    CURL* curl;
    CURLcode res;
    char* response_data = malloc(4096);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Prepare JSON data
        char json_data[100];
        snprintf(json_data, sizeof(json_data), "{\"token\": \"%s\"}", TOKEN);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, QUERY_URL);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    return response_data;
}

char* parse_response_move(const char* move_response) {
    return strdup(move_response);
}

char* send_moves_request(const char* next_move) {
    CURL* curl;
    CURLcode res;
    char* response_data = malloc(4096);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Prepare JSON data
        char json_data[100];
        snprintf(json_data, sizeof(json_data), "{\"token\": \"%s\", \"move\": \"%s\"}", TOKEN, next_move);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, MOVE_URL);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    return response_data;
}
