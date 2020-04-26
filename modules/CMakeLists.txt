# Modules

set(modules_list 
	stringmod
	document_parser
)


foreach(target ${modules_list})
	add_subdirectory(${target})
	
	# Add target path
	set(sources)
	foreach(src_file ${TARGET_SRC})
		set(sources "${sources};${target}/${src_file}")
	endforeach()
	set(TARGET_SRC ${sources})
	set(headers)
	foreach(hdr_file ${TARGET_HEADERS})
		set(headers "${headers};${target}/${hdr_file}")
	endforeach()
	set(TARGET_HEADERS ${headers})
	
	
	add_library(${target} ${TARGET_SRC} ${TARGET_HEADERS})

	set_property (TARGET ${target} PROPERTY FOLDER ${MODULES_DIR})
	set_property(TARGET ${target} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target}/include>
	  $<INSTALL_INTERFACE:include>  # <prefix>/include
	  )

	target_include_directories(${target} PUBLIC
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${target}/include>
	  $<INSTALL_INTERFACE:include>  # <prefix>/include
	)

	# Property install headers
	set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${TARGET_HEADERS}")
	install(TARGETS ${target}
			CONFIGURATIONS Debug
			RUNTIME DESTINATION bin)
	install(TARGETS ${target}
			CONFIGURATIONS Release
			RUNTIME DESTINATION bin)
	install(TARGETS ${target}
			PUBLIC_HEADER DESTINATION include)

	set(LIBRARY_DEPS "${LIBRARY_DEPS};${target}")
	set(INCLUDE_DIRS "${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/${target}/include/")
endforeach()