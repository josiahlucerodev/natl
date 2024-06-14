
//natl
#include <natl/util/iteration.h>
#include <natl/container/colony.h>

int main() {
	natl::Colony<natl::Size> colony(natl::ColonyLimits(30, 300));

	for ([[maybe_unused]] natl::Size index : natl::Repeat(100)) {
		colony.insert(index);
	}

	///colony.clear();
	colony.shrink();

	decltype(colony)::iterator iter = colony.getIteratorAtIndex(1).value();
	for ([[maybe_unused]] natl::Size index : natl::Repeat(3)) {
		colony.erase(iter);
		iter++;
	}
	iter = colony.getIteratorAtIndex(5).value();
	for ([[maybe_unused]] natl::Size index : natl::Repeat(2)) {
		colony.erase(iter);
		iter++;
	}

	colony.erase(colony.getIteratorAtIndex(4).value());

	natl::Colony<natl::Size> colony2 = colony;
	colony2.reshape(natl::ColonyLimits(30, 50), 2);
}