# Rosetta
This software is based on ScintillaTest and scintilla.

## How to build
This project contains a *scintilla* [^1] submodule. Hence, you must download that sumbmodule.

```
git clone --recursive https://github.com/aindros/rosetta.git
```
If you want to do a simple clone and download scintilla, you can do this after cloned this repository:

```
git submodule update --init https://github.com/aindros/rosetta.git
```

And then, just do:

```
make && ./rosetta
```

[^1]: https://github.com/aindros/scintilla.git
