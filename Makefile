complile_main:
	clang++ -std=c++20 -g -I/opt/homebrew/Cellar/fmt/11.0.2/include -I/Users/ravi.gamage/personal/order-book/include \
 	src/main.cpp -o src/main
run_ob:
	make complile_main
	src/main
	make clean
run_ob_debug:
	make complile_main
	lldb src/main
	make clean
clean:
	-rm src/main