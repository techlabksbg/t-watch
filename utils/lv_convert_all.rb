

Dir.glob("../src/**/*.png").each{|png|
  # puts png
  c = png.gsub(/.png$/,".c")
  if File.exist?(c)
    # puts " -> #{c}"
    if File.mtime(png)>File.mtime(c)
      # puts "   ---> This one ^^"
      cmd = "bash lv_img_conv.bash \"#{png}\""
      puts cmd
      puts `#{cmd}`
    end
  end
}

