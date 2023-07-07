#include <cstdlib>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <pthread.h>

class Thread {
public:
    int start() {
        return pthread_create(&_ThreadId, nullptr, Thread::thread_func, this);
    }
    int wait() {
        return pthread_join(_ThreadId, nullptr);
    }

protected:
    Thread() = default;
    Thread(const Thread&);

    virtual ~Thread() = default;
    virtual void run() = 0;

    static void* thread_func(void* d) {
        (static_cast <Thread*>(d))->run();
        return nullptr;
    }

private:
    pthread_t _ThreadId;
};

class TestingThread : public Thread {
public:
    TestingThread(const char* pcszText) : _pcszText(pcszText) {}
    virtual void run() {
        for (unsigned int i = 0; i < 20; i++, usleep(1000)) std::cout << _pcszText << std::endl;
    }
protected:
    const char* _pcszText;
};

int main(int argc, char* argv[], char* envp[]) {
    TestingThread ThreadA("a");
    TestingThread ThreadB("b");
    return ThreadA.start() || ThreadB.start() || ThreadA.wait() || ThreadB.wait() ? EXIT_FAILURE : EXIT_SUCCESS;
}