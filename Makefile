CC=gcc
CFLAGS=-I.
DEPS = services/services.h
OBJ = hldlc.o
EXE = runme.out
TESTS = $(wildcard unity_tests/*.c)
MOCKS = $(wildcard unity_tests/mocks/*.c)


%.o: services/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

runme.out: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE) *.out

clean-tests:
	rm -rf tests/mocks

unity_tests/test_hldlc.c: hldlc.o
	gcc -std=c99  $@ $< Unity/src/unity.c CMock/src/cmock.c unity_tests/mocks/Mockservices.c -o test_hldlc.out -I./services -I./CMock/src -I./Unity/src -I./unity_tests/mocks
	./test_hldlc.out

test: clean $(TESTS)
	echo $(TESTS)

mock-functions:
	mkdir -p unity_tests/mocks
	ruby CMock/lib/cmock.rb -o.cmock.yml $(DEPS)

.PHONY: test
.PHONY: clean
.PHONY: clean-tests
.PHONY: mock-functions
