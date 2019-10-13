# ls -F | nl
    The piping_commands code works specifically for ls -F | nl. 
    A parent process creates a pipe and forks two children process. 
    One child works on ls -F and writes the output into the pipe
    The other child read the output from the pipe and runs command nl on it
    The output should match the actual command. The output also shows child ids that
    finished before the parent did to ensure no processes becomes orphans.
