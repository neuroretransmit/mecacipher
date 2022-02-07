BIN := mecacipher
SRCDIR := src

$(BIN): $(SRCDIR)/main.cc
	./scripts/format.sh
	$(CXX) -Wall -Werror -Wextra -O3 -g $< -o $@

clean:
	rm -fv $(BIN)

