project(ManualExperimentDemo LANGUAGES CXX)

set(SOURCES
	main.cpp)


add_executable(ManualExperimentDemo ${SOURCES})

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/windows/bin/SquidstatLibraryd.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>;
  ${CMAKE_SOURCE_DIR}/windows/thirdParty/Qt/bin/Qt5Cored.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>;
  ${CMAKE_SOURCE_DIR}/windows/thirdParty/Qt/bin/Qt5SerialPortd.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMENT "Copy dll file to" $<TARGET_FILE_DIR:${PROJECT_NAME} "directory" VERBATIM
  )
endif()