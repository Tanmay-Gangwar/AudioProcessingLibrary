Final:
	g++ -fopenmp main.cpp -lmkl_intel_lp64 -lmkl_core -lmkl_gnu_thread -lpthread -o yourcode.out;
# g++ main.cpp -o yourcode.out;