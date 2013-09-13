all: run_tests

run_tests:
	@cd debug && make && cd ..
	@cd curl && make && cd ..
	@cd leveldb && make && cd ..
	@cd jsoncpp && make && cd ..
	@printf "test LOC:" && wc -l */tests.cpp | grep total
	@printf "code LOC:" && wc -l */*.hpp | grep total

