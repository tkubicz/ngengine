print("Second script")

print(test:func(10))

function TestClass:getSelf() 
	return self
end

local newInstance = TestClass

print(newInstance:func(20))

TestClass2 = createClass({frequency = 10, time = 20, counter = 0}, TestClass)

function TestClass2:_init()
	print("Frequency: " .. self.frequency .. ", time: " .. self.time .. ", counter: " .. self.counter)
end

function TestClass2:get_frequency()
	return self.frequency
end

local testClass2 = TestClass2()

print(testClass2:get_frequency(testClass2))

testTable = {1, 2, 3, 4, 5, 6, 7, 8}