sudo service rpcbind start
chmod +x run_client1.sh run_client2.sh;

# List of .sh files to run
sh_files=("run_client1.sh" "run_client2.sh")

# Iterate over the list and run each .sh file in the background
for file in "${sh_files[@]}"; do
    if [ -f "$file" ]; then
        echo "Running $file in the background..."
        chmod +x "$file"  # Ensure the file is executable
        "./$file" &      # Execute the .sh file in the background
    else
        echo "File $file not found or is not executable."
    fi
done

# Optionally, wait for all background processes to finish
wait
