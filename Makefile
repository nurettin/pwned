all: run_tests

run_tests:
	@cd pretty && $(MAKE) && cd ..
	@cd curl && $(MAKE) && cd ..
	@cd leveldb && $(MAKE) && cd ..
	@cd jsoncpp && $(MAKE) && cd ..
	@cd view && $(MAKE) && cd ..
	@cd progress && $(MAKE) && cd ..
	@printf "test LOC:" && wc -l */tests.cpp | grep total
	@printf "code LOC:" && wc -l */*.hpp | grep total

