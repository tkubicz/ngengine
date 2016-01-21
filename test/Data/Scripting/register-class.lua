TestProcess = class(ScriptProcess)

function TestProcess:on_init()
	print("init function")
end

function TestProcess:on_update(time)
	print("Update function, time = " .. tostring(time))
end

tp = TestProcess.new({frequency = 125})

print("is dead: " .. tostring(tp:is_dead()))
print("is alive: " .. tostring(tp:is_alive()))
print("is paused: " .. tostring(tp:is_paused()))

tp:succeed()
tp:fail()
tp:pause()
tp:unpause()

child = {}
tp:attach_child(child)

dp = TestProcess.new({frequency = 250})
print(tp.frequency)
print(dp.frequency)

tp:on_init()