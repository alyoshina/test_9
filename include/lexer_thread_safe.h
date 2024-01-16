#pragma once

#include "lexer.h"

#include <thread>
#include <mutex>
#include <condition_variable>

class LexerThreadSafe : public Lexer {
public:
    LexerThreadSafe(std::istream &in, std::mutex &in_m, std::condition_variable &in_cv)
        : Lexer(in)
        , m(in_m)
        , cv(in_cv) {}
    ~LexerThreadSafe() = default;

    void setStoped(bool v) override {
        std::unique_lock lk(m);
        stopped = v;
    }
    bool ready {false};

protected:
    bool readLine(std::string &str) override {
        std::unique_lock lk(m);
        cv.wait(lk, [this]{ return ready || stopped; });
        ready = false;
        return bool(std::getline(c_in, str));
    }
private:
    std::mutex &m;
    std::condition_variable &cv;
};