# webserv
webserv is a 42's project that recreate a simple webserver using c++.

## Description

Use of HTML 1.1 protocols and heavily inspired by NGINX configuration and usages.

## Installation

Prerequisites:
- (working on MacOS, not tested on Linux and Windows)

```bash
git clone git@github.com:KayzaFlo/42_webserv.git
make
```

## Features

- Read HTML Request
- Generate & Send back HTML Response accordingly
- Handle main error codes (400, 404, 301, 201...)

### Config parameters (check default.conf)
```
server {
  listen [adress and port];
  server_name [name];
  error_page [error code] [error page to redir to];
  root [path to root);
  index [path to index if Request a file];
  client_max_body_size [define max request body size];
  autoindex [on/off];
  methods [allowed methods];
  post_path [upload path];

  location [path] {<br>
    [overwrite any parameters above]<br>
  }
}
```

## Usage

```
    ./webserv [config file]
```
Connect to port listened in config file with browser or desired testing tool (curl, telnet, postman...)

## License

[MIT](https://choosealicense.com/licenses/mit/)
