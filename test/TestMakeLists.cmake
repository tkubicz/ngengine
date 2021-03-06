cmake_minimum_required (VERSION 3.0)

if(${BUILD_TESTS})

	set (EXTRA_LIBS 
		 nge GL GLU GLEW glfw z pthread lua${LUA_VERSION} cppformat
	)

	if (NGE_USE_IMAGE_PNG)
		set (EXTRA_LIBS ${EXTRA_LIBS} png)
	endif()

	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tests)

	set(source
		test/Actors/ActorTest.cpp
		test/Core/ConcurrentQueueTest.cpp
        test/Core/DelegateTest.cpp
		test/Core/ProcessManagerTest.cpp
		test/Core/ProcessTest.cpp
		test/Events/EventManagerTest.cpp
		test/Media/Images/TextureManagerTest.cpp
		test/Media/MediaManagerTest.cpp
		test/Parsers/StringUtilsTest.cpp
		test/Scripting/LuaScriptEventListenerTest.cpp
		test/Scripting/LuaScriptEventTest.cpp
		test/Scripting/LuaScriptManagerTest.cpp
		test/Scripting/LuaScriptProcessTest.cpp
        test/Thirdparty/CatchTest.cpp
		test/Tools/NewLoggerTest.cpp
		test/Tools/TimingTest.cpp
		test/TestSettings.cpp
		test/Main.cpp
	)
	
	set(TEST_EXECUTABLE_NAME tests)
	set(TEST_ASSET_DIR ${PROJECT_SOURCE_DIR}/test)

	configure_file(
		test/TestSettings.hpp.in
		${PROJECT_BINARY_DIR}/include/TestSettings.hpp
	)

	include_directories("${PROJECT_BINARY_DIR}/include")

	add_executable(${TEST_EXECUTABLE_NAME} ${source})
	target_link_libraries(${TEST_EXECUTABLE_NAME} ${EXTRA_LIBS})

	if (${SHOW_SOURCE_CODE_STATS})
		find_program(CLOC cloc)
		if (NOT CLOC)
			message(STATUS "Could not find cloc")
		else()
			message(STATUS "Found cloc: ${CLOC}")
			add_test(NAME NgeSourceCodeStats COMMAND ${CLOC} ${PROJECT_SOURCE_DIR}/source)
			add_test(NAME TestSourceCodeStats COMMAND ${CLOC} ${PROJECT_SOURCE_DIR}/test)
			add_test(NAME SummarySourceCodeStats COMMAND ${CLOC} ${PROJECT_SOURCE_DIR}/source ${PROJECT_SOURCE_DIR}/test)
		endif()
	endif()

	add_test(RunTests ${EXECUTABLE_OUTPUT_PATH}/${TEST_EXECUTABLE_NAME})

	add_test(NAME ListTests COMMAND ${EXECUTABLE_OUTPUT_PATH}/${TEST_EXECUTABLE_NAME} --list-tests)
	set_tests_properties(ListTests PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+ test cases")

	add_test(NAME ListTags COMMAND ${EXECUTABLE_OUTPUT_PATH}/${TEST_EXECUTABLE_NAME} --list-tags)
	set_tests_properties(ListTags PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+ tags")

	enable_testing()

endif()