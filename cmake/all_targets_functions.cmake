#add a property to both the main library and the test target
function(all_targets_compile_property)
    #see http://stackoverflow.com/questions/5096881/does-set-target-properties-in-cmake-override-cmake-cxx-flags 
    target_compile_options(${MAIN_LIB_TARGET} PRIVATE ${ARGN})
    target_compile_options(${MAIN_EXE_TARGET} PRIVATE ${ARGN})
    target_compile_options(${TEST_TARGET} PRIVATE ${ARGN})
endfunction()

function(all_targets_compile_definitions def)
    target_compile_definitions(${MAIN_LIB_TARGET} PRIVATE ${def})
    target_compile_definitions(${MAIN_EXE_TARGET} PRIVATE ${def})
    target_compile_definitions(${TEST_TARGET} PRIVATE ${def})
endfunction()

#NOTE: this function probably isn't needed since you can link everything to the main
#lib target and then link that to other targets
function(all_targets_link_libraries)
    target_link_libraries(${MAIN_LIB_TARGET} PRIVATE ${ARGN})
    target_link_libraries(${MAIN_EXE_TARGET} PRIVATE ${ARGN})
    target_link_libraries(${TEST_TARGET} PRIVATE ${ARGN})
endfunction()
