# Language

## Libraries

There inexists a traditional standard library. Use platform-specific
procedures.

```
win32 :: #import "User32.lib" "Kernel32.lib";
```

# Declarations

## Scope Declaration

```
scope-declaration
    : identifier '{' <declaration> '}'
    ;
```

A scope declaration declares a collection of symbols.

Moving in and out of scopes to retrieve:

```
..out1.in1;
...out2.in1.in2;
....out3.in1.in2.in3;
```

## Storage Declarations

```
storage-declaration
    : identifier ':' path
    ;
```

When you declare something, the type can be any identifier. The compiler
extracts the type of the identifier to use as the type of the declared objects,
which means, you don't necessarily declare types; you only declare values.

Using an equals sign or braces to define a declaration means the object has
runtime storage. Essentially, you can retrieve the runtime address of the
object.

Using a colon to define a declaration means it only has compiletime storage.

```
variable: u8 = 0;
array: {u8*3} = {1, 2, 3};
enumeration: {sun, mon, tue} = .sun;
record: {x: f64, y: f64, z: f64} = {x=0,y=0,z=0};
procedure: (n: u8) -> u8 { -> n; }
procedure_pointer: @(n: u8) -> u8 = @d;
CONSTANT: u8 : 32;
Record :: {r: u8, g: u8, b: u8};
using_big_record: Record = {r=0,g=0,b=0};
using_small_record: record = {r=0,g=0,b=0};

x ? y() : z();
x ? {} : {};

week_day ?
    | .sun: do_sunday(),
    | .mon: do_monday(),
    | .tue: do_tuesday(),
    : do_somthing();

week_day ?
    | .sun {}
    | .mon {}
    | .tue {}
    : {}

{4, 2, 3, 1, 2}
    |> enumerate
    |> sort;

float: f32 = 0.0;
uint: u32 = float'u32; ## cast
```

```
#if #os == #os_win32 {
    #import "User32.lib"
}
#elif #os == #os_linux {
}
```
