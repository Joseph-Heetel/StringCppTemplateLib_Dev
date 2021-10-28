# StringCppTemplateLib_Dev

This is the dev repository for [StringHeaderOnly.hpp](https://gist.github.com/Joseph-Heetel/36c76fbaf7dcbc974e4cab16b18bb5d2)

> A single file library exposing a String class combining stringview und refcounted string object functionality into one.
Additionally providing a StringBuilder implementation and methods for writing and parsing built-in types.

Distributed via [gist.github.com](https://gist.github.com/Joseph-Heetel/36c76fbaf7dcbc974e4cab16b18bb5d2)

Licensed under MIT License

## Current features
* String class which can be both a string_view and a refcount managed string (unless you want to mutate the string data you can just not care)
  * Basic string manipulation methods
  * Basic interoperability with std::istream and std::ostream
  * Iterator class for simple read-only iteration through the character sequence
* Stringbuilder class for efficient concatenation of strings
* Methods for parsing and stringifying internal types

## Planned features
* More string manipulation features such as Format functions etc.
* Conversion UTF8 <-> UTF16
* Functionality validation with automatic tests

## Changelog
* `0.1.1` More efficient template usage, inline documentation
* `0.1.0` Initial release
