
//natl
#include <natl/thread.h>
#include <natl/smartThread.h>
#include <natl/mutex.h>
#include <thread>


void work (natl::ThreadStopAlert stopControl) noexcept {
	while (!stopControl.stopRequested()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "did work\n";
	}
	return;
}

template<typename Functor, typename... Args>
consteval natl::Bool test(Functor&& functor, Args&&... args) {
	return natl::impl::DoesFunctorTakeSmartThreadStopAlert<Functor, Args...>;
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