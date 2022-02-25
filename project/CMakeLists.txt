
cmake_minimum_required(VERSION 3.16)


set (3rd_part  "3rd_part")

set(PRE_CONFIGURE_FILE "${CMAKE_SOURCE_DIR}/cmake/git.h.in")
set(POST_CONFIGURE_FILE "${CMAKE_BINARY_DIR}/git.h")
include (${CMAKE_SOURCE_DIR}/cmake/git_watcher.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/functions.cmake)
include(ExternalProject)



add_library(restbed INTERFACE)
set_target_properties(restbed PROPERTIES
        "INTERFACE_INCLUDE_DIRECTORIES" "${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/include"
        "INTERFACE_LINK_LIBRARIES" "${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/library"
)

find_package(restbed 
	PATHS
	"${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/usr/share"
	)
if(NOT restbed_FOUND)
	ExternalProject_Add(restbed_external
		GIT_REPOSITORY https://github.com/corvusoft/restbed.git
		#GIT_SUBMODULES_RECURSE ON
		#GIT_CONFIG advice.detachedHead=false
		CMAKE_ARGS -DBUILD_SSL=NO  -DBUILD_SHARED=YES -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		INSTALL_DIR ${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}
		DEPENDS restbed
		)
	add_custom_target(restbed_install ALL DEPENDS  restbed  restbed_external)
	add_custom_command(TARGET   restbed_install
		COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/usr/share
		COMMAND cp ${CMAKE_SOURCE_DIR}/cmake/restbed-config.cmake ${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/usr/share
		)

else()
	add_custom_target(restbed_install ALL    )	
	add_custom_command(TARGET   restbed_install
		COMMAND cp ${CMAKE_SOURCE_DIR}/cmake/restbed-config.cmake ${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/usr/share

		)
endif()


project(appserver)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)








set (server_SRC
	IRestServer.cpp
	DataServerResetAPI.cpp 
	dbmanage.cpp
	data_server_db.cpp
	DBConnectionParams.cpp
	main.cpp
	)
set (server_H
	DataServerResetAPI.h 
	dbmanage.h
	data_server_db.h
	DBConnectionParams.h
	IRestServer.h
	)





add_executable(server 
	${server_SRC}
	${server_H}
	)

add_dependencies(server  
	restbed
	 )
target_link_libraries(server PRIVATE
	-Wl,--start-group  	
        ${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/library/librestbed.so.4.7
	pthread
	jsoncpp 
	curl
	#restbed
	pqxx 
	pq
	-Wl,--end-group
	)

target_include_directories(server PUBLIC
	${PROJECT_BINARY_DIR}/	
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/include/
	)
