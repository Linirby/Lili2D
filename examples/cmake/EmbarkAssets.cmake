function(embark_target_assets TARGET_NAME)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets"
            COMMENT "Embarking assets/ directory for ${TARGET_NAME}"
        )
    endif()

    file(GLOB ASSET_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.jpeg"
    )
    if(ASSET_FILES)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${ASSET_FILES}
                "$<TARGET_FILE_DIR:${TARGET_NAME}>"
            COMMENT "Embarking media assets for ${TARGET_NAME}"
        )
    endif()

    file(GLOB SHADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hlsl"
    )
    if(SHADER_FILES)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${TARGET_NAME}>/src"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SHADER_FILES}
                "$<TARGET_FILE_DIR:${TARGET_NAME}>/src"
            COMMENT "Embarking shader files for ${TARGET_NAME}"
        )
    endif()
endfunction()
