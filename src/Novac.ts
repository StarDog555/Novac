import { Token_count, Tokenize, Token } from "./lexer.ts";

const args = Deno.args;
const argc = args.length;

let src: string = "";
let file_path: string = "";
let file_length: number = 0;
let Outname: string = "Program";
let tokens: Array<Token> = [];

let Debug = false;
let Compiled = false;

function Exit_With_Error(Message: string): never {
  console.error("[-] ERROR:", Message);
  Deno.exit(1);
}

function print(Message: string) {
  console.log(Message);
}

if (argc === 0) {
  Exit_With_Error("No Input File");
}

// --help
if (args.includes("--help")) {
  print("TODO: Open Local Docs");
}

// --debug
if (args.includes("--debug")) {
  Debug = true;
}

// --compile
if (args.includes("--compile")) {
  const compileIndex = args.indexOf("--compile");

  if (compileIndex !== args.length - 1) {
    Exit_With_Error("--compile needs to be at the end");
  }

  Compiled = true;
}

// -o <NAME>
const outputIndex = args.indexOf("-o");

if (outputIndex !== -1) {
  if (outputIndex + 1 >= args.length) {
    Exit_With_Error("-o requires an output name");
  }

  Outname = args[outputIndex + 1];
}

// .nv file
const file = args.find(arg => arg.toLowerCase().endsWith(".nv"));

if (file === undefined) {
  Exit_With_Error("No .nv File Specified");
}

// Read Nova source
src = await Deno.readTextFile(file);

// Get file information
const info = await Deno.stat(file);

file_path = file;
file_length = info.size;

// Debug information
if (Debug === true) {
  print(`INFO: File Path: ${file_path}`);
  print(`INFO: File Length: ${file_length}`);
  if (Outname === "Program") {
    print("INFO: No FileName, Set To Program");
  }
  print(`INFO: Output Name: ${Outname}`);
  print(`INFO: Compiled: ${Compiled}`);
  print("INFO: Debug: true");
}

// Tokenize
tokens = Tokenize(src, 0);

// Debug token output
if (Debug === true) {
  print(`INFO: Token Count: ${Token_count}`);
  print("INFO: Tokens:");
  
  for (const token of tokens) {
    print(`${token.type} | "${token.value}" | Row: ${token.row} | Col: ${token.col}`
    );
  }
}

if (Compiled === true) {
  print(`TODO: Package Everything Together -> ${Outname}`);
}
