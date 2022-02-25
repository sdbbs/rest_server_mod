


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

endif()


include_directories(
	${PROJECT_BINARY_DIR}/	
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/include/
	)



set (dataServer_SRC
	IRestServer.cpp
	DataServerResetAPI.cpp 
	dbmanage.cpp
	data_server_db.cpp
	DBConnectionParams.cpp
	main.cpp
	)
set (dataServer_H
	DataServerResetAPI.h 
	dbmanage.h
	data_server_db.h
	DBConnectionParams.h
	../common/IRestServer.h
	)





add_executable(dataServer 
	${dataServer_SRC}
	${dataServer_H}
	)

add_dependencies(dataServer  
	common
	restbed
	)
target_link_libraries(dataServer PRIVATE
	-Wl,--start-group  	
	${CMAKE_SOURCE_DIR}/${3rd_part}/restbed/${CMAKE_BUILD_TYPE}/library/librestbed.so.4.7
	pthread
	jsoncpp 
	crypto
	ssl
	pq
	common
	curl
	#restbed
	pqxx 
	dl
	-Wl,--end-group
	)


