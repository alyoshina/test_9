#pragma once

#include "lexer_thread_safe.h"
#include "parser.h"

class ParserTread {
public:
    ParserTread(std::list<std::shared_ptr<IOutput>>& list, const std::size_t bulk) {
        //converting string received from console into a command type
        auto lexer = std::make_shared<LexerThreadSafe>(input, m, cv);
        //converting received command type into a command
        parser = std::make_shared<Parser>(lexer, list, bulk);
        m_thread = std::thread(&ParserTread::run, this);
    }
    ~ParserTread() = default;
    void join() { m_thread.join(); }
    void addData(const char *data, std::size_t size) {
        {
            std::lock_guard lock(m);
            input << std::string(data, size);
        }
        cv.notify_all();
    }
    void dataEnd() {
        if (parser) parser->setStoped(true);
        cv.notify_all();
    }

private:
    std::shared_ptr<Parser> parser;
    std::thread m_thread;
    std::stringstream input;
    std::condition_variable cv;
    std::mutex m;

    void run() {
        while (parser->parse()) {
            ;
        }
    }
};