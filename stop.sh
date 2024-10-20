kill $(ps aux | grep '\./main' | awk '{print $2}')
