

def collect()
  dirs = %w(America Indian Antarctica Australia Asia Europe Africa Atlantic Pacific Arctic).sort.each{|d|
    puts File.basename(d)
    Dir.glob("/usr/share/zoneinfo/#{d}/**/*").sort.each{|f|
      if File.file?(f) 
        city = File.basename(f)
        tzstring = `tail -n 1 "#{f}"`
        puts " #{city}\n  #{tzstring}"
      end
    }
  }
end

collect()

