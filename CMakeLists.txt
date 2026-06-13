cmake_minimum_required(VERSION 4.2)
project(CJSON_Fork C)

set(CMAKE_C_STANDARD 11)

add_executable(CJSON_Fork main.c
        Cjson.h
        Cjson.c
        list.h)

add_executable(UnitTest Tests/main.c
        Tests/unity/unity.c
        Tests/unity/unity.h
        Tests/unity/unity_internals.h
        Tests/unity/unity_config.h
        Cjson.c
        Cjson.h
        Tests/test_cjson_scanner.c
        Tests/test_cjson_scanner.h

)