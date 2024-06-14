
//natl
#include <natl/sync/thread.h>
#include <natl/sync/smartThread.h>
#include <natl/sync/mutex.h>
#include <thread>


void work (natl::ThreadStopAlert stopControl) noexcept {
	while (!stopControl.stopRequested()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "did work\n";
	}
	return;
}

constexpr int compileTimeTest() {
	return 0;
}

int main() {
	natl::SmartThread thread(work);
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	thread.requestStop();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}