ListenerClass = class()

function ListenerClass:listener(event)
	print("Event received: ")
end

lc = ListenerClass.new()

register_event_listener(0x123456, ListenerClass.listener)
