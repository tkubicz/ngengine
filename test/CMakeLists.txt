cmake_minimum_required (VERSION 3.0)

if(${BUILD_TESTS})

	set (extra_libs 
		${Boost_LIBRARIES} nge GL GLU GLEW glfw z pthread lua${LUA_VERSION}
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
		
		# Scripting
        LuaScriptEventTest   Scripting/LuaScriptEventTest.cpp
		LuaScriptManagerTest Scripting/LuaScriptManagerTest.cpp
		LuaScriptProcessTest Scripting/LuaScriptProcessTest.cpp
	)

	array2d_begin_loop(advanced "${tuples}" 2 "name;path")
	while (advanced)
		add_executable(${name} test/${path})
		target_link_libraries(${name} ${extra_libs})
		add_test(${name}Runner ${EXECUTABLE_OUTPUT_PATH}/${name})
		array2d_advance()
	endwhile()

	enable_testing()

endif()