#include <iostream>
#include <curl>

using namespace std;

int main() {
    CURL* curl;
    CURLcode res;
    const char* token = std::getenv("token")
    while(1) {
        cout << "the token is: " << token << endl; 
    }
    return 0;
}