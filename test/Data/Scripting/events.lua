TestClass = class(Test)

function TestClass:on_init()
	print("On init")
end

function TestClass:on_uninit()
	print("On uninit")
end

function TestClass:event_listener(event)
	print("This is event listener: ")
end

tc = TestClass.new()

function event_listener(event)
	print("This is test: " .. tostring(event))
end

SimpleEvent = class(Event)
se = SimpleEvent.new({shared = true})

function SimpleEvent:get_name()
	return "SimpleEvent"
end

print("Event type: " .. se:get_event_type())
print("Event name: " .. tostring(se:get_name()))

-- register_event_listener(0x123, event_listener)
register_event_listener(se:get_event_type(), tc, tc.event_listener)

queue_event(se)

-- queue_event(tc)