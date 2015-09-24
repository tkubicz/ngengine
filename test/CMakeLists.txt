cmake_minimum_required (VERSION 2.8)

if(${BUILD_TESTS})

	set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
	set (EXTRA_LIBS ${EXTRA_LIBS} nge)
    set (EXTRA_LIBS ${EXTRA_LIBS} GL)
    set (EXTRA_LIBS ${EXTRA_LIBS} GLU)
    set (EXTRA_LIBS ${EXTRA_LIBS} GLEW)
    set (EXTRA_LIBS ${EXTRA_LIBS} glfw)
    set (EXTRA_LIBS ${EXTRA_LIBS} z)
	set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
	set (EXTRA_LIBS ${EXTRA_LIBS} lua${LUA_VERSION})

	add_executable(ProcessTest test/ProcessTest.cpp)
	target_link_libraries(ProcessTest ${EXTRA_LIBS})

	add_executable(DelegateTest test/DelegateTest.cpp)
	target_link_libraries(DelegateTest ${EXTRA_LIBS})

	add_executable(ConcurrentQueueTest test/ConcurrentQueueTest.cpp)
	target_link_libraries(ConcurrentQueueTest ${EXTRA_LIBS})

	add_executable(EventManagerTest test/EventManagerTest.cpp)
	target_link_libraries(EventManagerTest ${EXTRA_LIBS})

	add_executable(SeleneTest test/SeleneTest.cpp)
	target_link_libraries(SeleneTest ${EXTRA_LIBS})

	add_executable(LuaScriptManagerTest test/LuaScriptManagerTest.cpp)
	target_link_libraries(LuaScriptManagerTest ${EXTRA_LIBS})

	add_executable(LuaScriptProcessTest test/LuaScriptProcessTest.cpp)
	target_link_libraries(LuaScriptProcessTest ${EXTRA_LIBS})

	add_executable(LuaScriptEventTest test/Scripting/LuaScriptEventTest.cpp)
	target_link_libraries(LuaScriptEventTest ${EXTRA_LIBS})

	add_executable(LuaScriptExportsTest test/Scripting/LuaScriptExportsTest.cpp)
	target_link_libraries(LuaScriptExportsTest ${EXTRA_LIBS})

	enable_testing()

	add_test(ProcessTestRunner			ProcessTest)
	add_test(DelegateTestRunner			DelegateTest)
	add_test(ConcurrentQueueTestRunner	ConcurrentQueueTest)
	add_test(EventManagerTestRunner		EventManagerTest)
	add_test(SeleneTestRunner			SeleneTest)
	add_test(LuaScriptManagerRunner		LuaScriptManagerTest)
	add_test(LuaScriptProcessRunner		LuaScriptProcessTest)
	add_test(LuaScriptEventRunner		LuaScriptEventTest)
	add_test(LuaScriptExportsRunner		LuaScriptExportsTest)

endif()