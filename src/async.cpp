#include "async.h"

namespace {

MainTreads::MainTreads(std::size_t bulk) {
    //log
    list.push_back(std::make_shared<ConsoleOutputThread>(std::cout));
    //file1, file2
    list.push_back(std::make_shared<FileOutputThreads>());
    parserTread = std::make_shared<ParserTread>(list, bulk);
}
MainTreads::~MainTreads() {
    parserTread->dataEnd();
    parserTread->join();
    for (auto io: list) {
        io->stop();
    }        
}

} //namespace

namespace async {

handle_t connect(std::size_t bulk) {
    MainTreads *m = new MainTreads(bulk);
    return static_cast<handle_t>(m);
}

void receive(handle_t handle, const char *data, std::size_t size) {
    MainTreads *m = static_cast<MainTreads *>(handle);
    m->addData(data, size);
}

void disconnect(handle_t handle) {
    MainTreads *m = static_cast<MainTreads *>(handle);
    delete m;
}

} //async
