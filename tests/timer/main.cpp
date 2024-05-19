
#include <thread>

//natl
#include <natl/timer.h>
#include <natl/printFormatted.h>
#include <natl/thread.h>

int main() {
	natl::Milliseconds<natl::i64> sleepTime(100);

	for ([[maybe_unused]] natl::Size i : natl::Repeat(10)) {
		natl::printlnf("start");
		natl::Timer timer;
		natl::thisThreadSleep(sleepTime);
		natl::printlnf("c: ", timer.getMillisecondsInt());

		std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
		natl::thisThreadSleep(sleepTime);
		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
		natl::printlnf("std: ", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	}
}