-- Define event
EventFromScript = class(ScriptEvent)

function EventFromScript:get_event_type()
	return 0x123456
end

-- Create instance of the event
efs = EventFromScript.new({shared = true, value = 10})

-- Queue event
queue_event(efs)
