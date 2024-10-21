#ifndef MAIN_H 
#define MAIN_H
#include <string>
#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <unistd.h>
#include <stdexcept>
#include <memory>

void log(std::string msg, std::string log_level = "info");

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf(new char[ size ]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

#endif