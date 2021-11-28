while line=gets
  line.scan(/0x[0-9a-f]+:0x[0-9a-f]+/).each{|addr|
    #p addr
    cmd = "~/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-addr2line -pfiaC -e ../.pio/build/ttgo-t-watch/firmware.elf #{addr}"
    res = `#{cmd}`#.gsub(/^.*?: (.*?) ..\/IvosWatch\//, "$1 ../")
#    0x400fa17a: scrl_reposition at /home/ivo/burggraben/informatik/labor/arduino/t-watch/IvosWatch/.pio/libdeps/ttgo-t-watch/TTGO TWatch Library/src/lvgl/src/lv_widgets/lv_page.c:1172
    puts res
    res = res.scan(/^(0x[0-9a-f]+): (.*?) at (.*):([0-9]+)$/)
    if res.size>0
      addr,func,file,line = *(res[0]) 
      file.gsub!(/^.*IvosWatch/, "..")
      puts "less -N +#{line} '#{file}'"
    end
  }
end

