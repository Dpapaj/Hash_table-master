add_executable(app main.c)

include(FetchContent)
FetchContent_Declare(
  ioutils
  GIT_REPOSITORY https://ioutils:aFiG6LqzyabnzhxzbhNC@git.vyuka.fai.utb.cz/ads/common/ioutils.git
)

FetchContent_MakeAvailable(ioutils)

target_link_libraries(app PUBLIC hashTable ioutils)
