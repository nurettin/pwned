.PHONY: all run_tests

all: run_tests

run_tests:
	@cd pretty && $(MAKE) && cd .. && \
	cd hcxselect && $(MAKE) -s -j 4 && cd .. && \
	cd curl && $(MAKE) && cd .. && \
	cd leveldb && $(MAKE) && cd .. && \
	cd jsoncpp && $(MAKE) && cd .. && \
	cd view && $(MAKE) && cd .. && \
	cd progress && $(MAKE) && cd .. && \
	cd parameters && $(MAKE) && cd .. && \
	cd re2 && $(MAKE) -s -j 4 && cd .. && \
	cd server && $(MAKE) && cd ..
	@printf "test LOC:" && wc -l */tests.cpp | grep total
	@printf "code LOC:" && wc -l */*.hpp | grep total
