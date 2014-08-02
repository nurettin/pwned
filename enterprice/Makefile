.PHONY: test_multi_candle_stream test_candle_stream test_mysql test_time test_memory test

WARNINGS=-Wextra -Wno-missing-field-initializers -Wall -Wpedantic -Wfatal-errors

test: test_candle_stream test_multi_candle_stream test_mysql test_time test_memory
	@-rm a.out
test_candle_stream:
	$(CXX) -std=c++1y $(WARNINGS) -DENTERPRICE_IO_CANDLESTREAM_HPP_TEST -x c++ candle_stream.hpp && ./a.out

test_multi_candle_stream:
	$(CXX) -std=c++1y $(WARNINGS) -DENTERPRICE_IO_MULTICANDLESTREAM_HPP_TEST -x c++ multi_candle_stream.hpp && ./a.out

test_mysql:
	$(CXX) -std=c++1y $(WARNINGS) -DMYSQL_HPP_TEST -x c++ mysql.hpp -l mysqlclient && ./a.out

test_time:
	$(CXX) -std=c++1y $(WARNINGS) -DTIME_HPP_TEST -x c++ time.hpp && ./a.out

test_memory:
	$(CXX) -std=c++1y $(WARNINGS) -DMEMORY_HPP_TEST -x c++ memory.hpp && ./a.out

