// This an thread-safety implementation of the singleton pattern.

#ifdef _WIN32
#include <Windows.h>
void MySleep(unsigned long milliseconds) {
	Sleep(milliseconds);
}
#elif __linux__
#include <unistd.h>
void MySleep(unsigned long milliseconds) {
	usleep(milliseconds * 1000);
}
#endif

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Singleton {
public:
	static Singleton* GetInstance();

private:
	Singleton() { std::cout << "Singleton constructor" << std::endl; }

	static std::mutex mut;
	static Singleton* instance;
};

std::mutex Singleton::mut;
Singleton* Singleton::instance = nullptr;

Singleton* Singleton::GetInstance() {
	if (!instance) {
		mut.lock();
		if (!instance)
			instance = new Singleton();
		mut.unlock();
	}
	
	return instance;
}

std::mutex mut;

void fun(int id) {
	auto instance = Singleton::GetInstance();
	mut.lock();
	MySleep(100);
	std::cout << "#" << std::dec << id;
	std::cout << " instance = " << std::hex << instance << std::endl;
	mut.unlock();
}

int main() {
	std::vector<std::thread> ths;
	for (int i = 0; i < 10; ++i)
		ths.push_back(std::thread(fun, i));
	for (int i = 0; i < 10; ++i)
		ths[i].join();
}