PROJECT_NAME = s21_3dViewer2
DIR = $(shell pwd)
# sudo apt-get -y install qt6-base-dev

.PHONY: all test clean style mem  compile lcov

ALL_FILES = $(PROJECT_NAME)/main.cpp $(PROJECT_NAME)/Controller/* $(PROJECT_NAME)/Model/* $(PROJECT_NAME)/View/*.cpp $(PROJECT_NAME)/View/*.h objparser/*

all: install test
ifeq ($(shell uname -s),Linux)
	./build/$(PROJECT_NAME)
else
	open build/$(PROJECT_NAME).app
endif

install:
	@if [ ! -d build ] ; then echo "creating build" ; mkdir build; fi
	cd build && cmake ../$(PROJECT_NAME)/ && make

uninstall: clean

dvi:
ifeq ($(shell uname -s),Linux)
	xdg-open readme.pdf
else
	open readme.pdf
endif

dist: clean install
	mkdir $(PROJECT_NAME)_DIST
ifeq ($(shell uname -s),Linux)
	cp build/$(PROJECT_NAME) $(PROJECT_NAME)_DIST/$(PROJECT_NAME)
else
	cp -R build/$(PROJECT_NAME).app $(PROJECT_NAME)_DIST
endif
	tar cvzf $(PROJECT_NAME).tar.gz $(PROJECT_NAME)_DIST
	rm -rf $(PROJECT_NAME)_DIST build

test:
	@if [ ! -d build ] ; then echo "creating build" ; mkdir build; fi
	cd build && cmake ../$(PROJECT_NAME)/ && make tests && ./tests

clean:
	rm -rf build lcoverage $(PROJECT_NAME)_DIST

style:
	clang-format -style=google -i $(ALL_FILES)
	cpplint --filter=-build/include_subdir --headers=h,inl $(ALL_FILES)
	cppcheck --check-config --enable=all --inconclusive --suppress=missingIncludeSystem --language=c++ $(ALL_FILES)

mem:
ifeq ($(shell uname -s),Linux)
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./build/tests
else
	leaks --atExit -- ./build/tests
endif


lcov:
	@echo "Generating coverage report..."
	rm -rf lcoverage
	cd build && make gcov && cd .. && mkdir lcoverage
ifeq ($(shell uname -s),Linux)
	lcov --capture --directory $(DIR) --output-file lcoverage/main_coverage.info --no-external --ignore-errors gcov
	lcov -r lcoverage/main_coverage.info "*test*" -o lcoverage/main_coverage.info
	lcov -r lcoverage/main_coverage.info "*autogen*" -o lcoverage/main_coverage.info
#	lcov -r lcoverage/main_coverage.info "*Controller*" -o lcoverage/main_coverage.info
	genhtml lcoverage/main_coverage.info --output-directory lcoverage
	xdg-open lcoverage/index.html
else
	lcov --capture --directory $(DIR) --output-file lcoverage/main_coverage.info --no-external --ignore-errors inconsistent,gcov
	lcov -r lcoverage/main_coverage.info "test" -o lcoverage/main_coverage.info --ignore-errors inconsistent
	lcov -r lcoverage/main_coverage.info "autogen" -o lcoverage/main_coverage.info --ignore-errors inconsistent
#	lcov -r lcoverage/main_coverage.info "Controller" -o lcoverage/main_coverage.info --ignore-errors inconsistent
	lcov -r lcoverage/main_coverage.info "cereal" -o lcoverage/main_coverage.info --ignore-errors inconsistent
	genhtml lcoverage/main_coverage.info --output-directory lcoverage --ignore-errors inconsistent,unmapped
	open lcoverage/index.html
endif

