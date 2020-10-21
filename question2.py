import re
import sys

if len(sys.argv) < 2:
    sys.exit(-1)

n = int(sys.argv[1])

r = "^(a?){" + str(n) + "}a{" + str(n) + "}$"
print("regex: '{}'".format(r))

while True:
    s = sys.stdin.readline().strip()
    if re.match("^" + r + "$", s):
        print("contient '{}': oui".format(s))
    else:
        print("contient '{}': non".format(s))
