[gawk] extensions to ease some actions, so you can focus in writing much shorter and nested code.

They are written to be used in the GNU version of [awk]

Listing all of the functions in no particular order.

* [arrjoin(" ", "keys", arr)](#arrjoin)
* [arrfirst("key", arr)](#arrfirst)
* [arrlast("key", arr)](#arrlast)
* [round(-7.51)](#round)
* [numsonly("Hello 3.e14 World")](#numsonly)
* [tobase(2, 123)](#tobase)
* [frombase(2, "01111011")](#frombase)
* [repeat("-", 20)](#repeat)

[Grey area](#grey_area):

* [basename("/hello/world")](#basename)
* [dirname("/hello/world")](#dirname)
* [digest functions](#digest_functions)
* [many mathematical funcs](#many)

---

## arrjoin

arrjoin is advanced version of [gawks join].

**arrjoin** takes 4 arguments of which 3 are mandatory.

arg1 mandatory is separator to be used.

arg2 mandatory is string to request the joining of **keys**/**values**/**kv**, kv stands for both.

arg3 mandatory is the array itself.

arg4 non mandatory is string used as pattern for excluding keys and values.

You can even use it to merge multiple arrays into singular one.

```
@load "arraysfuncs";
{
  x["hello"]++;
  x["world"]++;

  // hello world
  printf("%s\n", arrjoin(" ", "keys", x));

  // 1 1
  printf("%s\n", arrjoin(" ", "values", x));

  // all keys and values
  printf("%s\n", arrjoin(" ","kv", x));

  // exclude all "foo" keys/values when found
  printf("%s\n", arrjoin(" ", "kv", x, "foo"));

  // python's equivalent of
  // dict to list conversion
  split(arrjoin(" ", "kv", x), arr2, " ");

  // now we can access arr2 in numeric way
  printf("%s\n",arr2[3]);

  // merging multiple arrays into one larger
  // named intoVar
  a1 = arrjoin(" ", "keys", a);
  b1 = arrjoin(" ", "values", b);
  intoVar = a1" "b1;
  printf("%s\n", intoVar);
}
```

Arrays in gawk are associative, which makes them harder to be indexed in numeric way, since the left and right operand (key/value) can be of string and/or numeric type. The "dict to list" conversion is really handy when you don't know what the keys/values in given array are, and you would like to index the array in numeric way.

```python
# python
a=["hello","world"]  # list
b=("hello", "world") # tuple
c={"hello":"world","123":456} # dict

# KeyError
print("{0}".format(c["nonExisting"]))

# dict converted into a sorted list of tuples
# [('123', 456), ('hello', 'world')]
d=[(x,z) for x,z in c.items()]

# ('hello', 'world') 456
print("{0} {1}".format(d[1],  d[0][1]))
```

---

## arrfirst

arrfirst("key", x) will return the first **key**/**value**/**kv** in the array x.

**arrfirst** takes 2 arguments, first one must be string in the form of **keys**/**values**/**kv**, kv stands for both, second one is the array.

You can even use it in conjunction with [arrjoin()] to join all array keys/values/kv except the first one.

```
@load "arraysfuncs";
{
  x[0]="hello";
  x[1]="world";
  x[2]="hi";

  // "hello"
  printf("%s\n", arrfirst("value", x));
}
```

---

## arrlast

arrlast("key", x) will return the last **key**/**value**/**kv** in the array x.

**arrlast** takes 2 arguments, first one must be string in the form of **key**/**value**/**kv**, kv stands for both, second one is the array.

You can even use it in conjunction with [arrjoin()] to join all array keys/values/kv except the last one.

```
@load "arraysfuncs";
{
  x[0]="hello";
  x[1]="world";
  x[2]="hi";

  // "hi"
  printf("%s\n", arrlast("value", x));
}
```

---

## round

round(-7.51) will return rounded float to the nearest given number, so 10.5 will be rounded to 11.0

The function tries to comply with the [IEEE-754] [standard] which [gawk2] uses.

**round** takes 1 argument, it must be of numeric type. I do advise you to use it for floating numbers, and not for integers, what's to be rounded in **2** ?

```
@load "numfuncs";
{
  printf("%g\n", round(7.6));
  printf("%g\n", round(-7323121.58732131312));
}
```

---

## numsonly

numsonly("Hello 3.e14 World") will extract all numbers from given string, and return them as space separated string back to you.

**numsonly** takes 2 arguments of which 1 is mandatory. 

arg1 mandatory is the string that will be parsed.

arg2 non mandatory is separator to be used to split the string into tokens. By default it's one single **" "** space.

```
@load "numfuncs";
{
  x = "3.e14 456 world 789 lklwlqkklewqkk 0xDeadBeef eklqwkewkqeqwekjjvfs";
  z = "3.e14|456|world|789|lklwlqkklewqkk|0xDeadBeef|eklqwkewkqeqwekjjvfs";

  // 3e+14 456 789 0xDeadBeef
  printf("%s\n", numsonly(x));
  printf("%s\n", numsonly(z, "|"));
}
```

---

## tobase

tobase(2, 123) will convert the given number in base 2 and return it as string, since it can be quiet long.

**tobase** takes 2 arguments which has to be of numeric type. The first argument is the requested base that you would like to use, it should be in the range of 2 - 36. The second argument is the number that will be converted.

```
@load "numfuncs";
{
  // 1111011
  printf("%s\n", tobase(2, 123));
}
```

---

## frombase

The opposite action of [tobase()].

**frombase** takes 2 arguments. The first argument is the base that argument 2 was converted into, the base number should be in the range of 2 - 36. The second argument is the number that will be converted back, it should be of string type.

```
@load "numfuncs";
{
  // 123
  printf("%g\n", frombase(2, "1111011"));
}
```

---

## repeat

repeat("-", 20) will repeat given string certain times.

**repeat** takes 2 arguments. The first argument is the string to be repeated. The second argument is the number that arg1 will be repeated, the number should in the range of 1 - 2000.

```
@load "general";
{
  // ---------------------
  printf("%s\n", repeat("-", 20));

  // using gawk only
  x=0;z="-";while(20>x){z=z"-";x++;}
}
```

---

## Grey_area

The following functions happen to be in what I call grey area. They will be mostly useful when working with external data, like one coming from [getline].

---

## basename

basename("/hello/world") will return the basename of given string.

**basename** takes 1 argument which has to be of string type.

```
@load "general";
{
  // world
  printf("%s\n", basename("/hello/world"));
}
```

---

## dirname

The opposite action of [basename()].

**dirname** takes 1 argument which has to be of string type.

```
@load "general";
{
  // /hello
  printf("%s\n", dirname("/hello/world"));
}
```

---

## digest_functions

Compute the checksum for given string.

The digest functions in no particular order:

```
sha1
sha224
sha256
sha384
sha512
md5
md4
mdc2
ripemd160
whirlpool
```

Each of the above functions take 1 argument which has to be of string type.

```
@load "general";
{
  // 59e1748777448c69de6b800d7a33bbfb9ff1b463e44354c3553bcdb9c666fa90125a3c79f90397bdf5f6a13de828684f
  printf("%s\n", sha384("hello"));
}
```

---

## many

Tried to add as many of the following [mathematical] functions into gawk. Go ahead visit the link and read the `man` page according to each function that you'd like to use.

All functions take 1 numeric argument except the following which take 2:

cpow, hypot, remainder, fmin, fmax, fdim, fmod, copysign

Listing the rest functions that take 1 argument:

```
ceil
floor
trunc
abs
cabs
fabs
logb
clog
cimag
creal
cexp
cbrt
log2
log10
exp2
expm1
tan
asin
acos
atan
sinh
cosh
tanh
erf
erfc
lgamma
tgamma
j0
j1
y0
y1
```

Grey area end.

---

## Installation

```bash
export AWKLIBPATH=$AWKLIBPATH:/tmp/lib/gawk
autoreconf -if
./configure --prefix=/tmp
make
make install
```


[getline]: https://www.gnu.org/software/gawk/manual/html_node/Getline.html
[gawks join]: https://www.gnu.org/software/gawk/manual/html\_node/Join-Function.html
[awk]: https://www.gnu.org/software/gawk/manual/gawk.html#POSIX\_002fGNU
[gawk]: https://www.gnu.org/software/gawk/manual/gawk.pdf
[gawk2]: https://www.gnu.org/software/gawk/manual/html_node/Floating_002dpoint-Context.html#Floating_002dpoint-Context
[standard]: https://en.wikipedia.org/wiki/IEEE_floating_point
[IEEE-754]: https://www.gnu.org/software/gawk/manual/html_node/Rounding-Mode.html#Rounding-Mode
[mathematical]: https://en.wikipedia.org/wiki/C_mathematical_functions
[basename()]: #basename
[tobase()]: #tobase
[arrjoin()]: #arrjoin
