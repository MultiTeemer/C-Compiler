Dir["D:/Works/C++/Compilator/Tests/Parser/*"].each do |filename|
	patches = filename.split('/').last.split('.')
	name = patches[0]
	if name.length == 1
		name = "00" + name
	elsif name.length == 2
		name = "0" + name
	else
		name = name
	end
	File.rename(filename, "D:/Works/C++/Compilator/Tests/Parser/" + name + '.' + patches.last)
end