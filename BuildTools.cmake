# some build tools, not covering cross-compiling since i dont do that for this app and to skip anoying workaround and boilerplate
add_executable(bytepack src2/BuildTools/bytepack.c)

add_executable(imgpack src2/BuildTools/imgpack.c)
target_include_directories(imgpack PRIVATE src2/BuildTools)
target_link_libraries(imgpack PRIVATE m)

function(bytepack_)
  cmake_parse_arguments(
    PA
    "" #boolean
    "OUTPUT;" #single value
    "SOURCES" #multiple values
    "${ARGN}"
  )
  set(OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/built_assets/)
  file(MAKE_DIRECTORY ${OUTDIR})

  set(SOURCES "")
  foreach(src ${PA_SOURCES})
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
  endforeach()

  add_custom_command(
    OUTPUT ${OUTDIR}${PA_OUTPUT}.h ${OUTDIR}/${PA_OUTPUT}.c
    COMMAND bytepack ${OUTDIR}${PA_OUTPUT} ${SOURCES}

    DEPENDS ${SOURCES} bytepack
    VERBATIM
  )
  add_custom_target(${PA_OUTPUT}_file DEPENDS ${OUTDIR}${PA_OUTPUT}.c)
  add_library(${PA_OUTPUT} STATIC ${OUTDIR}${PA_OUTPUT}.c)
  add_dependencies(${PA_OUTPUT} ${PA_OUTPUT}_file)
  target_include_directories(${PA_OUTPUT} PUBLIC ${OUTDIR})
endfunction()

function(imgpack_)
  cmake_parse_arguments(
    PA
    "" #boolean
    "OUTPUT;CHANNELS;FLIP" #single value
    "SOURCES" #multiple values
    "${ARGN}"
  )
  set(OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/built_assets/)
  file(MAKE_DIRECTORY ${OUTDIR})

  set(SOURCES "")
  foreach(src ${PA_SOURCES})
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
  endforeach()

  add_custom_command(
    OUTPUT ${OUTDIR}${PA_OUTPUT}.h ${OUTDIR}/${PA_OUTPUT}.c
    COMMAND imgpack ${OUTDIR}${PA_OUTPUT} ${PA_CHANNELS} ${PA_FLIP} ${SOURCES}

    DEPENDS ${SOURCES} imgpack
    VERBATIM
  )
  add_custom_target(${PA_OUTPUT}_file DEPENDS ${OUTDIR}${PA_OUTPUT}.c)
  add_library(${PA_OUTPUT} STATIC ${OUTDIR}${PA_OUTPUT}.c)
  add_dependencies(${PA_OUTPUT} ${PA_OUTPUT}_file)
  target_include_directories(${PA_OUTPUT} PUBLIC ${OUTDIR})
endfunction()
