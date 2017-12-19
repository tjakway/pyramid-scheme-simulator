
function(set_signal_handling_definition)
    if(ENABLE_SIGNAL_HANDLER)
        add_definitions("-DENABLE_SIGNAL_HANDLER")
    endif()
endfunction()
