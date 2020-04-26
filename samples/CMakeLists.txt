# Samples
set(app_list 
	practice1_integrals
	practice2_string_replace
	practice3_stringmod
	practice5_displaytxt
	practice6_order_vector
	practice7_quick_sort
)

foreach(app_filename ${app_list})
	get_filename_component(${app_filename} ${CMAKE_CURRENT_SOURCE_DIR}/${app_filename}.cpp NAME_WE)
	add_executable(${app_filename} ${CMAKE_CURRENT_SOURCE_DIR}/${app_filename}.cpp)
	set_property (TARGET ${app_filename} PROPERTY FOLDER ${SAMPLES_DIR})
	target_include_directories(${app_filename} PUBLIC
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	  $<INSTALL_INTERFACE:include>  # <prefix>/include
	)
endforeach()

# Link LIBs
IF (TARGET practice3_stringmod)
	target_link_libraries(practice3_stringmod stringmod)
ENDIF()

IF (TARGET practice5_displaytxt)
	target_link_libraries(practice5_displaytxt document_parser)
ENDIF()

IF (TARGET practice6_order_vector)
	target_include_directories(practice6_order_vector PUBLIC ${CMAKE_SOURCE_DIR}/${MODULES_DIR}/order_vector/include)
ENDIF()

add_subdirectory(qt_practice1_draw)
set_property (TARGET qt_practice1_draw PROPERTY FOLDER ${QT_SAMPLES_DIR})
add_subdirectory(qt_practice2_mvc)
set_property (TARGET qt_practice2_mvc PROPERTY FOLDER ${QT_SAMPLES_DIR})
add_subdirectory(qt_practice3_thread)
set_property (TARGET qt_practice3_thread PROPERTY FOLDER ${QT_SAMPLES_DIR})