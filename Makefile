complile_main:
	clang++ -std=c++20 main.cpp -o main
run_ob:
	make complile_main
	./main