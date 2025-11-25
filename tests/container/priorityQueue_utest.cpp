//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/container/priorityQueue.h>
#include <natl/container/stringView.h>
#include <natl/util/test.h>

using value_type = natl::Size;

template<typename PriorityQueueType, natl::Bool Ascending>
struct PriorityQueueTestSuite : public natl::TestSuite<PriorityQueueTestSuite<PriorityQueueType, Ascending>> {
	using priority_queue_type = PriorityQueueType;
	constexpr static inline natl::Bool ascending = Ascending;
};

template<typename test_suite_type = PriorityQueueTestSuite<natl::PriorityQueue<value_type>, false>>
struct PriorityQueueTestSuiteTests {
	using priority_queue_type = typename test_suite_type::priority_queue_type;

	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::Size count = 0;
		if (natl::isConstantEvaluated()) {
			count = 10;
		} else {
			count = 10000;
		}

		natl::DynArray<value_type> values;
		for (natl::Size i : natl::Repeat(count)) {
			values.pushBack(i);
		}
		natl::shuffle(values.begin(), values.end(), ts.getRNG());

		priority_queue_type pqueue(values);
		ts.assertTrue(natl::isHeap(pqueue.getContainer().begin(), pqueue.getContainer().end(), typename priority_queue_type::value_compare()));
		ts.assertEqual(count, pqueue.size());

		values.clear();
		for (natl::Size i : natl::Repeat(count)) {
			values.pushBack(count + i);
		}
		natl::shuffle(values.begin(), values.end(), ts.getRNG());
		count *= 2;

		for (value_type value : values) {
			pqueue.push(value);
		}
		ts.assertTrue(natl::isHeap(pqueue.getContainer().begin(), pqueue.getContainer().end(), typename priority_queue_type::value_compare()));
		ts.assertEqual(count, pqueue.size());

		const natl::Size oc = count;
		while (not pqueue.isEmpty()) {
			if (test_suite_type::ascending) {
				ts.assertEqual(oc - count, pqueue.top());
			} else {
				ts.assertEqual(count - 1, pqueue.top());
			}

			pqueue.pop();
			count--;
		}
		ts.assertEqual(count, 0);
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};


struct StdPriorityQueueTestSuite : public PriorityQueueTestSuite<natl::PriorityQueue<value_type>, true> {};
NATL_REGISTER_TESTS(StdPriorityQueueTestSuite, PriorityQueueTestSuiteTests);

struct MinHeapTestSuite : public PriorityQueueTestSuite<natl::MinHeap<value_type>, true> {};
NATL_REGISTER_TESTS(MinHeapTestSuite, PriorityQueueTestSuiteTests);

struct SmallMinHeapTestSuite : public PriorityQueueTestSuite<natl::SmallMinHeap<value_type, 100>, true> {};
NATL_REGISTER_TESTS(SmallMinHeapTestSuite, PriorityQueueTestSuiteTests);

struct MaxHeapTestSuite : public PriorityQueueTestSuite<natl::MaxHeap<value_type>, false> {};
NATL_REGISTER_TESTS(MaxHeapTestSuite, PriorityQueueTestSuiteTests);

struct SmallMaxHeapTestSuite : public PriorityQueueTestSuite<natl::SmallMaxHeap<value_type, 100>, false> {};
NATL_REGISTER_TESTS(SmallMaxHeapTestSuite, PriorityQueueTestSuiteTests);

NATL_TESTS_MAIN();