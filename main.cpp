#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
int totalAles = 0;

void fillAles() {
    for (int i = 1; i <= 20; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            totalAles = i;
            std::cout << "Filled " << i << " ale" << (i > 1 ? "s" : "") << std::endl;
            if (totalAles == 20) {
                ready = true;
                cv.notify_one();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void drinkAles() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return ready; });
    for (int i = 19; i >= 0; --i) {
        totalAles = i;
        std::cout << "Drank ale " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    std::thread fillThread(fillAles);
    std::thread drinkThread(drinkAles);

    fillThread.join();
    drinkThread.join();

    return 0;
}
