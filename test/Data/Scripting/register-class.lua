TestProcess = class(ScriptProcess)

function TestProcess:on_init()
	print("init function")
end

function TestProcess:on_update(time)
	print("update function, time = " .. tostring(time))
end

function TestProcess:custom_test_function()
	print("this is custom function")
end

function TestProcess:on_success()
	self:custom_test_function()
end

tp = TestProcess.new({frequency = 125})

print("is dead: " .. tostring(tp:is_dead()))
print("is alive: " .. tostring(tp:is_alive()))
print("is paused: " .. tostring(tp:is_paused()))

-- tp:succeed()
-- tp:fail()
-- tp:pause()
-- tp:unpause()

tp:build_data(tp)