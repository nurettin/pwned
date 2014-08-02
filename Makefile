.PHONY: all update run_tests

all: update run_tests

update:
	git submodule update --init --recursive

run_tests:
	@cd sqlsafe && $(MAKE) && cd .. && \
	cd pretty && $(MAKE) && cd .. && \
	cd hcxselect && $(MAKE) -s && cd .. && \
	cd curl && $(MAKE) && cd .. && \
	cd leveldb && $(MAKE) && cd .. && \
	cd jsoncpp && $(MAKE) && cd .. && \
	cd view && $(MAKE) && cd .. && \
	cd progress && $(MAKE) && cd .. && \
	cd parameters && $(MAKE) && cd .. && \
	cd re2 && $(MAKE) -s -j 4 && cd .. && \
	cd server && $(MAKE) && cd .. && \
	cd pew && $(MAKE) && cd .. && \
	cd active_proto && $(MAKE) && cd .. && \
	cd ecto1 && $(MAKE) && cd .. && \
	cd enterprice && $(MAKE) test && cd ..
	@printf "test LOC:" && wc -l */tests.cpp | grep total
	@printf "code LOC:" && wc -l */*.hpp | grep total

