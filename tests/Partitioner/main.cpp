import natl.partitioner;

int main() {
	const char* fooString = foo();
	natl::Partition<char> charPartition;
	natl::DynamicPartitioner<char> charPartitioner;
	charPartitioner.resize(100);
	charPartition = charPartitioner.newPartition(4);


	std::cout << fooString << "\n";
	int temp = 8;
}