

## Application
i
- User starts pomodoro timer for 25 minutes
- The program will start a timer and show the user the end time
- The program will load a list of websites it wants to block from the environmental variables or a config file
- The programm will start a proxy server and block any requests for websites the user doesn't want to visit
- Once the timer is finished, the proxy server will shut down and allow the user to visit those websites again

## Development 

Pomodoro Timer
- Simple timer with a 25 min countdown and 5 min breaks if the user likes

Load Blocked Websites
- Read and parse a list of websites from a .env or text file

Proxy Server
- Initialize a server to listen to HTTP/HTTPS ports
- Intercept and inspect each web request

Website Blocking
- Enhance the proxy server to block requests to specified websites
    - Check if the URL is in the blocked list
    - Deny access if not on the list; show a dummy value

Integrate with Pomodoro Timer
- Link the proxy operation with the timer; start or stop based on the timing

## TO-DO's
- [X] Build a pomodoro timer with a 25 min countdown
- [] Parse a text file and print it to the console
- [] Create a proxy server to listen to HTTP requests
- [] Enhance the server to handle HTTPs requests
- [] Test the proxy server with dummy input
- [] Inspect the server requests and print them to the console
- [] Block a dummy website from the requests to the server
- [] Integrate it with the pomodoro timer
