# Copyright (c) 2023-2025 Dan O’Malley
# This file is licensed under the MIT License. See LICENSE for details.


default: build-frontend build-backend assemble build-driver
	
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

assemble:
	nasm -felf64 ./build/prog.s -o ./build/prog.o
	objdump -D ./build/prog.o

build-driver:
	clang ./test-cases/$(type)/driver.cpp ./build/prog.o -o ./build/driver
	clang -c $(prog) -o ./build/baseline_evaluator.o
	clang ./test-cases/$(type)/evaluator.cpp ./build/prog.o ./build/baseline_evaluator.o -o ./build/evaluator
	cat $(prog)
	./build/evaluator

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
