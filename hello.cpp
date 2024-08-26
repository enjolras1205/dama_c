#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    const char* token = std::getenv("token");
    while(1) {
        cout << "the token is: " << token << endl; 
    }
    return 0;
};
