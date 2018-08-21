file(GLOB SRCS "*.cpp")

link_libraries(cuttle-parser)

add_library(cuttle-generator SHARED ${SRCS})
