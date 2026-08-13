# **Base Definitions**:

[] = [**NODE**] or [ANYTHING]

\end = [END][EXPE]

; = [END][Part][_Root]

{} = [Part]_Root or [ANYTHING]

_Root = [ROOT][PART][**NODE**]

int_lit = [NUMBER]

[READABLE] = [ONLY][FOR][USER]

| = [Part]/[EXPE][READABLE]

++ = [CAN][REPEAT]

### Examples

- int_lit + int_lit = [EXPE]

- [EXIT] = [Part]_Root

---

# [EXIT]

[_Root]{EXIT} -> {EXPE} | int_lit + int_lit \end;

# [VARIA] (variables)

Int: [_Root]{VARIA} -> {INT} -> {EXPE} | int_lit(EXPE)\end;

Bool: [_Root]{VARIA} -> {BOOL} -> {EXPE} | [TRUE], [FALSE]\end;

> **Note**: TRUE = 1, FALSE = 0

String: [_Root]{VARIA} -> {STRING} -> {EXPE} | {""} | + | {""} | {"{VARIA}"}++  

Means {STRING} containes a {EXPE} that containes a {""} (String) and CAN containe a + to Join 2 Strings together or can containe a {VARIA} and ++ means can containe more then 1 

Obj: [_Root]{VARIA} -> {OBJ} -> {EXPE} | \==/ | {FUNCTION} {VARIA}

Means OBJ Containes a \==/ (Short For Object) can Containe {FUNCTION} And {VARIA}