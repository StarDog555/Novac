const KeyWords = new Set<string>([
  "exit"
]);

interface EXPE {
  Right: EXPE | null,
  Left: EXPE | null,
  value: number,
  operater: string
}

interface EXIT { E: EXPE }

interface PARTS {
  exit: EXIT
}

interface _Root {
  Parts: PARTS[],
  Part_count: number
}

export {
  EXPE,
  EXIT,
  PARTS,
  _Root,
  KeyWords
};
