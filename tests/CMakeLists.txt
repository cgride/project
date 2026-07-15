function(cgride_project_add_test name source)
  add_executable(${name}
    ${source}
  )

  target_link_libraries(${name}
    PRIVATE
      cgride::project
  )

  target_compile_features(${name}
    PRIVATE
      cxx_std_23
  )

  set_target_properties(${name}
    PROPERTIES
      CXX_EXTENSIONS OFF
  )

  add_test(
    NAME ${name}
    COMMAND ${name}
  )
endfunction()

cgride_project_add_test(cgride_project_source_set_test source_set_test.cpp)
cgride_project_add_test(cgride_project_requirement_test requirement_test.cpp)
cgride_project_add_test(cgride_project_target_test target_test.cpp)
cgride_project_add_test(cgride_project_project_test project_test.cpp)
cgride_project_add_test(cgride_project_validation_test validation_test.cpp)
