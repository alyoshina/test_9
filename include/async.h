#pragma once

#include "parser_tread.h"

namespace {

class MainTreads {
public:
    MainTreads(std::size_t bulk);
    ~MainTreads();
    void addData(const char *data, std::size_t size) {
        parserTread->addData(data, size);
    }
private:
    std::shared_ptr<ParserTread> parserTread;
    std::list<std::shared_ptr<IOutput>> list; /**< command output methods */
};

} //namespace

namespace async {

using handle_t = void *;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

} //async
