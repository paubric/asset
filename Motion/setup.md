# Install motion:
``` sudo apt-get install motion ```

# Creating motion files:
``` 
mkdir ~/.motion
nano ~/.motion/motion.conf 
```

# Setting up motion:
Write in the motion file the following

``` 
stream_port 8081
stream_localhost on 
output_pictures off 
```

# Run motion and connect to the stream:
Run ``` motion ``` in the terminal and now connect to 127.0.0.1:8081 and the video should be there!
