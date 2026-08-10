> **Nova Compiler**

# Installation (Compiling from Source)

**Novac** (Nova Compiler) Uses a No Build Build System mpp (Make++) Repo > **Link**: https://github.com/StarDog555/mpp

**Why It's Good:**

- **There Is Only One Version** (No need to install seprate Versions) [X]

- **Only Need A C Compiler** (No Need to install tons of build tools) [X]

- **SUPER easy to setup** (No Need to spend hours trying to build stuff) [X]

To Install You Will Need One These C Compilers 
{

**Gcc, CC, Clang, Or Cl**

}

If Your Using Gcc, CC, clang follow this:

Compile it:

```bash
!COMPILER! -o mpp mpp.c
```

Run it:

```bash
./mpp !COMPILER!
```


If Your Using Cl On Windows Follow this:

Open Terminal:

Open Developer Command Prompt for VS 2022 (or your installed version).

Compile it:

```bash 
cl /Fe:mpp.exe mpp.c
```

Run It (**Note**: If Your Using CMD Run mpp !COMPILER!):

```bash 
./mpp !COMPILER!
```

> **Note**: For Any Of These You Will Have to Run the Command In the Project's ROOT Dir

> **Note**: The COMPILER is the Compiler, You want to use

---

# Author

StarDog555