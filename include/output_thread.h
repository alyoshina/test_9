#pragma once

#include "output.h"

#include <filesystem>
#include <format>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

class IOutputThread : public IOutput {
public:
    IOutputThread(uint threadsNumber = 1) {
        for (uint i = 0; i < threadsNumber; ++i) {
            std::thread t = std::thread(&IOutputThread::run, this);
            threads.emplace_back(std::move(t));
        }
    }
    ~IOutputThread() = default;
    void stop() override {
        stopped = true;
        cv.notify_all();
        for (auto& t: threads) {
            t.join();
        }
    }
    void addData(std::shared_ptr<Data> s) override {
        {
            std::unique_lock lk(m);
            dataList.push_back(s);
            ready = true;
        }
        cv.notify_one();
    }

protected:
    virtual void worker(std::shared_ptr<Data> s) = 0;
    void run() {
        while (!stopped || !dataEmpty()) {
            std::shared_ptr<Data> s_ptr;
            {
                std::unique_lock lk(m); //std::scoped_lock lock
                cv.wait(lk, [this]{ return ready || stopped; });
                ready = false;
            }

            while ((s_ptr = getData())) {
                worker(s_ptr);
            }
        }
    };
    std::mutex m; //std::recursive_mutex
    std::condition_variable cv; //std::condition_variable_any
    bool stopped {false}; //atomic
    std::atomic<bool> ready {false};
    std::vector<std::thread> threads;
    std::list<std::shared_ptr<Data>> dataList;
    std::shared_ptr<Data> getData() {
        //lock если такой же!!!!!!!!!!! std::recursive_mutex
        std::shared_ptr<Data> s;
        std::lock_guard lk(m);
        if (!dataList.empty()) {
            s = dataList.front();
            dataList.pop_front();
        }
        return s;
    }
    bool dataEmpty() {
        std::unique_lock lk(m);
        return dataList.empty();
    }
};

/** @brief Thread (log) to output commands information to the console.
*
*/
class ConsoleOutputThread : public IOutputThread {
public:
    ConsoleOutputThread(std::ostream &o) : out(o) {}
    ~ConsoleOutputThread() = default;

private:
    std::ostream& out;
    void worker(std::shared_ptr<Data> s) override {
        out << s->data;
        out.flush();
    }
};

/** @brief Threads (file1, file2) to output commands information to the files.
*
*/
class FileOutputThreads : public IOutputThread {
public:
    FileOutputThreads(uint threadsNumber = 2) : IOutputThread(threadsNumber) {}
    ~FileOutputThreads() = default;

private:
    void worker(std::shared_ptr<Data> s) override {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        int i {1};
        std::string fileName(std::format("{}_{}_{}.log", s->name, ss.str(), i));
        while(std::filesystem::exists(fileName)) {
            fileName = std::format("{}_{}_{}.log", s->name, ss.str(), ++i);
        }
        std::ofstream out(fileName, std::ios::binary);
        if (out.is_open()) {
            out << s->data;
        }
    }
};