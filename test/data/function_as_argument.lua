-- function_as_argument.lua

function add(a, b)
	return a + b
end

function pass_add(x, y)
	take_fun_arg(add, x, y)
end