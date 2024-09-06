#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

// Environment Variables
const char *TOKEN = "your_token_here"; // 要替换成实际的 Token
const char *WHITE = "true"; // 这里可以根据需要修改
const char *QUERY_URL = "https://battle1024.ejoy.com/play/query";
const char *MOVE_URL = "https://battle1024.ejoy.com/play/move";

// HTTP请求接收结构
struct response {
    char *memory;
    size_t size;
};

// 回调函数用于接收 HTTP 响应
size_t write_callback(void *ptr, size_t size, size_t nmemb, struct response *resp) {
    size_t realsize = size * nmemb;
    resp->memory = realloc(resp->memory, resp->size + realsize + 1);
    if (resp->memory == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(resp->memory[resp->size]), ptr, realsize);
    resp->size += realsize;
    resp->memory[resp->size] = 0; // Null-terminate the string
    return realsize;
}

char *send_request(const char *url, const char *json_data) {
    CURL *curl;
    CURLcode res;
    struct response resp;
    resp.memory = malloc(1);
    resp.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, NULL);
        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return resp.memory;
}

char *send_init_request() {
    cJSON *data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "token", TOKEN);
    char *json_data = cJSON_Print(data);
    char *response = send_request(QUERY_URL, json_data);
    cJSON_Delete(data);
    free(json_data);
    return response;
}

char *send_moves_request(char **moves, int count) {
    cJSON *data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "token", TOKEN);
    cJSON *moves_array = cJSON_CreateArray();

    for (int i = 0; i < count; i++) {
        cJSON_AddItemToArray(moves_array, cJSON_CreateString(moves[i]));
    }
    cJSON_AddItemToObject(data, "move", moves_array);
    
    char *json_data = cJSON_Print(data);
    char *response = send_request(MOVE_URL, json_data);
    cJSON_Delete(data);
    free(json_data);
    return response;
}

void start_battle() {
    while (1) {
        printf("Battle starting...\n");
        char *init_response = send_init_request();
        printf("Init response: %s\n", init_response);
        
        // 解析 JSON 响应
        cJSON *response = cJSON_Parse(init_response);
        int code = cJSON_GetObjectItem(response, "code")->valueint;
        
        // 处理不同的状态码
        if (code == 10000 || code == 10002 || code == 10003) {
            printf("Code: %d\n", code);
            // 替换为处理获取下一步的棋盘逻辑
        } else {
            printf("Unexpected code received: %d\n", code);
            break;
        }

        free(init_response);
        cJSON_Delete(response);
    }
}

int main() {
    start_battle();
    return 0;
}
