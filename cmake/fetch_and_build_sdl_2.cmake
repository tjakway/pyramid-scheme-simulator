function(fetch_and_build_sdl2_release)
    set(SDL2_RELEASE SDL2-2.0.7)
    set(SDL2_RELEASE_FILENAME ${SDL2_RELEASE}.zip)

    #make the extract dir if it doesn't exist
    set(SDL2_EXTRACT_DIR ${CMAKE_BINARY_DIR}/sdl2_extract)

    if(NOT EXISTS ${SDL2_EXTRACT_DIR})
        file(MAKE_DIRECTORY ${SDL2_EXTRACT_DIR})
    endif()

    #download the zip
    set(SDL2_DOWNLOAD_DEST ${SDL2_EXTRACT_DIR}/${SDL2_RELEASE_FILENAME})

    if(NOT EXISTS ${SDL2_DOWNLOAD_DEST})
        message(STATUS "Downloading SDL2...")
        file(DOWNLOAD "https://www.libsdl.org/release/${SDL2_RELEASE_FILENAME}" 
            ${SDL2_DOWNLOAD_DEST} SHOW_PROGRESS)

        execute_process(COMMAND unzip ${SDL2_RELEASE_FILENAME} 
            WORKING_DIRECTORY ${SDL2_EXTRACT_DIR}
            OUTPUT_QUIET)

        message(STATUS "Building SDL2...")
        #build the archive we just extracted
        add_subdirectory(${SDL2_EXTRACT_DIR}/${SDL2_RELEASE} ${CMAKE_BINARY_DIR}/sdl2_bin)

        include_directories(${SDL2_EXTRACT_DIR}/${SDL2_RELEASE}/include )

    endif()
endfunction()
