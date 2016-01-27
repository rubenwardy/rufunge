# Rufunge

Rufunge is a befunge clone which aims to make the language more practical.]

## Aims

* Should be a "superset" of befunge: a befunge program should work in rufunge.
* Adds good support for subprogram libraries.
* Adds file access.

# 1. The Stack

## 1.1 Types

An element in the stack is an unsigned number, 32 bit or larger.

## 1.2 Numbers

When the cursor reaches a number, it pushes that digit onto the stack.  
For example, 123 will end up with a stack of `[3, 2, 1]`. (left most is top most)

The \` can be used to enter into numeric mode.
For example, \`123\` would give you a stack `[123]`.

## 1.3 Strings

Use " to enable string mode.  
For example, "hello" ends up with
`['o', 'l', 'l', 'e', 'h'] = [111, 108, 108, 101, 104]`.

# 2 Standard Operators and Library

`a` is used to refer to the topmost element popped and
`b` to the second topmost element popped.

## 2.1 Arithmetic Operators

| Symbol | Name | Description                   |
| ------ | ---- |------------------------------ |
|   +    | Add  | b + a                         |
|   -    | Sub  | b - a                         |
|   *    | Mul  | b * a                         |
|   /    | Div  | b / a                         |
|   %    | Mod  | b % a                         |

## 2.2 Logical Operators

| Symbol | Name | Description                   |
| ------ | ---- |------------------------------ |
|   !    | Not  | If a == 0, push 1 else push 0 |
|   `    | Grtr | If b > a,  push 1 else push 0 |

## 2.3 Flow Control

| Symbol | Name | Description                   |
| ------ | ---- |------------------------------ |
|   ^    | DirU | Direction Flow                |
|   >    | DirR | Direction Flow                |
|   v    | DirD | Direction Flow                |
|   <    | DirL | Direction Flow                |
|   ?    | RndD | Random direction              |
|   _    | H-If | If a==0, go right else left   |
| &#124; | V-If | If a==0, go down  else up     |
|   #    | brdg | Jump over next statement      |
|   @    | exit | Exit Program                  |
|   [    |  bs  | Skip til next                 |
|	]    |  be  | End skip                      |

## 2.4 Stack Operators

| Symbol | Name | Description                   |
| ------ | ---- |------------------------------ |
| 0 - 9  | Int  | Input an integer digit        |
|   "    | Str  | Toggle string mode            |
|   :    | Dup  | Push a twice                  |
|   \    | Swap | Swap a and b                  |
|   $    | Del  | Pop and discard $             |

## 2.5 Input and Output

| Symbol | Name | Description                   |
| ------ | ---- |------------------------------ |
|   .    | oint | Output a as int               |
|   ,    | ochr | Output a as char              |
|   &    | iint | Input  a as int               |
|   ~    | ichr | Input  a as chat              |
|   g    | get  | a=y, b=x, get (x, y)          |
|   p    | put  | a=y, b=x, c=v. set (x, y) = v |

## 2.6 Core Functions

S, T, U are reverse strings. The first letter in the string is at
the top of the stack.

For example, to pass "hello" as a parameter would be:  
`0"olleh" = ['h', 'e', 'l', 'l', 'o', 0] = [104, 101, 108, 108, 111, 0]`

| Symbol | Name | Description                            |
| ------ | ---- |--------------------------------------- |
|   M    | req  | Load module S. Pop 1 on success else 0 |
|   P    | load | Load subprogram S::R and call it a.    |
|   R    | rtn  | Return to caller                       |
|   L    | lock | Hide the stack before a certain cell - use to stop bugs overwriting previous program's stack elements. |

### Examples

```
v                      Program to find the length of a string
>                   v   
  v M"str"0         <  Load module "str"
v _ 0"rorre"#,:_ @     If failed output error
>                   v
v P"lstr"0"length"0 <  Load str::length, call it l.
> 0"sdlmsdlmsd" l . @  Run l on "sdlmsdlmsd".
```

`str::length` means the subprogram length in the module str.
If the module is a rf module, then there will be some
str/length.rf or stf/length.bf somewhere. If it's a native module,
then there will be some C++ code or some binding somewhere.


# 3 Modules and Subprograms

Modules are collections of rufunge files.  
Each rufunge file is a subprogram.
