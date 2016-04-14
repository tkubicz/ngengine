TestProcess = class()

function TestProcess:new()
	print("constructor")
end

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

tp = TestProcess.new({shared = true, frequency = 50})
tp:build_data(tp)