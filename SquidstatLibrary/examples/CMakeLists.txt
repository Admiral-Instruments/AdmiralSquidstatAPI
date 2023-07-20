set(CMAKE_DEBUG_POSTFIX d)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

if(WIN32)
	include_directories(
		../windows/include
		../windows/thirdParty/Qt/include/QtCore
		../windows/thirdParty/Qt/include/
	)

	link_libraries(${CMAKE_SOURCE_DIR}/windows/thirdParty/Qt/bin/Qt5Cored.lib ${CMAKE_SOURCE_DIR}/windows/bin/SquidstatLibraryd.lib)
endif()

if(NOT WIN32 AND APPLE AND NOT CMAKE_SYSTEM_NAME MATCHES "Linux")
    include_directories(
            ../mac/include
            ../mac/thirdParty/Qt/include/QtCore
            ../mac/thirdParty/Qt/include/
    )

    find_library(QtLib
        NAME QtCore QtCored
        HINTS ${CMAKE_SOURCE_DIR}/mac/thirdParty/Qt/bin
        )

    find_library(SquidstatLibrarylib_debug
        NAME SquidstatLibraryd
        HINTS ${CMAKE_SOURCE_DIR}/mac/bin
        )

    find_library(SquidstatLibrarylib_release
            NAME SquidstatLibrary
            HINTS ${CMAKE_SOURCE_DIR}/mac/bin
    )

    link_libraries(${SquidstatLibrarylib_debug} ${QtLib})
endif()

if(NOT WIN32 AND NOT APPLE AND CMAKE_SYSTEM_NAME MATCHES "Linux")
    include_directories(
            ../Linux/include
            ../Linux/thirdParty/Qt/include/QtCore
            ../Linux/thirdParty/Qt/include/
    )

    find_library(Qt5ore
        NAME libQt5Core.so.5
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
        )
        
    find_library(Qt5SerialPort
        NAME libQt5SerialPort.so.5 libicudata.so.56 libicui18n.so.56 libicuuc.so.56
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
        )
        
    find_library(libcudata
        NAME libicudata.so.56 libicui18n.so.56 libicuuc.so.56
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
        )	
    
    find_library(libcui18n
        NAME libicui18n.so.56 libicuuc.so.56
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
        )
        
    find_library(libicuuc
        NAME libicuuc.so.56
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
        )
        
    find_library(SquidstatLibrarylib_debug
        NAME SquidstatLibraryd
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
        )

    find_library(SquidstatLibrarylib_release
        NAME SquidstatLibrary
        HINTS ${CMAKE_SOURCE_DIR}/Linux/bin
    ) 
    
    link_libraries(${SquidstatLibrarylib_debug} ${Qt5ore} ${Qt5SerialPort} ${libcudata} ${libcui18n} ${libicuuc})
    add_compile_options(-fPIC)
endif()

add_subdirectory(ControlFlowDemo)
add_subdirectory(BasicDemo)
add_subdirectory(ManualExperimentDemo)
add_subdirectory(DataOutputToCSVDemo)
add_subdirectory(AcExperimentDemo)
add_subdirectory(NonBlockTestDemo)
add_subdirectory(FirmwareUpdateDemo)
add_subdirectory(AutoFirmwareUpdateDemo)