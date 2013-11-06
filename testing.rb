require 'fileutils'
programm = ARGV[0]
dirs = ["Tests/Lexer/", "Tests/Parser/", "Tests/Semantic/"]
keys = {dirs[1] => "-e", dirs[2] => "-table"}
count, passed = 0, 0
dirs.each do |dir|
	i = 1
	index = sprintf("%03d", i)
	while File.exists?(dir + "#{index}.in")
		count += 1
		filename = dir + "#{index}.in"
		if keys[dir]
			res = %x["#{programm}", "#{keys[dir]}" "#{filename}"]
		else
			res = %x["#{programm}", "#{filename}"]
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
puts "Result: #{passed}/#{count}"