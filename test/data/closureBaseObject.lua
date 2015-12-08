function MyClass(init)
	local self = {
		public_field = 0
	}

	local private_field = init

	function self.foo()
		return self.public_field + private_field
	end

	function self.bar()
		private_field = private_field + 1
	end

	return self
end

test = MyClass(5)
print(test.foo())
test.public_field = 3
test.bar()
print(test.foo())

MyClass.foo = function() return 1 end

print(test.foo())

Process.registerFunction("")