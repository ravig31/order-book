complile_main:
	clang++ -std=c++20 -I/opt/homebrew/Cellar/fmt/11.0.2/include main.cpp -o main -g
run_ob:
	make complile_main
	lldb ./main
clean:
	-rm *.o $(objects) main