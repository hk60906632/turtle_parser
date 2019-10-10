LIBS = -lm
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
SDL_link = `sdl2-config --cflags` `sdl2-config --libs` $(LIBS)


interpreter: turtle_interpreter
test_interpreter: test_turtle_interpreter
parser: turtle_parser
test_parser:test_turtle_parser
extension:extension_interpreter

turtle_parser: turtle_parser.c
	$(CC) -o turtle_parser turtle_parser.c $(CFLAGS)

test_turtle_parser: test_turtle_parser.c
	$(CC) -o test_turtle_parser test_turtle_parser.c

turtle_interpreter: turtle_interpreter.c  neillsdl2.c
	$(CC) -o turtle_interpreter turtle_interpreter.c  neillsdl2.c $(SDL_link) $(CFLAGS)

test_turtle_interpreter:test_turtle_interpreter.c
	$(CC) -o test_turtle_interpreter test_turtle_interpreter.c -lm $(LIBS)

extension_interpreter:extension_interpreter.c neillsdl2.c
	$(CC) -o extension_interpreter extension_interpreter.c neillsdl2.c $(SDL_link) $(CFLAGS)

run_parser: parser
				./turtle_parser turtle_parser_code.txt

run_test_parser: test_parser
				./test_turtle_parser test_turtle_parser_code.txt

run_interpreter_1: interpreter
				./turtle_interpreter interpreter_code_1.txt

run_interpreter_2: interpreter
				./turtle_interpreter interpreter_code_2.txt

run_interpreter_3: interpreter
				./turtle_interpreter interpreter_code_3.txt

run_test_interpreter: test_interpreter
				./test_turtle_interpreter test_turtle_interpreter_code.txt

run_extension_1: extension
				./extension_interpreter extension_demonstration_1.txt

run_extension_2: extension
				./extension_interpreter extension_demonstration_2.txt

run_extension_3: extension
				./extension_interpreter extension_demonstration_3.txt

clean:
		rm -fr turtle_interpreter test_turtle_interpreter extension_interpreter turtle_parser test_turtle_parser
