set(Library LegoClassifierContoursUtils)

add_subdirectory(src)
target_include_directories(${Library} PUBLIC inc)

add_subdirectory(tests)