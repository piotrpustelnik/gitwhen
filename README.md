# gitwhen - get logs from all repositories

[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![C](https://img.shields.io/badge/Language-C-brightgreen)

## Description

`gitwhen` is a command-line tool written in C that allows you to gather all logs from your local Git repositories between specified dates. It simplifies the process of analyzing and collating commit history across repositories for a given time frame.

## Features

- **Programming Language:** C
- **Purpose:** Collect logs from local Git repositories within date ranges
- **License:** MIT License

## Getting Started

### Prerequisites

- A C compiler (e.g., GCC)
- Git

### Installation

From source:

1. Clone the repository:

   ```bash
   git clone https://github.com/piotrpustelnik/gitwhen.git
   cd gitwhen
   ```

2. Build the project (compiles to single binary):

   ```bash
   make
   ```

3. Run the application:

   ```bash
   ./build/gitwhen
   ```


## Usage

List commits from all your repositories in current working directory between dates:
`gitwhen 2025-04-20 2025-04-25`

List commits from all your repositories in current working directory on particular day:
`gitwhen 2025-04-20` 

## Contributing

Contributions are welcome! Please open an issue or submit a pull request to contribute.

## License

This project is licensed under the [MIT License](LICENSE).

## Contact

Find me on GitHub: [piotrpustelnik](https://github.com/piotrpustelnik)