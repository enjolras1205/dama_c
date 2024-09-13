#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "board_handler.h"


#define QUERY_URL "https://battle1024.ejoy.com/play/query"
#define MOVE_URL "https://battle1024.ejoy.com/play/move"
#define TOKEN getenv("token") // 获取环境变量 TOKEN
#define WHITE getenv("white") // 获取环境变量 WHITE

char* send_init_request();
char* parse_response_init(const char* init_response);
char* send_moves_request(const char* next_move);
char* parse_response_move(const char* move_response);
void start_battle();
void free_memory(char* str);

void free_memory(char* str) {
    if (str) free(str);
}

void start_battle() {
    while (true) {
        printf("Sending Query request...\n");
            char* init_response = send_init_request();
            printf("%s\n", init_response);

            cJSON *jobj = cJSON_Parse(init_response);
            if (jobj == NULL) {
                fprintf(stderr, "Error parsing JSON\n");
                free_memory(init_response);
                break; // 错误处理
            }
            cJSON *code_obj = cJSON_GetObjectItem(jobj, "code");
            int code = code_obj ? code_obj->valueint : -1;

            if (strcmp(init_response, "game is over, do not query again") == 0) {
                printf("Game is over.\n");
                free_memory(init_response);
                cJSON_Delete(jobj);
                break;
            }

            cJSON *board_obj;
            if (code == 10000 || code == 10002 || code == 10003) {
                board_obj = cJSON_GetObjectItem(jobj, "board");
                int board[8][8] = {0}; 

                 // 将 JSON 棋盘数据解析到整形数组
                for (int i = 0; i < 8; i++) {
                    cJSON *row_obj = cJSON_GetArrayItem(board_obj, i);
                    for (int j = 0; j < 8; j++) {
                        cJSON *cell_obj = cJSON_GetArrayItem(row_obj, j);
                        board[i][j] = cell_obj ? cell_obj->valueint : 0;  // 获取每个单元的值
                    }
                }
                char* next_move = get_next_move(board, strcmp(WHITE, "true") == 0);
                char* move_response = send_moves_request(next_move);
                char* detail = parse_response_move(move_response);

                printf("Move response is:\n");
                printf("%s\n", detail);
                printf("=============\n");

                cJSON *detail_obj = cJSON_GetObjectItem(jobj, "code");
                int detail_code = detail_obj ? detail_obj->valueint : -1;

                while (detail_code == 20000) {
                    printf("Invalid move, please try again.\n");

                    next_move = get_next_move(board, strcmp(WHITE, "true") == 0);
                    move_response = send_moves_request(next_move);

                    detail = parse_response_move(move_response);
                    cJSON *code_obj = cJSON_GetObjectItem(detail_obj, "code");
                    detail_code = code_obj ? code_obj->valueint : -1;
                }

                if (detail_code == 10001) {
                    cJSON *winner_obj = cJSON_GetObjectItem(jobj, "winner");
                    printf("%s\n", cJSON_GetStringValue(winner_obj));
                    free_memory(init_response); // 确保释放之前的内存
                    cJSON_Delete(jobj); // 释放 JSON 对象内存
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
    }
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

int main() {
    start_battle();
    return 0;
}