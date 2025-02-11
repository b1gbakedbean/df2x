find_package(Git QUIET)

if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --dirty=-dev --always
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                  RESULT_VARIABLE GIT_DESCRIBE_RESULT
                  OUTPUT_VARIABLE PROJECT_GIT_REVISION
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if (NOT GIT_DESCRIBE_RESULT EQUAL "0")
    message(FATAL_ERROR "`git describe --tags --dirty=-dev --always` failed: ${GIT_DESCRIBE_RESULT}")
  endif()
else()
  message(FATAL_ERROR "Git wasn't found or .git doesn't exist")
endif()