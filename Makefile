all: run_tests

run_tests:
	cd debug && make && cd ..
	cd curl && make && cd ..
	cd leveldb && make && cd ..
	cd jsoncpp && make && cd ..
	@printf "test LOC\n" && wc -l */tests.cpp
	@printf "code LOC\n" && wc -l */*.hpp

