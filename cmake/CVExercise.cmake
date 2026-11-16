include_guard(GLOBAL)

find_package(OpenCV REQUIRED)

function(cv_add_exercise source)
    get_filename_component(source "${source}" ABSOLUTE)
    get_filename_component(target "${source}" NAME_WLE)
    get_filename_component(source_dir "${source}" DIRECTORY)

    if(TARGET "${target}")
        message(FATAL_ERROR
            "Cannot create executable '${target}' for '${source}': that target already exists. "
            "Give each standalone exercise .cpp file a unique filename."
        )
    endif()

    add_executable("${target}" "${source}")
    set_target_properties("${target}" PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        VS_DEBUGGER_WORKING_DIRECTORY "${source_dir}/build"
    )
    target_include_directories("${target}" PRIVATE
        "${source_dir}/include"
        ${OpenCV_INCLUDE_DIRS}
    )
    target_link_libraries("${target}" PRIVATE ${OpenCV_LIBS})

    # Run from the exercise's build folder. ../images reaches the originals;
    # ./images reaches this copy, so both starter-code conventions work.
    # Share one always-run asset target between programs in the same folder.
    string(MD5 assets_id "${source_dir}")
    set(assets_target "cv_assets_${assets_id}")
    if(NOT TARGET "${assets_target}")
        if(IS_DIRECTORY "${source_dir}/images")
            add_custom_target("${assets_target}"
                COMMAND "${CMAKE_COMMAND}" -E make_directory "${source_dir}/build"
                COMMAND "${CMAKE_COMMAND}" -E copy_directory
                    "${source_dir}/images" "${source_dir}/build/images"
                VERBATIM
            )
        else()
            add_custom_target("${assets_target}"
                COMMAND "${CMAKE_COMMAND}" -E make_directory "${source_dir}/build"
                VERBATIM
            )
        endif()
    endif()
    add_dependencies("${target}" "${assets_target}")
endfunction()
