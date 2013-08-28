sudo ps -eo comm,stat | grep "\bZ\b" | awk '{print $1}' | xargs -I {} pkill {}
