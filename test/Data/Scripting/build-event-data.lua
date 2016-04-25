-- Define a new ScriptEvent class
SomeEvent = class(ScriptEvent)

-- Create instance of the event
se = SomeEvent.new({shared = true, first_val = 1, second_val = "test"})

se:build(se)