## Purpose
Demonstrate how the md4 hash function works and provide a sample md4 hash code.
## Installation
### Python
Clone the repository, and you're done. You can use it as a module or utility in shell.
```shell
git clone https://github.com/Kaonaci/md4
```
### C
**Compile using Makefile or manually:**
Using Makefile:
```shell
Available targets:
  all     - Build release version (default)
  debug   - Build debug version
  release - Build optimized release version
  profile - Build version for profiling
  test    - Run basic tests
  clean   - Remove compiled files
  help    - Show this help
```
Manual compilation:
```shell
gcc -o md4 md4.c
```
## Usage
### Python
As a command-line utility:
```shell
python ./md4.py [-h] message [message ...]
```
Or, if your system supports shebang:
```bash
md4.py [-h] message [message ...]
```
Or, as a module in your code:
```python
from md4 import md4

# YOUR CODE
```
### C
As a command-line utility:
```shell
./md4 [-h] message [message ...]
```
## Examples
### Python
```shell
python ./md4.py "hello world"
# Output: aa010fbc1d14c795d86ef98c95479d17
```
Or, as a module in your code:
```python
from md4 import md4

print(md4("hello world"))
# aa010fbc1d14c795d86ef98c95479d17
```
### C
```shell
./md4 "hello world"
# Output: aa010fbc1d14c795d86ef98c95479d17
```