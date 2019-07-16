#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

#include "datetime.hpp"

class Log {
    public:
        static void Init() {
            std::string CurrDate = GetCurrentDate();
            outFile = std::make_unique<std::ofstream>(std::ofstream("Vision_Log_" + CurrDate + ".log", std::ofstream::out));
        }

        template <class ...Args>
        static void LogRuntimeError(Args... args) {
            std::lock_guard<std::mutex> lck (mtx);
            std::string _log_header = "RUNTIME ERROR";
            std::string _message = _log_header + ": ";
            for(auto& a: {args...}){
                _message += std::to_string(a);
            }
            _message += "\n";
        }

        template <class ...Args>
        static void LogException(Args... args) {
            std::lock_guard<std::mutex> lck (mtx);
            std::string _log_header = "EXCEPTION";
            std::string _message = _log_header + ": ";
            for(auto& a: {args...}){
                _message += std::to_string(a);
            }
            _message += "\n";
            
        }

        template <class ...Args>
        static void LogWarning(Args... args) {
            std::lock_guard<std::mutex> lck (mtx);
            std::string _log_header = "WARNING";
            std::string _message = _log_header + ": ";
            for(auto& a: {args...}){
                _message += std::to_string(a);
            }
            _message += "\n";
            
        }
        
        template <class ...Args>
        static void LogInfo(Args... args) {
            std::lock_guard<std::mutex> lck (mtx);
            std::string _log_header = "INFO";
            std::string _message = _log_header + ": ";
            for(auto& a: {args...}){
                _message += std::to_string(a) + " ";
            }
            _message += "\n";
        }

    public:
        static std::unique_ptr<std::ofstream> outFile;
        static std::mutex mtx;
};