-- registering_class_variables.lua

bar = Bar.new(4)

print(bar:x()) -- will print '4'

bar::set_x(-4)

print(bar:x()) -- will print '-4'