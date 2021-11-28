
artworks = []


Dir.glob("../src/**/abouticon.txt").each{|about|
  text = File.read(about)
  r = text.scan(/^[Aa]uthor:?[ \t]*(.*)/)
  if (r.size==0)
    raise "No Author mentioned in #{about}, please correct\n  It must contain a line beginning with Author, like\nAuthor Hans Muster"
  end
  r.flatten!
  # p r
  artworks+=r
}
artworks.sort!.uniq!
p artworks

sources = []
Dir.glob("../src/**/*.{cpp,h}").each{|source|
  text = File.read(source)
  r = text.scan(/\@author[ \t]+(.*)/)
  if (r.size>0)
    r.flatten!
    sources+=r
  else
    puts "No author in #{source}"
  end
}
sources.sort!.uniq!
p sources

puts "Artwork authors:"
puts artworks
puts "\nSource code authors:"
puts sources

inc = "const char * coders = \"#{sources.join('\n')}\";\n"
inc += "const char * artists = \"#{artworks.join('\n')}\";\n"

File.write("../src/apps/systemapps/credits/credits.txt", inc)
