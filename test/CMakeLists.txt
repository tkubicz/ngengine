cmake_minimum_required (VERSION 3.0)

if(${BUILD_TESTS})

	set (extra_libs 
		${Boost_LIBRARIES} nge GL GLU GLEW glfw z pthread lua${LUA_VERSION} cppformat
	)

	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/tests)

	include(array2d)

	set(tuples
		# Core 
		ConcurrentQueueTest Core/ConcurrentQueueTest.cpp
		DelegateTest		Core/DelegateTest.cpp
		ProcessTest			Core/ProcessTest.cpp
		ProcessManagerTest	Core/ProcessManagerTest.cpp

		# Events
		EventDelegateTest	Events/EventDelegateTest.cpp
		EventManagerTest	Events/EventManagerTest.cpp
		
		# Parsers
		StringUtilsTest		Parsers/StringUtilsTest.cpp

		# Scripting
        LuaScriptEventTest   Scripting/LuaScriptEventTest.cpp
		LuaScriptManagerTest Scripting/LuaScriptManagerTest.cpp
		LuaScriptProcessTest Scripting/LuaScriptProcessTest.cpp

        # Thirdparty
		CatchTest			Thirdparty/CatchTest.cpp
        CppFormatTest       Thirdparty/CppFormatTest.cpp

		# Tools
		LoggerTest			Tools/LoggerTest.cpp
		#NewLoggerTest		Tools/NewLoggerTest.cpp
		#TimingTest			Tools/TimingTest.cpp
	)

	array2d_begin_loop(advanced "${tuples}" 2 "name;path")
	while (advanced)
		add_executable(${name} test/${path})
		target_link_libraries(${name} ${extra_libs})
		add_test(${name}Runner ${EXECUTABLE_OUTPUT_PATH}/${name})
		array2d_advance()
	endwhile()

	set(source
		test/Tools/NewLoggerTest.cpp
		test/Tools/TimingTest.cpp
		test/Tools/ToolsTest.cpp
	)
	add_executable(ToolsTest ${source})
	target_link_libraries(ToolsTest ${extra_libs})

	add_test(ToolsTestRunner ${EXECUTABLE_OUTPUT_PATH}/ToolsTest)

	add_test(NAME ListTests COMMAND ${EXECUTABLE_OUTPUT_PATH}/ToolsTest --list-tests)
	set_tests_properties(ListTests PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+ test cases")

	add_test(NAME ListTags COMMAND ${EXECUTABLE_OUTPUT_PATH}/ToolsTest --list-tags)
	set_tests_properties(ListTags PROPERTIES PASS_REGULAR_EXPRESSION "[0-9]+ tags")

	enable_testing()

endif()