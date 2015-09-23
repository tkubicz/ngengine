function createClass(body, ...)
	-- "cls" is the new class
	local cls, bases = body or {}, {...}
	-- copy base class contents into the new class
	for i, base in ipairs(bases) do
		for k, v in pairs(base) do
			cls[k] = v
		end
	end
	-- set the class's __index, and start filling an "is_a" table that contains this class and all of its
	-- bases so you can do an "instance of" check using my_instance.is_a[MyClass]
	cls.__index, cls.is_a = cls, {[cls] = true }
	for i, base in ipairs(bases) do
		for c in pairs(base.is_a) do
			cls.is_a[c] = true
		end
		cls.is_a[base] = true
	end
	-- the class's __call metamethod
	setmetatable(cls, {__call = function (c, ...)
		local instance = setmetatable({}, c)
		-- run the init method if it's there
		local init = instance._init
		if init then init(instance, ...) end
		return instance
		end,
	})
	-- return the new class table, that's ready to fill with methods
	return cls
end

BaseClass = createClass()
function BaseClass:_init(val) self.value = val end
function BaseClass:get_value() return self.value end
base = BaseClass(5)

AnotherClass = createClass({frequency = 1000, time = 10}, BaseClass)

function AnotherClass:_init(frequency, time) self.frequency, self.time = frequency, time end
function AnotherClass:get_frequency() return self.frequency end

local another = AnotherClass(25, 20)
print(another:get_frequency())

local another2 = AnotherClass(50, 55)
print(another2:get_frequency())

YetAnotherClass = createClass(nil,BaseClass)
local yetAnother = YetAnotherClass