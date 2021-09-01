set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
	include_directories(
		../windows/include
		../windows/thirdParty/Qt/include/QtCore
		../windows/thirdParty/Qt/include/
	)

	link_libraries(${CMAKE_SOURCE_DIR}/windows/thirdParty/Qt/bin/Qt5Cored.lib ${CMAKE_SOURCE_DIR}/windows/bin/SquidstatLibraryd.lib)
endif()

add_subdirectory(ControlFlowDemo)
add_subdirectory(BasicDemo)
add_subdirectory(ManualExperimentDemo)