TestClass = class(Test)

function TestClass:on_init()
	print("On init")
end

function TestClass:on_uninit()
	print("On uninit")
end

function TestClass:event_listener(event)
	print("This is object event listener: " .. event:get_event_type())
end

function TestClass:mouse_event_listener(event)
	print("This is mouse event listener: " .. event:get_event_type())
end

tc = TestClass.new()

function event_listener(event)
	print("This is standalone event listener: " .. event:get_event_type())
end

SimpleEvent = class(ScriptEvent)
se = SimpleEvent.new({shared = true})

function SimpleEvent:get_name()
	return "SimpleEvent"
end

print("Event type: " .. se:get_event_type())
-- print("Event name: " .. tostring(se:get_name()))

register_event_listener(se:get_event_type(), event_listener)
register_event_listener(se:get_event_type(), tc, tc.event_listener)
register_event_listener(EventType.MouseEvent, tc, tc.mouse_event_listener)

queue_event(se)

-- queue_event(tc)