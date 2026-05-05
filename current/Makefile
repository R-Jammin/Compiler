# Copyright (c) 2023-2025 Dan O’Malley
# This file is licensed under the MIT License. See LICENSE for details.


# sudo apt install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
# sudo apt install qemu-user

default: build-frontend build-backend assemble build-driver

arm: build-frontend build-backend-arm assemble-arm build-driver-arm
	
build-frontend:
	mkdir ./logs
	mkdir ./build
	flex -o frontend_lex.yy.c frontend_lex.l
	yacc -d -v frontend_parser.y -Wcounterexamples
	clang -c parserstack.c
	clang -DYYDEBUG frontend_lex.yy.c y.tab.c parserstack.o -o ./build/frontend
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f y.output
	./build/frontend $(prog) 2> ./logs/frontend_parsing.log
	llc ./build/frontend_ir.ll -filetype=obj -o ./build/frontend_ir.o -O0
	objdump -D ./build/frontend_ir.o

build-backend:
	flex -o backend_lex.yy.c backend_lex.l
	yacc -d -v backend_parser.y -Wcounterexamples
	rm rm -f parserstack.o
	clang -c parserstack.c
	clang -DYYDEBUG backend_lex.yy.c y.tab.c parserstack.o -o ./build/backend
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f y.output
	./build/backend ./build/frontend_ir.ll 2> ./logs/backend_parsing.log
	cat ./build/prog.s

build-backend-arm:
	flex -o backend_lex.yy.c backend_lex.l
	yacc -d -v backend_parser-aarch64.y -Wcounterexamples
	rm rm -f parserstack.o
	clang -c parserstack.c
	clang -DYYDEBUG backend_lex.yy.c y.tab.c parserstack.o -o ./build/backend
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f y.output
	./build/backend ./build/frontend_ir.ll 2> ./logs/backend_parsing.log
	cat ./build/prog.s

assemble:
	nasm -felf64 ./build/prog.s -o ./build/prog.o
	objdump -D ./build/prog.o

assemble-arm:
	aarch64-linux-gnu-as ./build/prog.s -o ./build/prog.o
	aarch64-linux-gnu-objdump -D ./build/prog.o

build-driver:
	clang ./test-cases/x64/$(type)/driver.cpp ./build/prog.o -o ./build/driver
	clang -c $(prog) -o ./build/baseline_evaluator.o
	clang ./test-cases/x64/$(type)/evaluator.cpp ./build/prog.o ./build/baseline_evaluator.o -o ./build/evaluator
	cat $(prog)
	./build/evaluator

build-driver-arm:
	clang --target=aarch64-linux-gnu -static ./test-cases/arm/$(type)/driver.cpp ./build/prog.o -o ./build/driver
	clang --target=aarch64-linux-gnu -static -c $(prog) -o ./build/baseline_evaluator.o
	clang --target=aarch64-linux-gnu -static ./test-cases/arm/$(type)/evaluator.cpp ./build/prog.o ./build/baseline_evaluator.o -o ./build/evaluator
	cat $(prog)
	qemu-aarch64 ./build/evaluator

debug:
	gnome-terminal -- bash -c "gdb ./build/prog -x gdb_script.txt"


clean-frontend:
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f parserstack.o
	rm -f frontend_lex.yy.c
	rm -f frontend_ir.ll
	rm -f ./logs/*
	rm -f ./build/*
	

clean-backend:
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f parserstack.o
	rm -f backend_lex.yy.c
	rm -f ./logs/*
	rm -f ./build/*


clean-driver:
	rm -f ./build/driver
	rmdir ./build
	rmdir ./logs
	rm -f most_recent_test_log.txt


clean: clean-frontend clean-backend clean-driver
