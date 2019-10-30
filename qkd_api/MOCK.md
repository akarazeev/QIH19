# Mock QKD API

This is a mock implementation of the API which can be expanded to fit all your
needs. This implementation does nothing quantum.

Limitations of the mock:
- it is only suitable for local testing - both end-points must have access to
  the same `localhost` interface,
- can only run one session at a time on the same `key_handle`,
- `node_a.c` and `node_b.c` are only example applications.

The first two restrictions are a result of how the mock decides who is server,
who is the client. The first end-point to bind to the local port becomes the
server. If the other node finds that the port is unavailable it takes on a
client role. This was done so that you don't have to compile a separate server
and client mock binary.

However, the mock is implemented using network sockets so some of these
limitations can be easily overcome with minimal work if needed for testing
purposes.

### Getting started

Download the repo and compile the example:
```bash
git clone https://github.com/akarazeev/QIH19
cd QIH19/qkd_api/example
mkdir bin/
gcc node_a.c -I ../ ../qkd_api.c -o bin/a
gcc node_b.c -I ../ ../qkd_api.c -o bin/b
```

Execute `./bin/a`. Now the application is blocked on `QKD_GET_KEY` - it is
acting as a server and waits for another node to connect.

Open another terminal session in the same directory and execute `./bin/b`. The
application connects to the other node and requests the `key` to be
"generated".

