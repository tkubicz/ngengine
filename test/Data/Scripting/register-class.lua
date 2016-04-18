success = false

TestProcess = class(ScriptProcess)

function TestProcess:on_init()
	-- print("init function")
end

function TestProcess:on_update(time)
	self.accumulatedTime = self.accumulatedTime + time
	-- print(self.accumulatedTime)
	if self.accumulatedTime >= 250 then
		self:succeed()
	end
end

function TestProcess:custom_test_function()
	-- print("this is custom function")
	success = true
end

function TestProcess:on_success()
	self:custom_test_function()
end

tp = TestProcess.new({shared = true, frequency = 50, accumulatedTime = 0})
tp:build_data(tp)