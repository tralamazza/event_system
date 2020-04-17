add_executable(estest estest.cpp main.cpp)
target_link_libraries(estest PRIVATE eventsystem)
add_test(NAME event_system_test COMMAND estest)
