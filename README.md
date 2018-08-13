# prime-factors
This program determines if a number can be factored as exactly two primes, or itself is a prime. It is implemented based on Sieve of Eratosthenes.

## Getting Started

### Prerequisites

* GCC
* UNIX Shell OR Windows PowerShell

### Download source code and compile
The following instructions are presented using Bash in macOS:
```
# Change to HOME directory
$ cd ~

# Clone this repo and 'cd' into it
$ git clone https://github.com/jellycsc/prime-factors.git
$ cd prime-factors/

# Let's compile.
$ gcc -Wall -std=gnu99 -o pfact pfact.c 
```

### Usage
```
Usage: pfact n
```

### Example
```
$ ./pfact 15
[pid = 37241] {FILTER} m is 2
[pid = 37242] {FILTER} m is 3
15 3 5
Number of filters = 2
$ ./pfact 311
[pid = 37244] {FILTER} m is 2
[pid = 37245] {FILTER} m is 3
[pid = 37246] {FILTER} m is 5
[pid = 37247] {FILTER} m is 7
[pid = 37248] {FILTER} m is 11
[pid = 37249] {FILTER} m is 13
[pid = 37250] {FILTER} m is 17
311 is prime
Number of filters = 7
```

## Authors

| Name                    | GitHub                                     | Email
| ----------------------- | ------------------------------------------ | -------------------------
| Chenjie (Jack) Ni       | [jellycsc](https://github.com/jellycsc)    | nichenjie2013@gmail.com

## Thoughts and future improvements

* Details of the Sieve of Eratosthenes can be found [here](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes).

## Contributing to this project

1. Fork it [![GitHub forks](https://img.shields.io/github/forks/jellycsc/prime-factors.svg?style=social&label=Fork&maxAge=2592000)](https://github.com/jellycsc/prime-factors/fork)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to your feature branch (`git push origin my-new-feature`)
5. Create a new Pull Request

Details are described [here](https://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project).

## Bug Reporting [![GitHub issues](https://img.shields.io/github/issues/jellycsc/prime-factors.svg)](https://github.com/jellycsc/prime-factors/issues/)

Please click `issue` button above↑ to report any issues related to this project  
OR you can shoot an email to <nichenjie2013@gmail.com>

## License
This project is licensed under GNU General Public License v3.0 - see [LICENSE](LICENSE) file for more details.
