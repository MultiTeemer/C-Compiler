require 'fileutils'
programm = ARGV[0]
place = "d:/Works/C++/Compiler/"
dirs = ["Tests/Lexer/", "Tests/Parser/", "Tests/Semantic/", "Tests/CodeGenerate/"]
keys = {dirs[1] => "-e", dirs[2] => "-table", dirs[3] => "-code"}
count, passed = 0, 0
tmpfiles = []
dirs.each do |dir|
	i = 1
	index = sprintf("%03d", i)
	while File.exists?(dir + "#{index}.in")
		count += 1
		filename = dir + "#{index}.in"
		res = 
			case keys[dir]
				when nil then %x["#{programm}", "#{filename}"]
				when "-e", "-table" then %x["#{programm}", "#{keys[dir]}" "#{filename}"]
				when "-code" then 
					%x["#{programm}", "#{keys[dir]}" "#{filename}"]
					mainDir = 'd:/works/c++/compiler/'
					path = mainDir + filename + '.asm'
					%x['ml', "#{path}"]
					tmpfiles << path
					path = mainDir + "#{index}.in" + '.obj'
					%x['c:/masm32/bin/link.exe', "/subsystem:console" "#{path}"]
					path[/(obj)$/] = 'exe'
					tmp = %x["#{path}"]
					tmpfiles << path
					tmpfiles << path.sub(/(exe)$/, 'obj')
					tmp
			end
		#File.open(dir + "#{index}.out", "w"){|f| f.write res}
		output = res.split(/\n/)
		correct = IO.readlines(dir + "#{index}.out").map(&:chomp)
		if (output - correct).empty?
			passed += 1
			#puts "Test #{filename} passed"
		else
			puts "Test #{filename} failed"
		end
		i += 1
		index = sprintf("%03d", i)
	end
	puts "+" + "-" * 25 + "+"
end
tmpfiles.each {|file| File::delete(file) }
puts "Result: #{passed}/#{count}"