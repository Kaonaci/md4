# MD4
MD4 implementation in Python
## Purpose
Demonstrate how the md4 hash function works and provide a sample md4 hash code.
## Installation
Clone the repository, and you're done. You can use it as a module or utility in shell.
## Usage
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
## Examples
```shell
python ./md4.py "hello world"

>>> aa010fbc1d14c795d86ef98c95479d17
```
Or, as a module in your code:
```python
from md4 import md4

print(md4("hello world"))
# aa010fbc1d14c795d86ef98c95479d17
```
